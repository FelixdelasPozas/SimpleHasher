/*
 File: ComputerThread.h
 Created on: 06/06/2016
 Author: Felix de las Pozas Alvarez

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COMPUTERTHREAD_H_
#define COMPUTERTHREAD_H_

// Project
#include <Hash.h>

// Qt
#include <QThread>
#include <QMap>
#include <QMutex>
#include <QWaitCondition>

// C++
#include <atomic>
#include <iostream>

/** \class HashChecker
 * \brief Thread for computing an individual hash.
 *
 */
class HashChecker
: public QThread
{
    Q_OBJECT
  public:
    /** \brief HashChecker class constructor.
     * \param[in] hash hash object to update.
     * \param[in] file opened QFile object.
     * \param[in] parent raw pointer of the object parent of this one.
     *
     */
    HashChecker(std::shared_ptr<Hash> hash, QFile *file, QObject *parent = nullptr)
    : QThread   {parent}
    , m_hash    {hash}
    , m_file    {file}
    , m_progress{0}
    , m_abort   {false}
    {};

    /** \brief HashChecker class virtual destructor.
     *
     */
    ~HashChecker()
    {
      m_file->close();
      delete m_file;
    }

    /** \brief Returns the progress of the hash computation in [0,100].
     *
     */
    const int progress() const
    { return m_progress; }

    /** \brief Aborts the current hash computation.
     *
     */
    void abort()
    { if(isRunning()) m_abort = true; }

    /** \brief Returns true if aborted and false otherwise.
     *
     */
    const bool aborted() const
    { return m_abort; }

  signals:
    void hashComputed(const QString &filename, const Hash *hash);
    void hashUpdated(const QString &filename, const Hash *hash, const int value);
    void progressed();

  protected:
    void run()
    {
      m_progress = 0;
      m_file->seek(0);
      unsigned long long message_length = 0;
      const unsigned long long fileSize = m_file->size();
      m_hash->reset();

      while(fileSize != message_length && !m_abort)
      {
        int currentProgress = (message_length*100.)/fileSize;
        if(currentProgress != m_progress)
        {
          m_progress = currentProgress;
          emit progressed();
          emit hashUpdated(m_file->fileName(), m_hash.get(), m_progress);
        }

        auto block = m_file->read(m_hash->blockSize());
        message_length += block.length();
        m_hash->update(block, message_length * m_hash->bitsPerUnit());

        // last block needs to be processed
        if((fileSize == message_length) && (block.size() == m_hash->blockSize()))
        {
          m_hash->update(QByteArray(), message_length * m_hash->bitsPerUnit());
        }
      }

      m_file->close();
      if(!m_abort) emit hashComputed(m_file->fileName(), m_hash.get());
    }

  private:
    std::shared_ptr<Hash> m_hash;     /** hash object to update.                 */
    QFile                *m_file;     /** opened QFile object.                   */
    int                   m_progress; /** computation progress value in [0,100]. */
    bool                  m_abort;    /** true if aborted, false otherwise.      */

};

/** \class ComputerThread
 * \brief Class to compute the hashes in a separate thread.
 *
 */
class ComputerThread
: public QThread
{
    Q_OBJECT
  public:
    /** \brief ComputeThread class constructor.
     * \param[in] computations maps files to hashes to be computed.
     * \param[in] threadsNum number of simultaneous threads or -1 for system's maximum value.
     *
     */
    ComputerThread(QMap<QString, HashList> computations, const int threadsNum, QObject *parent = nullptr);

    /** \brief ComputeThread class virtual destructor.
     *
     */
    virtual ~ComputerThread()
    {};

    /** \brief Returns the map of file hashes.
     *
     */
    QMap<QString, HashList> getResults() const;

    /** \brief Stops computation and returns.
     *
     */
    void abort();

    /** \brief Returns true if the process has been aborted.
     *
     */
    bool isAborted() const
    { return m_abort; }

    /** \brief Returns the errors computing hashes, or empty if none.
     *
     */
    QString getErrors() const
    { return m_fileErrors; }

  signals:
    void progress(int value);
    void hashComputed(const QString &filename, const Hash *hash);
    void hashUpdated(const QString &filename, const Hash *hash, const int value);

  private slots:
    /** \brief Computes progress and emits progress and hashComputed signals.
     * \param[in] filename filename of the computed hash.
     * \param[in] hash hash object pointer.
     */
    void onHashComputed(const QString &filename, const Hash *hash);

    /** \brief Updates the total progress of the computations and reports it.
     *
     */
    void onProgressSignaled();

    /** \brief Removes the finished thread from the thread pool.
     *
     */
    void onThreadFinished();

  protected:
    virtual void run();

  private:
    QMap<QString, HashList> m_computations;  /** maps the files with the hashes to be computed.                 */
    bool                    m_abort;         /** set to true to stop computing and return ASAP.                 */
    int                     m_hashNumber;    /** total number of hashes to compute.                             */
    std::atomic<int>        m_progress;      /** progress accumulator.                                          */
    QMutex                  m_progressMutex; /** protects the progress variable.                                */
    QMutex                  m_mutex;         /** mutex for the wait condition.                                  */
    QWaitCondition          m_condition;     /** wait condition for the main thread.                            */
    int                     m_maxThreads;    /** max number of threads in the system.                           */
    std::atomic<int>        m_threadsNum;    /** number of threads currently running.                           */
    QMap<QString, HashList> m_results;       /** computed hashes.                                               */
    QString                 m_fileErrors;    /** hash errors or empty if none.        */

    QList<std::shared_ptr<HashChecker>> m_threads; /** list of running threads. */
};

#endif // COMPUTERTHREAD_H_
