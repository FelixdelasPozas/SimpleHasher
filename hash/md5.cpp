/*
 File: MD5.cpp
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

// Project
#include <hash/MD5.h>

// C++
#include <cstring>

// Qt
#include <QString>

/* MD5 table : additive constants, abs(sin(j+1)) 0 =< j =< 63 */
static const unsigned long MD5_CONSTANTS[64] =
{
  0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

/* MD5 table : access order for words */
static const unsigned int MD5_ACCESS_ORDER[64] =
{
  0, 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
  1, 6, 11,  0,  5, 10, 15,  4,  9, 14,  3,  8, 13,  2,  7, 12,
  5, 8, 11, 14,  1,  4,  7, 10, 13,  0,  3,  6,  9, 12, 15,  2,
  0, 7, 14,  5, 12,  3, 10,  1,  8, 15,  6, 13,  4, 11,  2,  9
};

/* MD5 table : bit positions for left shifts */
static const unsigned int MD5_SHIFTS[64] =
{
  7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
  5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
  4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
  6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

//----------------------------------------------------------------
MD5::MD5()
: Hash{}
{
  // initialize chaining variables.
  reset();
}

//----------------------------------------------------------------
const int MD5::blockSize() const
{
  return 64;
}

//----------------------------------------------------------------
const int MD5::bitsPerUnit() const
{
  return 8;
}

//----------------------------------------------------------------
void MD5::update(const QByteArray &buffer, const unsigned long long message_length)
{
  auto length = buffer.length();

  if (64 == length)
  {
    process_block(reinterpret_cast<const unsigned char *>(buffer.constData()));
    return;
  }

  QByteArray finalBuffer{64,0};
  std::memcpy(finalBuffer.data(), buffer.constData(), length);
  finalBuffer[length++] = 0x80;

  /* if length < 55 there is space for message length, we process 1 block */
  /* if not, we need to process two blocks                                */
  if (length >= 56)
  {
      process_block(reinterpret_cast<const unsigned char *>(finalBuffer.constData()));
      std::memset(finalBuffer.data(), 0x00, 64);
  }

  for (int loop = 0; loop < 8; loop++)
  {
    finalBuffer[56+loop] = static_cast<unsigned char>(((message_length) >> (8 * loop)) & 0xFF);
  }

  process_block(reinterpret_cast<const unsigned char *>(finalBuffer.constData()));

  /* for reversing the byte order, MD5 is little-endian */
  auto reverse = [] (unsigned long value)
  {
    unsigned char a,b,c,d;

    a = (unsigned char)((value >> 24) & 0xFF);
    b = (unsigned char)((value >> 16) & 0xFF);
    c = (unsigned char)((value >>  8) & 0xFF);
    d = (unsigned char)(value & 0xFF);

    value = (unsigned long)(d << 24);
    value |= (unsigned long)(c << 16);
    value |= (unsigned long)(b << 8);
    value |= (unsigned long)a;

    return value;
  };

  A = reverse(A);
  B = reverse(B);
  C = reverse(C);
  D = reverse(D);
}

//----------------------------------------------------------------
const QString MD5::value() const
{
  return QString("%1 %2 %3 %4").arg(A, 8, 16, QChar('0'))
                               .arg(B, 8, 16, QChar('0'))
                               .arg(C, 8, 16, QChar('0'))
                               .arg(D, 8, 16, QChar('0'));
}

//----------------------------------------------------------------
void MD5::process_block(const unsigned char *char_block)
{
  unsigned long a, b, c, d,temp;
  unsigned long long_block[16];
  unsigned int loop;

  auto Function = [](unsigned int loop, unsigned long x, unsigned long y, unsigned long z)
  {
    if (loop < 16)
    {
    return (z ^ (x & (y ^ z)));
    }
    else
    {
      if (loop < 32)
      {
        return (y ^ (z & (x ^ y)));
      }
      else
      {
        if (loop < 48)
        {
          return (x ^ y ^ z);
        }
      }
    }

    return (y ^ (x | ~z));
  };

  /* Rotational shift to the left */
  auto ROTL = [](unsigned long x,  int n )
  {
    return ((x << n) | (( x & 0xFFFFFFFF ) >> ( 32 - n )));
  };

  /* convert the block from unsigned char to unsigned long */
  /* MD5 is little-endian so we have to reverse bytes      */
  for (loop = 0; loop < 16; loop++)
  {
    long_block[loop] = ((unsigned long) (char_block[(loop*4)]   <<  0)) |
                       ((unsigned long) (char_block[(loop*4)+1] <<  8)) |
                       ((unsigned long) (char_block[(loop*4)+2] << 16)) |
                       ((unsigned long) (char_block[(loop*4)+3] << 24));
  }

  /* initialize working variables for this block */
  a = A;
  b = B;
  c = C;
  d = D;

  /* processing stuff, constants and block access order can be inserted into function() */
  for (loop = 0; loop < 64; loop++)
  {
      temp = a + Function(loop,b,c,d) + long_block[MD5_ACCESS_ORDER[loop]] + MD5_CONSTANTS[loop];
      temp = ROTL(temp, MD5_SHIFTS[loop]) + b;

      a = d;
      d = c;
      c = b;
      b = temp;
  }

  /* set the hash value for next block */
  A += a;
  B += b;
  C += c;
  D += d;
}

//----------------------------------------------------------------
void MD5::reset()
{
  A = 0x67452301;
  B = 0xefcdab89;
  C = 0x98badcfe;
  D = 0x10325476;
}
