/*
 File: SHA-1.h
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

#ifndef HASH_SHA1_H_
#define HASH_SHA1_H_

// Project
#include <Hash.h>

/** \class SHA
 * \brief Implements the Secure Hash Algorithm 1
 *
 */
class SHA1
: public Hash
{
  public:
    /** \brief SHA class constructor.
     *
     */
    SHA1();

    /** \brief SHA class virtual destructor.
     *
     */
    virtual ~SHA1()
    {};

    virtual void update(QFile &file);

    virtual void update(const QByteArray &buffer, const unsigned long long message_length);

    virtual const QString value();

    virtual const QString name()
    { return QString("SHA-1"); }

    virtual HashSPtr clone() const;

  private:
    /** \brief Updates the hash with the char block passed as argument.
     * \param[in] char_block data buffer at maximum 64 bytes in size.
     *
     */
    virtual void process_block(const unsigned char *char_block);

    /** chaining variables. */
    unsigned long SHA1_A;
    unsigned long SHA1_B;
    unsigned long SHA1_C;
    unsigned long SHA1_D;
    unsigned long SHA1_E;
};

#endif // HASH_SHA1_H_
