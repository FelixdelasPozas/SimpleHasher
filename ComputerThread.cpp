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

// Qt
#include <QMessageBox>
#include <QIcon>

//----------------------------------------------------------------
ComputerThread::ComputerThread(QMap<QString, HashList> computations, QObject *parent)
: QThread       {parent}
, m_computations{computations}
, m_abort       {false}
, m_currentIndex{0}
, m_hashIndex   {0}
{
  qRegisterMetaType<const Hash *>("constHashPtr");
}

//----------------------------------------------------------------
QMap<QString, HashList> ComputerThread::getResults() const
{
  return m_computations;
}

//----------------------------------------------------------------
void ComputerThread::abort()
{
  m_abort = true;
}

//----------------------------------------------------------------
void ComputerThread::hashProgress(int value)
{
  auto filesSize = m_computations.size();
  auto hashSize  = m_computations[m_computations.keys().at(m_currentIndex)].size();
  int fileProgress = (100 * m_currentIndex)/filesSize;
  int hashProgress = (100 * m_hashIndex)/(filesSize * hashSize);

  emit progress(fileProgress + hashProgress + value/(filesSize * hashSize));
}

//----------------------------------------------------------------
void ComputerThread::run()
{
  auto size = m_computations.keys().size();
  QString fileErrors;

  for(m_currentIndex = 0; m_currentIndex < size && !m_abort; ++m_currentIndex)
  {
    // TODO: connect to hash signals to provide a better progress value.
    emit progress((100 *(m_currentIndex+1))/size);

    auto filename = m_computations.keys().at(m_currentIndex);
    QFile file{filename};

    if(!file.open(QIODevice::ReadOnly) || !file.seek(0))
    {
      fileErrors.append(tr("%1 error: %2\n").arg(filename).arg(file.errorString()));
      continue;
    }

    for(m_hashIndex = 0; m_hashIndex < m_computations[filename].size(); ++m_hashIndex)
    {
      auto hash = m_computations[filename].at(m_hashIndex);

      connect(hash.get(), SIGNAL(progress(int)), this, SLOT(hashProgress(int)), Qt::DirectConnection);
      hash->update(file);
      disconnect(hash.get(), SIGNAL(progress(int)), this, SLOT(hashProgress(int)));
      file.seek(0);

      emit hashComputed(filename, hash.get());
    }
  }

  if(!fileErrors.isEmpty())
  {
    QMessageBox dialog;

    dialog.setWindowIcon(QIcon(":/SimpleHasher/application.svg"));
    dialog.setWindowTitle(tr("Errors in hash computation"));
    dialog.setText(tr("Some files couldn't be processed."));
    dialog.setDetailedText(fileErrors);

    dialog.exec();
  }
}
