/*
 File: SimpleHasher.h
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

#ifndef SIMPLEHASHER_H_
#define SIMPLEHASHER_H_

// Qt
#include <QMainWindow>
#include "ui_SimpleHasher.h"

class SimpleHasher
: public QMainWindow
, private Ui::SimpleHasherMainWindow
{
    Q_OBJECT
  public:
    SimpleHasher(QWidget *parent = nullptr, Qt::WindowFlags flags = 0);

    virtual ~SimpleHasher()
    {}

  private slots:
    void onAboutPressed();
    void onAddFilesPressed();
    void onRemoveFilePressed();
    void onComputePressed();
    void onCancelPressed();
    void onSavePressed();
    void onCheckBoxStateChanged(int value);

  private:
    void connectSignals();
    void hideProgress();
    void showProgress();

    void addFilesToTable(const QStringList &files);

    QStringList m_files; /** files in the table. */
};

#endif // SIMPLEHASHER_H_
