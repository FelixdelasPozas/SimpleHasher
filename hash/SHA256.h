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

#include <hash/SHA1.h>

/** \class SHA256
 * \brief Implements the Secure Hash Algorithm 2 of 256 bits.
 *
 */
class SHA256
: public SHA1
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

    virtual const QString value() const override;

    virtual const QString name() const override
    { return QString("SHA-256"); }

  private:
    /** \brief Updates the hash with the char block passed as argument.
     * \param[in] char_block data buffer at maximum 64 bytes in size.
     *
     */
    virtual void process_block(const unsigned char *char_block) override;

    /** chaining variables. */
    unsigned long SHA256_A;
    unsigned long SHA256_B;
    unsigned long SHA256_C;
    unsigned long SHA256_D;
    unsigned long SHA256_E;
    unsigned long SHA256_F;
    unsigned long SHA256_G;
    unsigned long SHA256_H;
};

#endif // HASH_SHA256_H_
