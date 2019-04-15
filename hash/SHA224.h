/*
 File: SHA224.h
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

#ifndef HASH_SHA224_H_
#define HASH_SHA224_H_

// Project
#include <hash/SHA1.h>

/** \class SHA224
 * \brief Implements the Secure Hash Algorithm 2 of 224 bits.
 *
 */
class SHA224
: public SHA1
{
  public:
    /** \brief SHA224 class constructor.
     *
     */
    SHA224();

    /** \brief SHA224 class virtual destructor.
     *
     */
    virtual ~SHA224()
    {};

    virtual const QString value() const override;

    virtual const QString name() const override
    { return QString("SHA-224"); }

    virtual void reset() override;

  private:
    virtual void process_block(const unsigned char *char_block) override;

    /** chaining variables */
    unsigned long SHA224_A;
    unsigned long SHA224_B;
    unsigned long SHA224_C;
    unsigned long SHA224_D;
    unsigned long SHA224_E;
    unsigned long SHA224_F;
    unsigned long SHA224_G;
    unsigned long SHA224_H;
};

#endif // HASH_SHA224_H_
