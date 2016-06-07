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

// 512 bits (64 chars) "random value" for the generation of the s-boxes.
QString Tiger::RANDOM_VALUE = QString("Tiger - A Fast New Hash Function, by Ross Anderson and Eli Biham");

// Default parameters used.
int Tiger::PASSES_NUMBER = 5;
int Tiger::BLOCK_PASSES = 3;
bool Tiger::TABLE_AVAILABLE = false;

static unsigned long long tiger_table[1024]; /** tiger table values. */

//----------------------------------------------------------------
Tiger::Tiger()
: Hash{}
{
  // initialize chaining variables
  hash.a = 0x0123456789abcdefULL;
  hash.b = 0xfedcba9876543210ULL;
  hash.c = 0xf096a5b4c3b2e187ULL;

  if(!TABLE_AVAILABLE)
  {
    // Generate table.
    generate_table(reinterpret_cast<const unsigned char *>(RANDOM_VALUE.toStdString().c_str()), PASSES_NUMBER);

    // Reset the values after the table generation.
    hash.a = 0x0123456789abcdefULL;
    hash.b = 0xfedcba9876543210ULL;
    hash.c = 0xf096a5b4c3b2e187ULL;

    TABLE_AVAILABLE = true;
  }
}

//----------------------------------------------------------------
void Tiger::update(QFile &file)
{
  unsigned long long message_length = 0;
  const unsigned long long fileSize = file.size();
  int currentProgress = -1;
  int progressValue = 0;

  while(fileSize != message_length)
  {
    auto block = file.read(64);
    message_length += block.length();
    update(block, message_length);

    // last block needs to be processed
    if((fileSize == message_length) && (block.size() == 64))
    {
      update(QByteArray(), message_length);
    }

    progressValue = (100*message_length)/fileSize;

    if(currentProgress != progressValue)
    {
      emit progress(progressValue);
      currentProgress = progressValue;
    }
  }
}

//----------------------------------------------------------------
void Tiger::update(const QByteArray& buffer, const unsigned long long message_length)
{
  auto length = buffer.length();

  if (64 == length)
  {
    process_block(reinterpret_cast<const unsigned char *>(buffer.constData()));
    return;
  }

  QByteArray finalBuffer{64,0};

  // copy the remaining bytes of the message to a temporal block
  memcpy(finalBuffer.data(), buffer.constData(), length);

  // padding the message
  finalBuffer[length++] = 0x01;

  // is this block too big? if so fill zeroes, process and make another
  if(length > 56)
  {
    process_block(reinterpret_cast<const unsigned char *>(finalBuffer.constData()));
    memset(finalBuffer.data(), 0x00, 64);
  }

  // insert message length at the end of block
  ((unsigned long long*)(&(finalBuffer.data()[56])))[0] = ((unsigned long long)message_length)<<3;

  process_block(reinterpret_cast<const unsigned char *>(finalBuffer.constData()));
}

//----------------------------------------------------------------
const QString Tiger::value() const
{
  return QString("%1 %2 %3").arg(hash.a, 16, 16, QChar('0'))
                            .arg(hash.b, 16, 16, QChar('0'))
                            .arg(hash.c, 16, 16, QChar('0'));
}

//----------------------------------------------------------------
void Tiger::process_block(const unsigned char* char_block)
{
  unsigned long long a, b, c, temp;
  unsigned long long a_temp, b_temp, c_temp;
  unsigned long long block[8];
  int pass_number, loop, mult;

  // init state, block 64 bit long longs, and save state values
  a_temp = a = hash.a;
  b_temp = b = hash.b;
  c_temp = c = hash.c;

  for(loop = 0; loop < 8; loop++)
  {
    block[loop] = reinterpret_cast<const unsigned long long *>(char_block)[loop];
  }

  // processing, PASSES is 3 or more
  for(pass_number = 0; pass_number < BLOCK_PASSES; pass_number++)
  {
    // if it's not our first pass, key schedule
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

    // the real work
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

  // feed forward and return
  a ^= a_temp;
  b -= b_temp;
  c += c_temp;

  hash.a = a;
  hash.b = b;
  hash.c = c;
}

//----------------------------------------------------------------
void Tiger::generate_table(const unsigned char *message, const unsigned int passes_number)
{
  unsigned int loop, passes, sb, column, abc;
  unsigned char temp_char;
  unsigned char *table_chars, *state_chars;

  // a char is used as index for filling starting values to the table
  table_chars = reinterpret_cast<unsigned char *>(tiger_table);
  state_chars = reinterpret_cast<unsigned char *>(&hash);

  // fill the table
  for (loop = 0; loop < 1024; loop++)
  {
    for (column = 0; column < 8; column++)
    {
      table_chars[((8 * loop) + column)] = static_cast<unsigned char>(loop & 0xFF);
    }
  }

  abc = 2;

  // generation
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
