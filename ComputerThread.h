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
     * \param[in] parent pointer of the QObject parent of this one.
     * \param[in] files list of file names.
     * \param[in] hashes list of hash objects.
     *
     */
    ComputerThread(const QStringList &files, const HashList &hashes, QObject *parent = nullptr);

    /** \brief ComputeThread clss virtual destructor.
     *
     */
    virtual ~ComputerThread()
    {};

    /** \brief Returns the map of file hashes.
     *
     */
    QMap<QString, HashList> getResults() const;

  signals:
    void progress(int value);

  protected:
    virtual void run();

  private:
    QStringList             m_files;      /** list of file names with absolute path to compute hashes for. */
    HashList                m_hashes;     /** list of hash objects.                                        */
    QMap<QString, HashList> m_fileHashes; /** results.                                                     */
};

#endif // COMPUTERTHREAD_H_
