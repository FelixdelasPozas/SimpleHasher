/*
 File: SHA256.cpp
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

#include <hash/SHA256.h>

/** SHA-256 Constants */
static const unsigned long SHA256_CONSTANTS[64] =
{
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

//----------------------------------------------------------------
SHA256::SHA256()
: SHA1{}
, SHA256_A{0x6a09e667}
, SHA256_B{0xbb67ae85}
, SHA256_C{0x3c6ef372}
, SHA256_D{0xa54ff53a}
, SHA256_E{0x510e527f}
, SHA256_F{0x9b05688c}
, SHA256_G{0x1f83d9ab}
, SHA256_H{0x5be0cd19}
{
  // initialize chaining variables.
}

//----------------------------------------------------------------
const QString SHA256::value() const
{
  return QString("%1 %2 %3 %4 %5 %6 %7 %8").arg(SHA256_A, 8, 16, QChar('0'))
                                           .arg(SHA256_B, 8, 16, QChar('0'))
                                           .arg(SHA256_C, 8, 16, QChar('0'))
                                           .arg(SHA256_D, 8, 16, QChar('0'))
                                           .arg(SHA256_E, 8, 16, QChar('0'))
                                           .arg(SHA256_F, 8, 16, QChar('0'))
                                           .arg(SHA256_G, 8, 16, QChar('0'))
                                           .arg(SHA256_H, 8, 16, QChar('0'));
}

//----------------------------------------------------------------
void SHA256::process_block(const unsigned char* char_block)
{
  unsigned long a, b, c, d, e, f, g, h, temp1, temp2;
  unsigned long expanded_blk[64];
  register unsigned int loop;

  // Rotational shift to the right
  auto ROTR = [](unsigned long x, unsigned int n) { return (((x & 0xFFFFFFFF) >> n) | (x << (32 - n))); };

  // Shift to the right
  auto SHR = [](unsigned long x, unsigned int n) { return ((x & 0xFFFFFFFF) >> n); };

  // SHA-256 uses six functions. As they do not depend on loop index we could inline them.
  auto SHA256_F1 = [](unsigned long x, unsigned long y, unsigned long z) { return ((x & y) | (z & (x | y))); };
  auto SHA256_F2 = [](unsigned long x, unsigned long y, unsigned long z) { return (z ^ (x & (y ^ z))); };
  auto SHA256_F3 = [ROTR](unsigned long x) { return (ROTR(x, 2) ^ ROTR(x,13) ^ ROTR(x,22)); };
  auto SHA256_F4 = [ROTR](unsigned long x) { return (ROTR(x, 6) ^ ROTR(x,11) ^ ROTR(x,25)); };
  auto SHA256_F5 = [ROTR, SHR](unsigned long x) { return (ROTR(x, 7) ^ ROTR(x,18) ^  SHR(x, 3)); };
  auto SHA256_F6 = [ROTR, SHR](unsigned long x) { return (ROTR(x,17) ^ ROTR(x,19) ^  SHR(x,10)); };

  // convert the block from unsigned char to unsigned long
  for (loop = 0; loop < 16; loop++)
  {
    expanded_blk[loop] = ((unsigned long) (char_block[(loop*4)]   << 24)) |
                         ((unsigned long) (char_block[(loop*4)+1] << 16)) |
                         ((unsigned long) (char_block[(loop*4)+2] <<  8)) |
                         ((unsigned long) (char_block[(loop*4)+3]));
  }

  // expanding the block from 16 to 80
  for (loop = 16; loop < 64; loop++)
  {
    expanded_blk[loop] = SHA256_F6(expanded_blk[loop-2]) +
                         expanded_blk[loop-7] +
                         SHA256_F5(expanded_blk[loop-15]) +
                         expanded_blk[loop-16];
  }

  // initialize working variables for this block
  a = SHA256_A;
  b = SHA256_B;
  c = SHA256_C;
  d = SHA256_D;
  e = SHA256_E;
  f = SHA256_F;
  g = SHA256_G;
  h = SHA256_H;


  // process block
  for (loop = 0; loop < 64; loop++)
  {
      temp1 = h + SHA256_F4(e) + SHA256_F2(e, f, g) + SHA256_CONSTANTS[loop] + expanded_blk[loop];
      temp2 = SHA256_F3(a) + SHA256_F1(a, b, c);

      h = g;
      g = f;
      f = e;
      e = d + temp1;
      d = c;
      c = b;
      b = a;
      a = temp1 + temp2;
  }

  // set the hash value for next block
  SHA256_A += a;
  SHA256_B += b;
  SHA256_C += c;
  SHA256_D += d;
  SHA256_E += e;
  SHA256_F += f;
  SHA256_G += g;
  SHA256_H += h;
}
