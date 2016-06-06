/*
 File: ComputerThread.cpp
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

// Project
#include <ComputerThread.h>

//----------------------------------------------------------------
ComputerThread::ComputerThread(const QStringList &files, const HashList &hashes, QObject *parent)
: QThread {parent}
, m_files {files}
, m_hashes{hashes}
, m_abort {false}
{
}

//----------------------------------------------------------------
QMap<QString, HashList> ComputerThread::getResults() const
{
  return m_fileHashes;
}

//----------------------------------------------------------------
void ComputerThread::abort()
{
  m_abort = true;
}

//----------------------------------------------------------------
void ComputerThread::run()
{
  for(int i = 0; i < m_files.size(); ++i)
  {
    // TODO: connect to hash signals to provide a better progress value.
    emit progress((100 *(i+1))/m_files.size());

    auto filename = m_files.at(i);
    QFile file{filename};

    if(!file.open(QIODevice::ReadOnly) || !file.seek(0))
    {
      // TODO: error message
      continue;
    }

    for(auto hash: m_hashes)
    {
      auto fileHash = hash->clone();
      fileHash->update(file);

      m_fileHashes[filename] << fileHash;
    }
  }
}
