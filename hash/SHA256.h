/*
 File: SHA256.h
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

#ifndef HASH_SHA256_H_
#define HASH_SHA256_H_

#include <Hash.h>

/** \class SHA256
 * \brief Implements the Secure Hash Algorithm 2 of 256 bits.
 *
 */
class SHA256
: public Hash
{
  public:
    /** \brief SHA256 class constructor.
     *
     */
    SHA256();

    /** \brief SHA256 class virtual destructor.
     *
     */
    virtual ~SHA256()
    {};

    virtual void update(const QByteArray &buffer, const unsigned long long message_length);

    virtual const QString value();

    virtual const QString name()
    { return QString("SHA-256"); }

    virtual HashSPtr clone() const;

  private:
    /** \brief Updates the hash with the char block passed as argument.
     * \param[in] char_block data buffer at maximum 64 bytes in size.
     *
     */
    void process_block(const unsigned char *char_block);

    unsigned long A, B, C, D, E, F, G, H; /** chaining variables. */
};

#endif // HASH_SHA256_H_
