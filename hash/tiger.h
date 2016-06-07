/*
 File: Tiger.h
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

#ifndef HASH_TIGER_H_
#define HASH_TIGER_H_

#include <Hash.h>

/** \class Tiger
 * \brief Implementation of the Tiger hash algorithm of 128 bits.
 *
 */
class Tiger
: public Hash
{
  public:
    /** \brief Tiger class constructor.
     *
     */
    Tiger(unsigned long long *table = nullptr);

    /** \brief Tiger class virtual destructor.
     *
     */
    virtual ~Tiger()
    {};

    virtual void update(QFile &file);

    virtual void update(const QByteArray &buffer, const unsigned long long message_length);

    virtual const QString value();

    virtual const QString name()
    { return QString("Tiger"); }

    virtual HashSPtr clone() const;

  private:
    /** \brief Updates the hash with the char block passed as argument.
     * \param[in] char_block data buffer at maximum 64 bytes in size.
     *
     */
    void process_block(const unsigned char *char_block);

    /** \brief Helper method to generate the Tiger table.
     * \param[in] message seed.
     * \param[in] passes_number number of passes.
     *
     */
    void generate_table(const unsigned char *message, const unsigned int passes_number);

    static QString RANDOM_VALUE;  /** seed for table generation.                       */
    static int PASSES_NUMBER;     /** number of passes for table generation.           */
    static int BLOCK_PASSES;      /** number of passes when processing the data block. */

    typedef struct
    {
        unsigned long long a;
        unsigned long long b;
        unsigned long long c;
    }
    TIGER_HASH;

    /** chaining variables. */
    TIGER_HASH hash;
};

#endif // HASH_TIGER_H_
