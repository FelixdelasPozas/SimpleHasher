/*
 File: CheckerDialog.h
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

#ifndef CHECKERDIALOG_H_
#define CHECKERDIALOG_H_

// Qt
#include "ui_CheckerDialog.h"

class QFile;

/** \class CheckerDialog
 * \brief Dialog to show when checking the validity of hashes in a file.
 *
 */
class CheckerDialog
: public QDialog
, private Ui::CheckerDialog
{
    Q_OBJECT
  public:
    /** \brief CheckerDialog
     * \param[in] files list of filenames of the files to check.
     * \param[in] parent pointer of the widget parent of this one.
     *
     */
    CheckerDialog(const QStringList &files, QWidget *parent = nullptr);

    /** \brief CheckerDialog class virtual destructor.
     *
     */
    virtual ~CheckerDialog()
    {}

    /** \brief Performs the checks.
     *
     */
    void check();

  private slots:
    /** \brief Cancels the hashes checks.
     *
     */
    void onCancel();

    /** \brief Shows the about dialog.
     *
     */
    void onAboutPressed();

  private:
    /** \brief Helper method to connect the signals of the UI with the slots.
     *
     */
    void connectSignals();

    /** \brief Returns the type of hashes in the file or "Unknown" otherwise.
     * \param[in] file file to check for hashes.
     *
     */
    const QString guessHash(QFile &file);

    QStringList m_files; /** list of filenames of the files to check. */
};

#endif // CHECKERDIALOG_H_
