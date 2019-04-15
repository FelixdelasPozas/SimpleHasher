/*
 File: SHA512.h
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

#ifndef HASH_SHA512_H_
#define HASH_SHA512_H_

// Project
#include <hash/SHA384.h>

/** \class SHA512
 * \brief Implementation of the Secure Hash Algorithm 2 of 512 bits.
 *
 */
class SHA512
: public SHA384
{
  public:
    /** \brief SHA512 class constructor.
     *
     */
    SHA512();

    /** \brief SHA512 class virtual destructor.
     *
     */
    virtual ~SHA512()
    {};

    virtual const QString value() const override;

    virtual const QString name() const override
    { return QString("SHA-512"); }

    virtual void reset() override;

  private:
    virtual void process_block(const unsigned char *char_block) override;

    /** chaining variables. */
    unsigned long long SHA512_A;
    unsigned long long SHA512_B;
    unsigned long long SHA512_C;
    unsigned long long SHA512_D;
    unsigned long long SHA512_E;
    unsigned long long SHA512_F;
    unsigned long long SHA512_G;
    unsigned long long SHA512_H;
};

#endif // HASH_SHA512_H_
