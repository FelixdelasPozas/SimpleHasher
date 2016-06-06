/*
 File: SimpleHasher.cpp
 Created on: 05/06/2016
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
#include <SimpleHasher.h>
#include <AboutDialog.h>
#include <hash/MD5.h>
#include <hash/SHA1.h>
#include <hash/SHA224.h>
#include <hash/SHA256.h>
#include <hash/SHA384.h>
#include <hash/SHA512.h>
#include <hash/Tiger.h>
#include <ComputerThread.h>

// Qt
#include <QFileDialog>
#include <QStringListModel>

//----------------------------------------------------------------
SimpleHasher::SimpleHasher(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow{parent, flags}
, m_thread   {nullptr}
{
  setupUi(this);

  hideProgress();

  QStringList labels = { tr("Filename"), tr("SHA-1") };

  m_hashTable->setColumnCount(2);
  m_hashTable->setAlternatingRowColors(true);
  m_hashTable->setSortingEnabled(false);
  m_hashTable->setHorizontalHeaderLabels(labels);
  m_hashTable->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
  m_hashTable->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
  m_hashTable->horizontalHeader()->setSectionsMovable(false);
  m_hashTable->horizontalHeader()->setSectionsClickable(false);
  m_hashTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
  m_hashTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  m_hashTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

  connectSignals();
}

//----------------------------------------------------------------
void SimpleHasher::onAboutPressed()
{
  AboutDialog dialog;
  dialog.exec();
}

//----------------------------------------------------------------
void SimpleHasher::connectSignals()
{
  connect(m_about,      SIGNAL(pressed()), this, SLOT(onAboutPressed()));
  connect(m_quit,       SIGNAL(pressed()), this, SLOT(close()));
  connect(m_cancel,     SIGNAL(pressed()), this, SLOT(onCancelPressed()));
  connect(m_addFile,    SIGNAL(pressed()), this, SLOT(onAddFilesPressed()));
  connect(m_removeFile, SIGNAL(pressed()), this, SLOT(onRemoveFilePressed()));
  connect(m_compute,    SIGNAL(pressed()), this, SLOT(onComputePressed()));
  connect(m_save,       SIGNAL(pressed()), this, SLOT(onSavePressed()));

  connect(m_md5,    SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
  connect(m_sha1,   SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
  connect(m_sha224, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
  connect(m_sha256, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
  connect(m_sha384, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
  connect(m_sha512, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
  connect(m_tiger,  SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
}

//----------------------------------------------------------------
void SimpleHasher::hideProgress()
{
  m_progress->hide();
  m_cancel->hide();
}

//----------------------------------------------------------------
void SimpleHasher::showProgress()
{
  m_progress->setValue(0);
  m_progress->show();
  m_cancel->show();
}

//----------------------------------------------------------------
void SimpleHasher::onAddFilesPressed()
{
  static auto directory = QDir::currentPath();

  auto files = QFileDialog::getOpenFileNames(centralWidget(), tr("Add files"), directory);

  if(!files.isEmpty())
  {
    // update dir value
    directory = QFileInfo{files.first()}.path();

    addFilesToTable(files);
  }
}

//----------------------------------------------------------------
void SimpleHasher::onRemoveFilePressed()
{
  auto index = m_hashTable->currentRow();

  m_hashTable->removeRow(index);
  m_files.removeAt(index);
}

//----------------------------------------------------------------
void SimpleHasher::onComputePressed()
{
  HashList hashes;

  if(m_md5->isChecked())    hashes << std::make_shared<MD5>();
  if(m_sha1->isChecked())   hashes << std::make_shared<SHA1>();
  if(m_sha224->isChecked()) hashes << std::make_shared<SHA224>();
  if(m_sha256->isChecked()) hashes << std::make_shared<SHA256>();
  if(m_sha384->isChecked()) hashes << std::make_shared<SHA384>();
  if(m_sha512->isChecked()) hashes << std::make_shared<SHA512>();
  if(m_tiger->isChecked())  hashes << std::make_shared<Tiger>();

  m_thread = std::make_shared<ComputerThread>(m_files, hashes);
  showProgress();

  connect(m_thread.get(), SIGNAL(progress(int)), m_progress, SLOT(setValue(int)));
  connect(m_thread.get(), SIGNAL(finished()), this, SLOT(onComputationFinished()));

  m_thread->start();
}

//----------------------------------------------------------------
void SimpleHasher::onCancelPressed()
{
  m_thread->abort();
}

//----------------------------------------------------------------
void SimpleHasher::onSavePressed()
{
  // TODO
}

//----------------------------------------------------------------
void SimpleHasher::onCheckBoxStateChanged(int value)
{
  auto checkBox = qobject_cast<QCheckBox *>(sender());

  if(checkBox == m_md5)
  {

  }

  if(checkBox == m_sha1)
  {

  }

  if(checkBox == m_sha224)
  {

  }

  if(checkBox == m_sha256)
  {

  }

  if(checkBox == m_sha384)
  {

  }

  if(checkBox == m_sha512)
  {

  }

  if(checkBox == m_tiger)
  {

  }
}

//----------------------------------------------------------------
void SimpleHasher::onComputationFinished()
{
  auto results = m_thread->getResults();

  for(int i = 0; i < m_files.size(); ++i)
  {
    for(auto hash: results[m_files.at(i)])
    {
      auto label = qobject_cast<QLabel *>(m_hashTable->cellWidget(i, 1));
      label->setText(hash->value());
    }
  }

  for(auto list: results.values())
  {
    list.clear();
  }

  m_thread = nullptr;
  hideProgress();
}

//----------------------------------------------------------------
void SimpleHasher::addFilesToTable(const QStringList &files)
{
  m_hashTable->setEnabled(true);

  int columnCount = 1;
  if(m_md5->isChecked())    ++columnCount;
  if(m_sha1->isChecked())   ++columnCount;
  if(m_sha224->isChecked()) ++columnCount;
  if(m_sha256->isChecked()) ++columnCount;
  if(m_sha384->isChecked()) ++columnCount;
  if(m_sha512->isChecked()) ++columnCount;
  if(m_tiger->isChecked())  ++columnCount;

  for(auto file: files)
  {
    if(m_files.contains(file))
    {
      continue;
    }

    m_files << file;

    auto row = m_hashTable->rowCount();
    auto filename = file.split(QChar('/')).last();

    m_hashTable->insertRow(row);
    m_hashTable->setCellWidget(row, 0, new QLabel{filename});

    for(int column = 1; column <= columnCount; ++column)
    {
      auto widget = new QLabel{"Not computed"};
      widget->setAlignment(Qt::AlignCenter);

      m_hashTable->setCellWidget(row, column, widget);
    }
  }

  auto enabled = (m_hashTable->rowCount() != 0);
  if(enabled)
  {
    m_hashTable->resizeColumnToContents(0);
  }

  m_removeFile->setEnabled(enabled);
  m_compute->setEnabled(enabled);
}
