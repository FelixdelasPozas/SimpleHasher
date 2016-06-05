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

// Qt
#include <QFileDialog>

//----------------------------------------------------------------
SimpleHasher::SimpleHasher(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow{parent, flags}
{
  setupUi(this);

  hideProgress();

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
  // TODO
}

//----------------------------------------------------------------
void SimpleHasher::onCancelPressed()
{
  // TODO
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
void SimpleHasher::addFilesToTable(const QStringList &files)
{
  for(auto file: files)
  {
    // TODO
  }
}
