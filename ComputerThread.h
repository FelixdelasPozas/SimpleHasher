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

    /** \brief ComputeThread clss virtual destructor.
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

  signals:
    void progress(int value);

    void hashComputed(const QString &filename, const Hash *hash);

  private slots:
    void onHashComputed(const QString &filename, const Hash *hash);

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
};

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
    : QThread{parent}
    , m_hash {hash}
    , m_file {file}
    {};

    /** \brief HashChecker class virtual destructor.
     *
     */
    ~HashChecker()
    {
      m_file->close();
      delete m_file;
    }

  signals:
    void hashComputed(const QString &filename, const Hash *hash);

  protected:
    void run()
    {
      m_hash->update(*m_file);

      emit hashComputed(m_file->fileName(), m_hash.get());
    }

  private:
    std::shared_ptr<Hash> m_hash; /** hash object to update. */
    QFile                *m_file; /** opened QFile object.   */
};



#endif // COMPUTERTHREAD_H_
