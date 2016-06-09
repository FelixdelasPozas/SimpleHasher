/*
 File: CheckerDialog.cpp
 Created on: 09/06/2016
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
#include <CheckerDialog.h>
#include <AboutDialog.h>

// Qt
#include <QMessageBox>
#include <QDebug>
#include <QRegExp>
#include <QFile>

#include <iostream>

//----------------------------------------------------------------
CheckerDialog::CheckerDialog(const QStringList &files, QWidget *parent)
: QDialog{parent}
{
  setupUi(this);

  connectSignals();
}

//----------------------------------------------------------------
void CheckerDialog::check()
{
  std::cout << "enter" << std::endl << std::flush;

  for(auto filename: m_files)
  {
    std::cout << "file: " << filename.toStdString() << std::endl << std::flush;
    qDebug() << "filename" << filename;
    QFile file{filename};

    if(!file.open(QIODevice::ReadOnly))
    {
      QMessageBox dialog{this};
      dialog.setWindowIcon(QIcon(":/SimpleHasher/application.svg"));
      dialog.setWindowTitle(tr("Can't open file"));
      dialog.setText(tr("The file '%1' couldn't be opened.").arg(filename));
      dialog.setDetailedText(tr("Error: ") + file.errorString());
      dialog.setIcon(QMessageBox::Icon::Information);

      dialog.exec();
      continue;
    }

    qDebug() << guessHash(file);

    file.close();
  }
}

//----------------------------------------------------------------
void CheckerDialog::onCancel()
{
  // TODO: cancel process.

  disconnect(m_cancel_exit, SIGNAL(pressed()),
             this,          SLOT(onCancel()));

  m_cancel_exit->setIcon(QIcon(":/SimpleHasher/exit.svg"));
  m_cancel_exit->setText("Exit");

  connect(m_cancel_exit, SIGNAL(pressed()),
          this,          SLOT(close()));
}

//----------------------------------------------------------------
void CheckerDialog::onAboutPressed()
{
  AboutDialog dialog(this);
  dialog.exec();
}

//----------------------------------------------------------------
void CheckerDialog::connectSignals()
{
  connect(m_about,       SIGNAL(pressed()),
          this,          SLOT(onAboutPressed()));

  connect(m_cancel_exit, SIGNAL(pressed()),
          this,          SLOT(onCancel()));
}

//----------------------------------------------------------------
const QString CheckerDialog::guessHash(QFile& file)
{
  QString result{"Unknown"};

  file.seek(0);
  auto data = file.read(80); // a bit more than the largest of the hashes (512 bits/8 char bits = 64).

  QRegExp reg{"([a-h]*[A-H]*[0-9]*)*"};
  reg.setMinimal(true);

  if(reg.indexIn(data) != -1)
  {
    auto hash = reg.cap();

    switch(hash.length())
    {
      case 16: return "MD5";
      case 20: return "SHA-1";
      case 28: return "SHA-224";
      case 32: return "SHA-256";
      case 48: return "SHA-384";
      case 64: return "SHA-512";
      case 24: return "Tiger";
      default:
        break;
    }
  }

  return result;
}
