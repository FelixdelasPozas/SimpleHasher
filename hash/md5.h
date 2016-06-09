/*
 File: MD5.h
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

#ifndef HASH_MD5_H_
#define HASH_MD5_H_

// Project
#include <Hash.h>

// Qt
#include <QObject>

/** \class MD5
 * \brief Implements the Message Digest Algorithm 5 of 128 bits.
 *
 */
class MD5
: public Hash
{
  public:
    /** \brief MD5 class constructor.
     *
     */
    MD5();

    /** \brief MD5 class virtual destructor.
     *
     */
    virtual ~MD5()
    {};

    virtual void update(QFile &file);

    virtual void update(const QByteArray &buffer, const unsigned long long message_length);

    virtual const QString value() const;

    virtual const QString name() const
    { return QString("MD5"); }

  private:
    /** \brief Updates the hash with the char block passed as argument.
     * \param[in] char_block data buffer at maximum 64 bytes in size.
     *
     */
    void process_block(const unsigned char *char_block);

    unsigned long A, B, C, D; /** chaining variables. */
};

#endif // HASH_MD5_H_
