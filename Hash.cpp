/*
 File: Hash.cpp
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
#include "Hash.h"

//----------------------------------------------------------------
Hash::Hash()
{
}

//----------------------------------------------------------------
void Hash::update(QFile file)
{
  unsigned long long message_length = 0;
  const unsigned long long fileSize = file.size();

  while(fileSize != message_length)
  {
    auto block = file.read(64);
    message_length += block.length();
    update(block, message_length * 8);

    emit progress((100*message_length)/fileSize);
  }
}
