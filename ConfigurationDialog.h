/*
 File: ConfigurationDialog.h
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

#ifndef CONFIGURATIONDIALOG_H_
#define CONFIGURATIONDIALOG_H_

// Project
#include "ui_ConfigurationDialog.h"

// Qt
#include <QDialog>

/** \class CondifurationDialog.
 * \brief Implements the configuration dialog.
 */
class ConfigurationDialog
: public QDialog
, private Ui::ConfigurationDialog
{
    Q_OBJECT
  public:
    /** \brief ConfigurationDialog class constructor.
     * \param[in] useSpaces true to check the 'use spaces' checkbox and false otherwise.
     * \param[in] split true to check the 'split' checkbox and false otherwise.
     * \param[in] uppercase true to check the 'uppercase' checkbox and false otherwise.
     *
     */
    ConfigurationDialog(bool useSpaces, bool split, bool uppercase, QWidget *parent = nullptr);

    /** \brief ConfigurationDialog class virtual destructor.
     *
     */
    virtual ~ConfigurationDialog()
    {}

    /** \brief Returns true if the 'use spaces' checkbox is checked.
     *
     */
    bool useSpacesChecked() const
    { return m_spacesCheckbox->isChecked(); }

    /** \brief Returns true if the 'split hashes' checkbox is checked.
     *
     */
    bool splitHashesChecked() const
    { return m_splitCheckbox->isChecked(); }

    /** \brief Returns true if the 'uppercase' checkbox is checked.
     *
     */
    bool uppercaseChecked() const
    { return m_uppercaseCheckbox->isChecked(); }

    /** \brief Returns true if the current checkboxes values are different from the initial values.
     *
     */
    bool isModified() const;

  private slots:
    /** \brief Installs or removes the 'Send To' link in the system.
     *
     */
    void onSendToButtonPressed();

  private:
    /** \brief Updates the 'Send To' UI.
     *
     */
    void updateSentToUI();

    /** \brief Returns true if the "Send To" shortcut is present.
     *
     */
    bool isSendToPresent() const;

    /** \brief Installs the "Send To" shortcut in the system.
     *
     */
    void installSendTo();

    /** \brief Removes the "Send To" shortcut from the system.
     *
     */
    void removeSendTo();

    bool m_useSpaces; /** initial value for 'use spaces' checkbox.   */
    bool m_splitHash; /** initial value for 'split hashes' checkbox. */
    bool m_uppercase; /** initial value for 'uppercase' checkbox.    */
};

#endif // CONFIGURATIONDIALOG_H_
