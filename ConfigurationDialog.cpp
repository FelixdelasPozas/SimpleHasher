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

#include <ConfigurationDialog.h>

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
}

//----------------------------------------------------------------
bool ConfigurationDialog::isModified() const
{
  return ((m_useSpaces != m_spacesCheckbox->isChecked()) ||
          (m_splitHash != m_splitCheckbox->isChecked())  ||
          (m_uppercase != m_uppercaseCheckbox->isChecked()));
}
