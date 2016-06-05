/*
 File: Tiger.cpp
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
#include <hash/Tiger.h>

/* 512 bits (64 chars) "random value" for the generation of the s-boxes */
/* originally defined as 16 32-bits words                               */
QString Tiger::RANDOM_VALUE = QString("Tiger - A Fast New Hash Function, by Ross Anderson and Eli Biham");
int Tiger::PASSES_NUMBER = 5;
int Tiger::BLOCK_PASSES = 3;

//----------------------------------------------------------------
Tiger::Tiger()
: Hash{}
, A{0x0123456789abcdefULL}
, B{0xfedcba9876543210ULL}
, C{0xf096a5b4c3b2e187ULL}
{
  // Initialize chaining values.

  // Generate table.
  generate_table(reinterpret_cast<const unsigned char *>(RANDOM_VALUE.toStdString().c_str()), PASSES_NUMBER);

  // Reset the values after the table generation.
  A = 0x0123456789abcdefULL;
  B = 0xfedcba9876543210ULL;
  C = 0xf096a5b4c3b2e187ULL;
}

//----------------------------------------------------------------
void Tiger::update(const QByteArray& buffer, const unsigned long long message_length)
{
  if (64 == buffer.length())
  {
    process_block(reinterpret_cast<const unsigned char *>(buffer.constData()));
    return;
  }

  /* buffer length < 64, padding needed */
  QByteArray finalBuffer{64,0};

  /* copy the remaining bytes of the message to a temporal block */
  memcpy(finalBuffer.data(), buffer.constData(), buffer.length());

  /* padding the message */
  finalBuffer[buffer.length()] = 0x01;

  /* is this block too big? if so fill zeroes, process and make another */
  if(buffer.length() > 56)
  {
    process_block(reinterpret_cast<const unsigned char *>(finalBuffer.constData()));
    memset(finalBuffer.data(), 0x00, 64);
  }

  /* insert message length */
  ((unsigned long long*)(&(finalBuffer.data()[56])))[0] = ((unsigned long long)message_length)<<3;

  process_block(reinterpret_cast<const unsigned char *>(finalBuffer.constData()));
}

//----------------------------------------------------------------
const QString Tiger::value()
{
  unsigned long a1 = ((A >> 32) & 0x00000000FFFFFFFF);
  unsigned long a2 = (A & 0x00000000FFFFFFFF);
  unsigned long b1 = ((B >> 32) & 0x00000000FFFFFFFF);
  unsigned long b2 = (B & 0x00000000FFFFFFFF);
  unsigned long c1 = ((C >> 32) & 0x00000000FFFFFFFF);
  unsigned long c2 = (C & 0x00000000FFFFFFFF);

  return QString("%1 %2 %3 %4 %5 %6").arg(a1, 4, 16, QChar('0'))
                                     .arg(a2, 4, 16, QChar('0'))
                                     .arg(b1, 4, 16, QChar('0'))
                                     .arg(b2, 4, 16, QChar('0'))
                                     .arg(c1, 4, 16, QChar('0'))
                                     .arg(c2, 4, 16, QChar('0'));
}

//----------------------------------------------------------------
void Tiger::process_block(const unsigned char* char_block)
{
  unsigned long long a, b, c, temp;
  unsigned long long a_temp, b_temp, c_temp;
  unsigned long long block[8];
  int pass_number, loop, mult;

  /* init state, block 64 bit long longs, and save state values */
  a_temp = a = A;
  b_temp = b = B;
  c_temp = c = C;

  for(loop = 0; loop < 8; loop++)
  {
    block[loop] = reinterpret_cast<const unsigned long long *>(char_block)[loop];
  }

  /* processing, PASSES is 3 or more */
  for(pass_number = 0; pass_number < BLOCK_PASSES; pass_number++)
  {
    /* if it's not our first pass, key schedule */
    if(pass_number != 0)
    {
      block[0] -= block[7] ^ 0xA5A5A5A5A5A5A5A5LL;
      block[1] ^= block[0];
      block[2] += block[1];
      block[3] -= block[2] ^ ((~block[1]) << 19);
      block[4] ^= block[3];
      block[5] += block[4];
      block[6] -= block[5] ^ ((~block[4]) >> 23);
      block[7] ^= block[6];
      block[0] += block[7];
      block[1] -= block[0] ^ ((~block[7]) << 19);
      block[2] ^= block[1];
      block[3] += block[2];
      block[4] -= block[3] ^ ((~block[2]) >> 23);
      block[5] ^= block[4];
      block[6] += block[5];
      block[7] -= block[6] ^ 0x0123456789ABCDEFLL;
    }

    /* first pass mult = 5, second = 7, third or more = 9 */
    switch (pass_number)
    {
      case 0:
        mult = 5;
        break;
      case 1:
        mult = 7;
        break;
      default :
        mult = 9;
    }

    /* the real work */
    for (loop = 0; loop < 8; loop++)
    {
      c ^= block[loop];
      a -= tiger_table[  0 + ((unsigned char)(c))] ^
           tiger_table[256 + ((unsigned char)(((unsigned long)(c)) >> (2 * 8)))] ^
           tiger_table[512 + ((unsigned char)((c) >> (4 * 8)))] ^
           tiger_table[768 + ((unsigned char)(((unsigned long)((c) >> (4 * 8))) >> (2 * 8)))] ;
      b += tiger_table[768 + ((unsigned char)(((unsigned long)(c)) >> (1 * 8)))] ^
           tiger_table[512 + ((unsigned char)(((unsigned long)(c)) >> (3 * 8)))] ^
           tiger_table[256 + ((unsigned char)(((unsigned long)((c) >> (4 * 8))) >> (1 * 8)))] ^
           tiger_table[  0 + ((unsigned char)(((unsigned long)((c) >> (4 * 8))) >> (3 * 8)))];
      b *= mult;

      temp = a;
      a = b;
      b = c;
      c = temp;
    }
  }

  /* feed forward and return */
  a ^= a_temp;
  b -= b_temp;
  c += c_temp;

  A = a;
  B = b;
  C = c;
}

//----------------------------------------------------------------
void Tiger::generate_table(const unsigned char *message, const unsigned int passes_number)
{
  unsigned int loop, passes, sb, column, abc;
  unsigned char temp_char;
  unsigned char *table_chars, *state_chars;

  /* a char is used as index for filling starting values to the table */
  table_chars = (unsigned char *) tiger_table;

  struct
  {
    unsigned long long a;
    unsigned long long b;
    unsigned long long c;
  } hash;

  hash.a = A;
  hash.b = B;
  hash.c = C;

  state_chars = (unsigned char *) &hash;

  /* fill the table */
  for (loop = 0; loop < 1024; loop++)
  {
    for (column = 0; column < 8; column++)
    {
      table_chars[((8 * loop) + column)] = (unsigned char) (loop & 0xFF);
    }
  }

  abc = 2;

  /* generation */
  for (passes = 0; passes < passes_number; passes++)
  {
    for (loop = 0; loop < 256; loop++)
    {
      for (sb = 0; sb < 1024; sb += 256)
      {
        ++abc;
        if (abc == 3)
        {
          abc = 0;
          process_block(message);
        }
        for (column = 0; column < 8; column++)
        {
          temp_char = table_chars[(8 * (sb + loop)) + column];
          table_chars[(8 * (sb + loop)) + column] = table_chars[(8 * (sb + state_chars[(8 * (abc)) + column])) + column];
          table_chars[(8 * (sb + state_chars[(8 * (abc)) + column])) + column] = temp_char;
        }
      }
    }
  }
}
