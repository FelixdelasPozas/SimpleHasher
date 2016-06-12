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

// C++
#include <memory>

// Project
#include <Hash.h>

class ComputerThread;
class QPoint;

class SimpleHasher
: public QMainWindow
, private Ui::SimpleHasherMainWindow
{
    Q_OBJECT
  public:
    SimpleHasher(const QStringList &files, QWidget *parent = nullptr, Qt::WindowFlags flags = 0);

    virtual ~SimpleHasher();

  private slots:
    void onAboutPressed();
    void onAddFilesPressed();
    void onRemoveFilePressed();
    void onComputePressed();
    void onCancelPressed();
    void onSavePressed();
    void onCheckBoxStateChanged();
    void onComputationFinished();
    void onHashComputed(const QString &file, const Hash *hash);
    void copyHashesToClipboard();
    void onContextMenuActivated(const QPoint &pos);
    void onOptionsPressed();

  private:
    enum class Mode: char { GENERATE = 0, CHECK };

    static QString STATE_MD5;
    static QString STATE_SHA1;
    static QString STATE_SHA224;
    static QString STATE_SHA256;
    static QString STATE_SHA384;
    static QString STATE_SHA512;
    static QString STATE_TIGER;
    static QString GEOMETRY;
    static QString OPTIONS_ONELINE;
    static QString OPTIONS_UPPERCASE;
    static QString OPTIONS_SPACES;
    static QString THREADS_NUMBER;

    void loadSettings();
    void saveSettings();
    void createContextMenu();
    void connectSignals();
    void hideProgress();
    void showProgress();
    void loadInformation();
    const QString guessHash(QFile &file);

    void addFilesToTable(const QStringList &files);

    Mode                                   m_mode;       /** operation mode.                                                 */
    QStringList                            m_files;      /** files in the table.                                             */
    std::shared_ptr<ComputerThread>        m_thread;     /** computer thread.                                                */
    bool                                   m_spaces;     /** true to divide the hashes with spaces.                          */
    bool                                   m_oneline;    /** true to show the long hashes in one line.                       */
    bool                                   m_uppercase;  /** true to show the hashes in uppercase.                           */
    int                                    m_threadsNum; /** number of simultaneous threads to compute hashes.               */
    QMap<QString, QMap<QString, HashSPtr>> m_results;    /** maps files -> computed hashes.                                  */
    QStringList                            m_headers;    /** list of column strings, just to avoid computing over and over.. */
    std::shared_ptr<QMenu>                 m_menu;       /** contextual menu for the table.                                  */
};

#endif // SIMPLEHASHER_H_
