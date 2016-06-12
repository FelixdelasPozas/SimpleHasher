/*
 File: SHA384.h
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

#ifndef HASH_SHA384_H_
#define HASH_SHA384_H_

// Project
#include <Hash.h>

/** \class SHA384
 * \brief Implementation of the Secure Hash Algorithm 2 of 384 bits
 */
class SHA384
: public Hash
{
  public:
    SHA384();
    virtual ~SHA384()
    {};

    virtual void update(QFile &file);

    virtual void update(const QByteArray &buffer, const unsigned long long message_length);

    virtual const QString value() const;

    virtual const QString name() const
    { return QString("SHA-384"); }

  private:
    /** \brief Updates the hash with the char block passed as argument.
     * \param[in] char_block data buffer at maximum 128 bytes in size.
     *
     */
    virtual void process_block(const unsigned char *char_block);

    /** chaining variables. */
    unsigned long long int SHA384_A;
    unsigned long long int SHA384_B;
    unsigned long long int SHA384_C;
    unsigned long long int SHA384_D;
    unsigned long long int SHA384_E;
    unsigned long long int SHA384_F;
    unsigned long long int SHA384_G;
    unsigned long long int SHA384_H;
};

#endif // HASH_SHA384_H_
