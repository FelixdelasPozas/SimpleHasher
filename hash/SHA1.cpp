/*
 File: SHA-1.cpp
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

#include <hash/SHA1.h>

//----------------------------------------------------------------
SHA1::SHA1()
: Hash{}
, A{0x67452301}
, B{0xEFCDAB89}
, C{0x98BADCFE}
, D{0x10325476}
, E{0xC3D2E1F0}
{
  // initialize chaining variables.
}

//----------------------------------------------------------------
void SHA1::update(const QByteArray& buffer, const unsigned long long message_length)
{
  register unsigned int loop;

  if (64 == buffer.length())
  {
      process_block(reinterpret_cast<const unsigned char *>(buffer.constData()));
      return;
  }

  QByteArray finalBuffer{64,0};
  memcpy(finalBuffer.data(), buffer.constData(), buffer.length());
  finalBuffer[buffer.length()] = 0x80;

  for(auto i = buffer.length(); i < 64; ++i)
  {
    finalBuffer[i] = 0;
  }

  /* if length < 55 there is space for message length, we process 1 block */
  /* if not, we need to process two blocks                                */
  if (buffer.length() >= 56)
  {
      process_block(reinterpret_cast<const unsigned char *>(finalBuffer.constData()));
      memset(finalBuffer.data(), 0x00, 64);
  }

  for (loop = 0; loop < 8; loop++)
  {
    finalBuffer[56+loop] = ((message_length) >> (8 * loop)) & 0xFF;
  }

  process_block(reinterpret_cast<const unsigned char *>(finalBuffer.constData()));
}

//----------------------------------------------------------------
void SHA1::update(QFile file)
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

//----------------------------------------------------------------
const QString SHA1::value()
{
  return QString("%1 %2 %3 %4 %5").arg(A, 4, 16, QChar('0'))
                                  .arg(B, 4, 16, QChar('0'))
                                  .arg(C, 4, 16, QChar('0'))
                                  .arg(D, 4, 16, QChar('0'))
                                  .arg(E, 4, 16, QChar('0'));
}

//----------------------------------------------------------------
void SHA1::process_block(const unsigned char *char_block)
{
  unsigned long a, b, c, d, e, temp;
  unsigned long expanded_blk[80];
  register unsigned int loop;

  auto Function = [](unsigned int loop, unsigned long x, unsigned long y, unsigned long z)
  {
    if (loop < 20)
    {
      return ( ( z ^ ( x & ( y ^ z ) ) ) + 0x5A827999 );
    }
    else
    {
      if (loop < 40)
      {
        return ( ( x ^ y ^ z ) + 0x6ED9EBA1 );
      }
      else
      {
        if (loop < 60)
        {
          return ( ( (x & y) | (z & (x | y) ) ) + 0x8F1BBCDC);
        }
      }
    }

    return ( ( x ^ y ^ z ) + 0xCA62C1D6);
  };

  /* Rotational shift to the left */
  auto ROTL = [](unsigned long x,  int n )
  {
    return ((x << n) | (( x & 0xFFFFFFFF ) >> ( 32 - n )));
  };

  /* convert the block from unsigned char to unsigned long */
  for (loop = 0; loop < 16; loop++)
  {
    expanded_blk[loop] = ((unsigned long) (char_block[(loop*4)]   << 24)) |
                         ((unsigned long) (char_block[(loop*4)+1] << 16)) |
                         ((unsigned long) (char_block[(loop*4)+2] <<  8)) |
                         ((unsigned long) (char_block[(loop*4)+3]));
  }

  /* expanding the block from 16 to 80 */
  for (loop = 16; loop < 80; loop++)
  {
    expanded_blk[loop] = ROTL((((expanded_blk[loop-3]   ^
                                 expanded_blk[loop-8])  ^
                                 expanded_blk[loop-14]) ^
                                 expanded_blk[loop-16]), 1);
  }

  /* initialize working variables for this block */
  a = A;
  b = B;
  c = C;
  d = D;
  e = E;

  /* processing stuff */
  for (loop = 0; loop < 80; loop++)
  {
      temp = ROTL(a,5) + Function(loop,b,c,d) + e + expanded_blk[loop];
      e = d;
      d = c;
      c = ROTL(b,30);
      b = a;
      a = temp;
  }

  /* set the hash value for next block */
  A += a;
  B += b;
  C += c;
  D += d;
  E += e;
}
