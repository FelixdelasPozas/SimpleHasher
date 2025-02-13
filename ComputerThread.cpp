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
, m_computations(computations)
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
  return m_results;
}

//----------------------------------------------------------------
void ComputerThread::abort()
{
  m_abort = true;
  m_condition.wakeAll();
}

//----------------------------------------------------------------
void ComputerThread::onHashComputed(const QString &filename, const Hash *hash)
{
  QMutexLocker lock(&m_progressMutex);

  for(auto hashSPtr: m_computations[filename])
  {
    if(hashSPtr.get() == hash)
    {
      m_results[filename] << hashSPtr;
      break;
    }
  }

  emit hashComputed(filename, hash);
}

//----------------------------------------------------------------
void ComputerThread::onProgressSignaled()
{
  QMutexLocker lock(&m_progressMutex);

  if(m_hashNumber > 0)
  {
    auto progressValue = m_progress * 100;

    std::for_each(m_threads.constBegin(), m_threads.constEnd(), [&progressValue](const std::shared_ptr<HashChecker> task) { if(task) progressValue += task->progress();});

    emit progress(progressValue/m_hashNumber);
  }
}

//----------------------------------------------------------------
void ComputerThread::run()
{
  m_fileErrors.clear();

  for(int i = 0; i < m_computations.keys().size() && !m_abort; ++i)
  {
    auto filename = m_computations.keys().at(i);

    for(auto hash: m_computations[filename])
    {
      // wait if we have reached the maximum number of threads.
      if(m_threadsNum == m_maxThreads)
      {
        m_mutex.lock();
        m_condition.wait(&m_mutex);
        m_mutex.unlock();
      }

      if(m_abort) break;

      auto file = new QFile{filename};

      if(!file->open(QIODevice::ReadOnly) || !file->seek(0))
      {
        m_fileErrors.append(tr("%1 error: %2\n").arg(filename).arg(file->errorString()));
        delete file;
        continue;
      }

      auto runnable = std::make_shared<HashChecker>(hash, file);
      connect(runnable.get(), SIGNAL(hashComputed(const QString &, const Hash *)), this, SLOT(onHashComputed(const QString &, const Hash *)));
      connect(runnable.get(), SIGNAL(hashUpdated(const QString &, const Hash *, const int)), this, SIGNAL(hashUpdated(const QString &, const Hash *, const int)));
      connect(runnable.get(), SIGNAL(progressed()), this, SLOT(onProgressSignaled()));
      connect(runnable.get(), SIGNAL(finished()), this, SLOT(onThreadFinished()));

      {
        QMutexLocker lock(&m_progressMutex);

        ++m_threadsNum;
        m_threads << runnable;
      }
      runnable->start();
    }
  }

  while(!m_threads.isEmpty() && !m_abort)
  {
    m_mutex.lock();
    m_condition.wait(&m_mutex);
    m_mutex.unlock();
  }

  if(m_abort)
  {
    QMutexLocker lock(&m_progressMutex);

    std::for_each(m_threads.begin(), m_threads.end(), [](std::shared_ptr<HashChecker> thread) { thread->abort(); });
  }

  QApplication::processEvents();
}

//----------------------------------------------------------------
void ComputerThread::onThreadFinished()
{
  QMutexLocker lock(&m_progressMutex);

  --m_threadsNum;
  ++m_progress;

  auto senderThread = qobject_cast<HashChecker *>(sender());
  if(senderThread)
  {
    disconnect(senderThread, SIGNAL(hashComputed(const QString &, const Hash *)), this, SLOT(onHashComputed(const QString &, const Hash *)));
    disconnect(senderThread, SIGNAL(hashUpdated(const QString &, const Hash *, const int)), this, SIGNAL(hashUpdated(const QString &, const Hash *, const int)));
    disconnect(senderThread, SIGNAL(progressed()), this, SLOT(onProgressSignaled()));
    disconnect(senderThread, SIGNAL(finished()), this, SLOT(onThreadFinished()));

    for(auto thread: m_threads)
    {
      if(thread.get() == senderThread)
      {
        m_threads.removeOne(thread);
        break;
      }
    }
  }

  m_condition.wakeAll();
}
