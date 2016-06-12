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
#include <QThreadPool>
#include <QApplication>

//----------------------------------------------------------------
ComputerThread::ComputerThread(QMap<QString, HashList> computations, const int threadsNum, QObject *parent)
: QThread       {parent}
, m_computations{computations}
, m_abort       {false}
, m_hashNumber  {0}
, m_progress    {0}
, m_maxThreads  {threadsNum}
, m_threadsNum  {0}
{
  qRegisterMetaType<const Hash *>("constHashPtr");

  for(auto file: m_computations.keys())
  {
    m_hashNumber += m_computations[file].size();
  }

  if(m_maxThreads == -1)
  {
    m_maxThreads = QThreadPool::globalInstance()->maxThreadCount();
  }
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
void ComputerThread::onHashComputed(const QString &filename, const Hash *hash)
{
  QMutexLocker lock(&m_progressMutex);

  emit hashComputed(filename, hash);

  ++m_progress;

  emit progress((100*m_progress)/m_hashNumber);

  --m_threadsNum;
  m_condition.wakeAll();
}

//----------------------------------------------------------------
void ComputerThread::run()
{
  QString fileErrors;
  QList<std::shared_ptr<HashChecker>> threads;

  for(auto filename: m_computations.keys())
  {
    for(auto hash: m_computations[filename])
    {
      auto file = new QFile{filename};

      if(!file->open(QIODevice::ReadOnly) || !file->seek(0))
      {
        fileErrors.append(tr("%1 error: %2\n").arg(filename).arg(file->errorString()));
        delete file;
        continue;
      }

      auto runnable = std::make_shared<HashChecker>(hash, file);
      connect(runnable.get(), SIGNAL(hashComputed(const QString &, const Hash *)), this, SLOT(onHashComputed(const QString &, const Hash *)));

      threads << runnable;

      // wait if we have reached the maximum number of threads of the system.
      if(m_threadsNum == m_maxThreads)
      {
        m_mutex.lock();
        m_condition.wait(&m_mutex);
        m_mutex.unlock();
      }

      ++m_threadsNum;
      runnable->start();
    }
  }

  QApplication::processEvents();

  for(auto thread: threads)
  {
    // waits for all threads to finish before ending.
    thread->wait();
  }
  threads.clear();

  if(!fileErrors.isEmpty())
  {
    QMessageBox dialog;

    dialog.setWindowIcon(QIcon(":/SimpleHasher/application.svg"));
    dialog.setWindowTitle(tr("Errors in hash computation"));
    dialog.setText(tr("Some files couldn't be processed."));
    dialog.setDetailedText(fileErrors);
    dialog.setIcon(QMessageBox::Icon::Critical);

    dialog.exec();
  }
}
