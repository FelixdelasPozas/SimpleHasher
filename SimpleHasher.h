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

/** \class SimpleHasher
 * \brief Application main window class.
 *
 */
class SimpleHasher
: public QMainWindow
, private Ui::SimpleHasherMainWindow
{
    Q_OBJECT
  public:
    /** \brief SimpleHasher class constructor.
     * \param[in] files list of files passed to the application as parameters.
     * \param[in] parent pointer of the class parent of this one.
     * \param[in] flags window flags.
     *
     */
    SimpleHasher(const QStringList &files, QWidget *parent = nullptr, Qt::WindowFlags flags = 0);

    /** \brief SimpleHasher class virtual destructor.
     *
     */
    virtual ~SimpleHasher();

  private slots:
    /** \brief Shows the about dialog.
     *
     */
    void onAboutPressed();

    /** \brief Launches the add files dialog and adds the files specified by the user to the table.
     *
     */
    void onAddFilesPressed();

    /** \brief Removes the currently selected files from the table.
     *
     */
    void onRemoveFilePressed();

    /** \brief Launches the computation thread.
     *
     */
    void onComputePressed();

    /** \brief Cancels the computation thread.
     *
     *
     */
    void onCancelPressed();

    /** \brief Launches the select directory dialog and saves the computed hashes to the selected directory.
     *
     */
    void onSavePressed();

    /** \brief Updates the GUI when a hash checkbox changes state.
     *
     */
    void onCheckBoxStateChanged();

    /** \brief Updates the UI when the computation thread finishes.
     *
     */
    void onComputationFinished();

    /** \brief Updates the UI with the value of the computed hash.
     * \param[in] filename filename of the computed hash.
     * \param[in] hash hash object pointer.
     *
     */
    void onHashComputed(const QString &filename, const Hash *hash);

    /** \brief Copies the values of the selected hashes to the clipboard.
     *
     */
    void copyHashesToClipboard();

    /** \brief Shows the context menu if activated over a hash cell.
     *
     */
    void onContextMenuActivated(const QPoint &pos);

    /** \brief Shows the configuration dialog.
     *
     */
    void onOptionsPressed();

  private:
    /** \class Mode
     * \brief Enumeration of application operation mode.
     *
     */
    enum class Mode: char { GENERATE = 0, CHECK };

    /** Settings strings. */
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

    /** \brief Helper method to load the application settings from the ini file.
     *
     */
    void loadSettings();

    /** \brief Helper method to save the application settings to the ini file.
     *
     */
    void saveSettings();

    /** \brief Helper method to create the contextual menu on application initialization.
     *
     */
    void createContextMenu();

    /** \brief Helper method to connect GUI signals on application initialization.
     *
     */
    void connectSignals();

    /** \brief Hides the progress bar and the cancel button. Disables the hash group.
     *
     */
    void hideProgress();

    /** \brief Shows the progress bar and the cancel button. Enables the hash group.
     *
     */
    void showProgress();

    /** \brief Helper method to load the information contained in the files passed as arguments to the application.
     *
     */
    void loadInformation();

    /** \brief Returns a string that describes the type of hash detected in the file passed as argument.
     * \param[in] file opened file object.
     *
     */
    const QString guessHash(QFile &file);

    /** \brief Helper method to add a list of files to the table.
     *
     */
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
