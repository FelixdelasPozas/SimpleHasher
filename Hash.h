/*
 File: Hash.h
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

#ifndef HASH_H_
#define HASH_H_

// Qt
#include <QByteArray>
#include <QFile>

// C++
#include <memory>

class Hash;

using HashSPtr = std::shared_ptr<Hash>;
using HashList = QList<HashSPtr>;

/** \class Hash
 * \brief Base class that defines the basic hash algorithm methods.
 */
class Hash
{
  public:
    /** \brief Hash class constructor.
     *
     */
    Hash();

    /** \brief Hash class virtual destructor.
     *
     */
    virtual ~Hash()
    {};

    /** \brief Returns the size in bytes of the blocks that the hash processes.
     *
     */
    virtual const int blockSize() const = 0;

    /** \brief Returns the size of the unit the hash works with. I.E. 8 bof byte and 1 for bit.
     *
     */
    virtual const int bitsPerUnit() const = 0;

    /** \brief Returns the hash value as a string.
     *
     */
    virtual const QString value() const = 0;

    /** \brief Returns the Hash name.
     *
     */
    virtual const QString name() const = 0;

    /** \brief Resets the hash internal values.
     *
     */
    virtual void reset() = 0;

    /** \brief Updated the hash with the contents of the buffer.
     * \param[in] buffer data buffer.
     * \param[in] message_length length of the message being hashed.
     *
     */
    virtual void update(const QByteArray &buffer, const unsigned long long message_length) = 0;
};

#endif // HASH_H_
