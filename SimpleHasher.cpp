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
#include <ConfigurationDialog.h>

// Qt
#include <QFileDialog>
#include <QStringListModel>
#include <QFontDatabase>
#include <QSettings>
#include <QVariant>
#include <QMessageBox>
#include <QMenu>
#include <QClipboard>

QString SimpleHasher::STATE_MD5         = QString("MD5 Enabled");
QString SimpleHasher::STATE_SHA1        = QString("SHA-1 Enabled");
QString SimpleHasher::STATE_SHA224      = QString("SHA-224 Enabled");
QString SimpleHasher::STATE_SHA256      = QString("SHA-256 Enabled");
QString SimpleHasher::STATE_SHA384      = QString("SHA-384 Enabled");
QString SimpleHasher::STATE_SHA512      = QString("SHA-512 Enabled");
QString SimpleHasher::STATE_TIGER       = QString("Tiger Enabled");
QString SimpleHasher::GEOMETRY          = QString("Application Geometry");
QString SimpleHasher::OPTIONS_ONELINE   = QString("Hash in one line");
QString SimpleHasher::OPTIONS_UPPERCASE = QString("Hash in uppercase");
QString SimpleHasher::OPTIONS_SPACES    = QString("Break hash with spaces");

//----------------------------------------------------------------
SimpleHasher::SimpleHasher(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow{parent, flags}
, m_thread   {nullptr}
, m_spaces   {true}
, m_oneline  {false}
, m_uppercase{false}
{
  setupUi(this);

  QStringList labels = { tr("Filename") };

  m_hashTable->setColumnCount(1);
  m_hashTable->setAlternatingRowColors(true);
  m_hashTable->setSortingEnabled(false);
  m_hashTable->setHorizontalHeaderLabels(labels);
  m_hashTable->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
  m_hashTable->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
  m_hashTable->horizontalHeader()->setSectionsMovable(false);
  m_hashTable->horizontalHeader()->setSectionsClickable(false);
  m_hashTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
  m_hashTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

  loadSettings();

  onCheckBoxStateChanged();

  hideProgress();

  createContextMenu();

  connectSignals();
}

//----------------------------------------------------------------
SimpleHasher::~SimpleHasher()
{
  saveSettings();
}

//----------------------------------------------------------------
void SimpleHasher::onAboutPressed()
{
  AboutDialog dialog{centralWidget()};
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
  connect(m_options,    SIGNAL(pressed()), this, SLOT(onOptionsPressed()));

  connect(m_md5,    SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged()));
  connect(m_sha1,   SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged()));
  connect(m_sha224, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged()));
  connect(m_sha256, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged()));
  connect(m_sha384, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged()));
  connect(m_sha512, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged()));
  connect(m_tiger,  SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged()));
}

//----------------------------------------------------------------
void SimpleHasher::hideProgress()
{
  m_hashGroup->setEnabled(true);
  m_progress->hide();
  m_cancel->hide();
  m_cancel->setEnabled(true);
}

//----------------------------------------------------------------
void SimpleHasher::showProgress()
{
  m_hashGroup->setEnabled(false);
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
  auto selectedIndexes = m_hashTable->selectionModel()->selectedIndexes();
  if(selectedIndexes.empty())
  {
    QMessageBox dialog(centralWidget());
    dialog.setWindowIcon(QIcon(":/SimpleHasher/application.svg"));
    dialog.setWindowTitle(tr("Can't remove files"));
    dialog.setText(tr("You must select at least one cell from the file to be removed."));
    dialog.setIcon(QMessageBox::Icon::Information);

    dialog.exec();
  }

  std::vector<int> selectedRows;
  for(auto index: selectedIndexes)
  {
    selectedRows.push_back(index.row());
  }

  auto compare = [](int a, int b) { return a > b; };
  std::sort(selectedRows.begin(), selectedRows.end(), compare);


  while(!selectedRows.empty())
  {
    m_hashTable->removeRow(selectedRows.front());
    m_files.removeAt(selectedRows.front());
    selectedRows.erase(selectedRows.begin());
  }
}

//----------------------------------------------------------------
void SimpleHasher::onComputePressed()
{
  HashList hashes;

  QMap<QString, HashList> computations;

  for(int i = 0; i < m_files.size(); ++i)
  {
    hashes.clear();
    auto file = m_files.at(i);

    if(m_md5->isChecked()    && !m_results[file].keys().contains("MD5"))     hashes << std::make_shared<MD5>();
    if(m_sha1->isChecked()   && !m_results[file].keys().contains("SHA-1"))   hashes << std::make_shared<SHA1>();
    if(m_sha224->isChecked() && !m_results[file].keys().contains("SHA-224")) hashes << std::make_shared<SHA224>();
    if(m_sha256->isChecked() && !m_results[file].keys().contains("SHA-256")) hashes << std::make_shared<SHA256>();
    if(m_sha384->isChecked() && !m_results[file].keys().contains("SHA-384")) hashes << std::make_shared<SHA384>();
    if(m_sha512->isChecked() && !m_results[file].keys().contains("SHA-512")) hashes << std::make_shared<SHA512>();
    if(m_tiger->isChecked()  && !m_results[file].keys().contains("Tiger"))   hashes << std::make_shared<Tiger>();

    if(!hashes.empty())
    {
      computations.insert(file, hashes);
    }
  }

  if(!computations.empty())
  {
    m_thread = std::make_shared<ComputerThread>(computations);
    showProgress();

    connect(m_thread.get(), SIGNAL(progress(int)), m_progress, SLOT(setValue(int)));
    connect(m_thread.get(), SIGNAL(finished()), this, SLOT(onComputationFinished()));
    connect(m_thread.get(), SIGNAL(hashComputed(const QString &, const Hash *)), this, SLOT(onHashComputed(const QString &, const Hash *)), Qt::DirectConnection);

    m_thread->start();
  }
}

//----------------------------------------------------------------
void SimpleHasher::onCancelPressed()
{
  m_cancel->setEnabled(false);
  m_thread->abort();
}

//----------------------------------------------------------------
void SimpleHasher::onSavePressed()
{
  static QString dir = QDir::currentPath();

  auto selectedDir = QFileDialog::getExistingDirectory(centralWidget(), tr("Select directory for output files"), dir);

  if(!selectedDir.isEmpty())
  {
    // set the value of dir first
    dir = selectedDir;

    QDir outputdir{selectedDir};
    QStringList notSaved;

    for(int index = 1; index < m_headers.size(); ++index)
    {
      if(!m_results[m_files[0]].keys().contains(m_headers.at(index)))
      {
        notSaved << m_headers.at(index);
        continue;
      }

      auto hash     = m_headers.at(index);
      auto filename = hash.remove('-').toUpper() + tr("SUMS");
      auto filepath = outputdir.absoluteFilePath(filename);

      QFile file{filepath};

      if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
      {
        QMessageBox dialog(centralWidget());
        dialog.setWindowIcon(QIcon(":/SimpleHasher/application.svg"));
        dialog.setWindowTitle(tr("Error saving hashes"));
        dialog.setText(tr("Couldn't create file '%1'.").arg(filename));
        dialog.setDetailedText(tr("Error: ") + file.errorString());
        dialog.setIcon(QMessageBox::Icon::Information);

        dialog.exec();
        return;
      }

      QByteArray data;

      for(int i = 0; i < m_files.size(); ++i)
      {
        auto hashText = m_results[m_files.at(i)][m_headers.at(index)]->value();
        auto name     = m_files.at(i).split(QChar('/')).last();
        data.append(hashText.remove('\n').remove(' ').toLower());
        data.append(tr(" *%1%2").arg(name).arg((i == m_files.size()-1 ? "" : "\n")));
      }

      file.write(data);
      if(!file.flush())
      {
        QMessageBox dialog(centralWidget());
        dialog.setWindowIcon(QIcon(":/SimpleHasher/application.svg"));
        dialog.setWindowTitle(tr("Error saving hashes"));
        dialog.setText(tr("Couldn't create file '%1'.").arg(filename));
        dialog.setDetailedText(tr("Error: ") + file.errorString());
        dialog.setIcon(QMessageBox::Icon::Information);

        dialog.exec();

        QFile::remove(filepath);
        return;
      }

      file.close();
    }
  }
}

//----------------------------------------------------------------
void SimpleHasher::onCheckBoxStateChanged()
{
  QStringList labels = { tr("Filename") };

  if(m_md5->isChecked())    labels << tr("MD5");
  if(m_sha1->isChecked())   labels << tr("SHA-1");
  if(m_sha224->isChecked()) labels << tr("SHA-224");
  if(m_sha256->isChecked()) labels << tr("SHA-256");
  if(m_sha384->isChecked()) labels << tr("SHA-384");
  if(m_sha512->isChecked()) labels << tr("SHA-512");
  if(m_tiger->isChecked())  labels << tr("Tiger");

  if(labels.size() == 1)
  {
    auto checkbox = qobject_cast<QCheckBox *>(sender());
    checkbox->setChecked(true);

    QMessageBox dialog(centralWidget());
    dialog.setWindowIcon(QIcon(":/SimpleHasher/application.svg"));
    dialog.setWindowTitle(tr("Error selecting hash algorithms"));
    dialog.setText(tr("At least one of the hash algorithms must be selected."));
    dialog.setIcon(QMessageBox::Icon::Information);

    dialog.exec();

    return;
  }

  for(int i = m_hashTable->columnCount(); i > labels.size(); --i)
  {
    m_hashTable->removeColumn(i-1);
  }

  for(int i = m_hashTable->columnCount(); i < labels.size(); ++i)
  {
    m_hashTable->insertColumn(i);
  }

  for(int row = 0; row < m_files.size(); ++row)
  {
    for(int column = 1; column < labels.size(); ++column)
    {
      QString text{"Not Computed"};

      if(m_results[m_files.at(row)].contains(labels.at(column)))
      {
        auto hash = m_results[m_files.at(row)][labels.at(column)];
        text = hash->value();

        if(m_oneline)   text = text.replace('\n', ' ');
        if(!m_spaces)   text = text.remove(' ');
        if(m_uppercase) text = text.toUpper();
      }

      auto widget = new QLabel{text};
      widget->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
      widget->setAlignment(Qt::AlignCenter);
      widget->setContentsMargins(2, 2, 2, 2);

      m_hashTable->setCellWidget(row, column, widget);
    }
  }

  m_headers = labels;
  m_hashTable->setHorizontalHeaderLabels(labels);
  m_hashTable->resizeColumnsToContents();
}

//----------------------------------------------------------------
void SimpleHasher::onComputationFinished()
{
  disconnect(m_thread.get(), SIGNAL(progress(int)), m_progress, SLOT(setValue(int)));
  disconnect(m_thread.get(), SIGNAL(finished()), this, SLOT(onComputationFinished()));
  disconnect(m_thread.get(), SIGNAL(hashComputed(const QString &, const Hash *)), this, SLOT(onHashComputed(const QString &, const Hash *)));

  auto results = m_thread->getResults();

  for(int i = 0; i < m_files.size(); ++i)
  {
    for(auto hash: results[m_files.at(i)])
    {
      m_results[m_files.at(i)][hash->name()] = hash;
    }
  }

  for(auto list: results.values())
  {
    list.clear();
  }

  m_thread = nullptr;
  for(int i = 0; i < m_hashTable->columnCount(); ++i)
  {
    m_hashTable->resizeColumnToContents(i);
  }

  m_save->setEnabled(true);

  hideProgress();
}

//----------------------------------------------------------------
void SimpleHasher::loadSettings()
{
  QSettings settings{"SimpleHasher.ini", QSettings::IniFormat};

  auto geometry = settings.value(GEOMETRY, QByteArray()).toByteArray();
  if(geometry.length() != 0)
  {
    restoreGeometry(geometry);
  }

  settings.beginGroup("HashAlgorithms");
  m_md5   ->setChecked(settings.value(STATE_MD5, false).toBool());
  m_sha1  ->setChecked(settings.value(STATE_SHA1, true).toBool());
  m_sha224->setChecked(settings.value(STATE_SHA224, false).toBool());
  m_sha256->setChecked(settings.value(STATE_SHA256, false).toBool());
  m_sha384->setChecked(settings.value(STATE_SHA384, false).toBool());
  m_sha512->setChecked(settings.value(STATE_SHA512, false).toBool());
  m_tiger ->setChecked(settings.value(STATE_TIGER, false).toBool());
  settings.endGroup();

  settings.beginGroup("Options");
  m_oneline   = settings.value(OPTIONS_ONELINE, false).toBool();
  m_spaces    = settings.value(OPTIONS_SPACES, true).toBool();
  m_uppercase = settings.value(OPTIONS_UPPERCASE, false).toBool();
  settings.endGroup();
}

//----------------------------------------------------------------
void SimpleHasher::saveSettings()
{
  QSettings settings{"SimpleHasher.ini", QSettings::IniFormat};

  settings.setValue(GEOMETRY, saveGeometry());

  settings.beginGroup("HashAlgorithms");
  settings.setValue(STATE_MD5,    m_md5->isChecked());
  settings.setValue(STATE_SHA1,   m_sha1->isChecked());
  settings.setValue(STATE_SHA224, m_sha224->isChecked());
  settings.setValue(STATE_SHA256, m_sha256->isChecked());
  settings.setValue(STATE_SHA384, m_sha384->isChecked());
  settings.setValue(STATE_SHA512, m_sha512->isChecked());
  settings.setValue(STATE_TIGER,  m_tiger->isChecked());
  settings.endGroup();

  settings.beginGroup("Options");
  settings.setValue(OPTIONS_ONELINE,   m_oneline);
  settings.setValue(OPTIONS_SPACES,    m_spaces);
  settings.setValue(OPTIONS_UPPERCASE, m_uppercase);
  settings.endGroup();
}

//----------------------------------------------------------------
void SimpleHasher::onHashComputed(const QString& file, const Hash *hash)
{
  auto row = m_files.indexOf(file);
  auto column = m_headers.indexOf(hash->name());

  auto widget = qobject_cast<QLabel *>(m_hashTable->cellWidget(row, column));

  auto text = hash->value();
  if(m_oneline)   text = text.replace('\n', ' ');
  if(!m_spaces)   text = text.remove(' ');
  if(m_uppercase) text = text.toUpper();

  widget->setText(text);

  m_hashTable->resizeColumnToContents(column);

  QApplication::processEvents();
}

//----------------------------------------------------------------
void SimpleHasher::createContextMenu()
{
  m_menu = std::make_shared<QMenu>(centralWidget());
  auto copy = new QAction{QIcon{":/SimpleHasher/clipboard.svg"}, tr("Copy hashes to clipboard"), m_menu.get()};
  m_menu->addAction(copy);

  connect(copy, SIGNAL(triggered()), this, SLOT(copyHashesToClipboard()));

  m_hashTable->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(m_hashTable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onContextMenuActivated(const QPoint &)));
}

//----------------------------------------------------------------
void SimpleHasher::copyHashesToClipboard()
{
  auto selectedIndexes = m_hashTable->selectionModel()->selectedIndexes();

  if(!selectedIndexes.isEmpty())
  {
    QString text;
    QList<int> indexes;
    for(auto index: selectedIndexes)
    {
      if(index.column() == 0) return;

      if(index != selectedIndexes.first())
      {
        text += tr("\n");
      }

      auto label = qobject_cast<QLabel *>(m_hashTable->cellWidget(index.row(), index.column()));
      text += label->text().remove('\n').remove(' ').toLower();

      auto name  = qobject_cast<QLabel *>(m_hashTable->cellWidget(index.row(), 0));
      auto filename = name->text().remove("<b>").remove("</b>");
      text += tr(" *%1 (%2)").arg(filename).arg(m_headers.at(index.column()));
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->clear();
    clipboard->setText(text);
  }
}

//----------------------------------------------------------------
void SimpleHasher::onContextMenuActivated(const QPoint &pos)
{
  if(!m_hashTable->isEnabled()) return;

  m_menu->popup(m_hashTable->mapToGlobal(pos));
}

//----------------------------------------------------------------
void SimpleHasher::onOptionsPressed()
{
  ConfigurationDialog dialog{m_spaces, m_oneline, m_uppercase, centralWidget()};

  if(dialog.exec() == QDialog::Accepted && dialog.isModified())
  {
    m_spaces    = dialog.useSpacesChecked();
    m_oneline   = dialog.splitHashesChecked();
    m_uppercase = dialog.uppercaseChecked();

    onCheckBoxStateChanged();
  }
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
    m_hashTable->setCellWidget(row, 0, new QLabel{tr("<b>%1</b>").arg(filename)});

    for(int column = 1; column <= columnCount; ++column)
    {
      auto widget = new QLabel{"Not computed"};
      widget->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
      widget->setAlignment(Qt::AlignCenter);

      m_hashTable->setCellWidget(row, column, widget);
    }
  }

  auto enabled = (m_hashTable->rowCount() != 0);
  if(enabled)
  {
    for(int i = 0; i < m_hashTable->rowCount(); ++i)
    {
      m_hashTable->resizeColumnToContents(i);
    }
  }

  m_removeFile->setEnabled(enabled);
  m_compute->setEnabled(enabled);
}
