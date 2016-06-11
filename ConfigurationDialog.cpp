/*
 File: ConfigurationDialog.cpp
 Created on: 08/06/2016
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
#include <ConfigurationDialog.h>

// Qt
#include <QDir>
#include <QApplication>
#include <QMessageBox>

const QString SEND_TO_DIRPATH = QString("%1/AppData/Roaming/Microsoft/Windows/SendTo");

//----------------------------------------------------------------
ConfigurationDialog::ConfigurationDialog(bool useSpaces, bool split, bool uppercase, QWidget *parent)
: QDialog    {parent}
, m_useSpaces{useSpaces}
, m_splitHash{split}
, m_uppercase{uppercase}
{
  setupUi(this);

  m_spacesCheckbox->setChecked(useSpaces);
  m_splitCheckbox->setChecked(split);
  m_uppercaseCheckbox->setChecked(uppercase);

  updateSentToUI();

  connect(m_sentToButton, SIGNAL(pressed()), this, SLOT(onSendToButtonPressed()));
}

//----------------------------------------------------------------
bool ConfigurationDialog::isModified() const
{
  return ((m_useSpaces != m_spacesCheckbox->isChecked()) ||
          (m_splitHash != m_splitCheckbox->isChecked())  ||
          (m_uppercase != m_uppercaseCheckbox->isChecked()));
}

//----------------------------------------------------------------
bool ConfigurationDialog::isSendToPresent() const
{
  auto sendToDir = QDir{SEND_TO_DIRPATH.arg(QDir::homePath())};

  return sendToDir.exists("SimpleHasher.lnk");
}

//----------------------------------------------------------------
void ConfigurationDialog::updateSentToUI()
{
  if(isSendToPresent())
  {
    m_sentToText->setText(tr("'Send To' shortcut is installed."));
    m_sentToButton->setText(tr("Remove"));
  }
  else
  {
    m_sentToText->setText(tr("'Send To' shortcut is not installed."));
    m_sentToButton->setText(tr("Install"));
  }
}

//----------------------------------------------------------------
void ConfigurationDialog::onSendToButtonPressed()
{
  auto sendToDir = QDir{SEND_TO_DIRPATH.arg(QDir::homePath())};
  auto name      = sendToDir.absoluteFilePath("SimpleHasher.lnk");
  auto exec      = QApplication::applicationFilePath();

  if(isSendToPresent())
  {
    if(!QFile::remove(name))
    {
      QMessageBox dialog;
      dialog.setWindowIcon(QIcon(":/SimpleHasher/application.svg"));
      dialog.setWindowTitle(tr("Error"));
      dialog.setText(tr("Can't remove 'Send To' shortcut."));
      dialog.setIcon(QMessageBox::Icon::Critical);

      dialog.exec();
    }
  }
  else
  {
    if(!QFile::link(exec, name))
    {
      QMessageBox dialog;
      dialog.setWindowIcon(QIcon(":/SimpleHasher/application.svg"));
      dialog.setWindowTitle(tr("Error"));
      dialog.setText(tr("Can't create 'Send To' shortcut."));
      dialog.setIcon(QMessageBox::Icon::Critical);

      dialog.exec();
    }
  }

  updateSentToUI();
}
