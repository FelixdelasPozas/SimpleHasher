/*
 File: SHA512.cpp
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
#include <hash/SHA512.h>

/* SHA-512 Constants */
static const unsigned long long SHA512_CONSTANTS[80] =
{
  0x428A2F98D728AE22LL, 0x7137449123EF65CDLL, 0xB5C0FBCFEC4D3B2FLL, 0xE9B5DBA58189DBBCLL,
  0x3956C25BF348B538LL, 0x59F111F1B605D019LL, 0x923F82A4AF194F9BLL, 0xAB1C5ED5DA6D8118LL,
  0xD807AA98A3030242LL, 0x12835B0145706FBELL, 0x243185BE4EE4B28CLL, 0x550C7DC3D5FFB4E2LL,
  0x72BE5D74F27B896FLL, 0x80DEB1FE3B1696B1LL, 0x9BDC06A725C71235LL, 0xC19BF174CF692694LL,
  0xE49B69C19EF14AD2LL, 0xEFBE4786384F25E3LL, 0x0FC19DC68B8CD5B5LL, 0x240CA1CC77AC9C65LL,
  0x2DE92C6F592B0275LL, 0x4A7484AA6EA6E483LL, 0x5CB0A9DCBD41FBD4LL, 0x76F988DA831153B5LL,
  0x983E5152EE66DFABLL, 0xA831C66D2DB43210LL, 0xB00327C898FB213FLL, 0xBF597FC7BEEF0EE4LL,
  0xC6E00BF33DA88FC2LL, 0xD5A79147930AA725LL, 0x06CA6351E003826FLL, 0x142929670A0E6E70LL,
  0x27B70A8546D22FFCLL, 0x2E1B21385C26C926LL, 0x4D2C6DFC5AC42AEDLL, 0x53380D139D95B3DFLL,
  0x650A73548BAF63DELL, 0x766A0ABB3C77B2A8LL, 0x81C2C92E47EDAEE6LL, 0x92722C851482353BLL,
  0xA2BFE8A14CF10364LL, 0xA81A664BBC423001LL, 0xC24B8B70D0F89791LL, 0xC76C51A30654BE30LL,
  0xD192E819D6EF5218LL, 0xD69906245565A910LL, 0xF40E35855771202ALL, 0x106AA07032BBD1B8LL,
  0x19A4C116B8D2D0C8LL, 0x1E376C085141AB53LL, 0x2748774CDF8EEB99LL, 0x34B0BCB5E19B48A8LL,
  0x391C0CB3C5C95A63LL, 0x4ED8AA4AE3418ACBLL, 0x5B9CCA4F7763E373LL, 0x682E6FF3D6B2B8A3LL,
  0x748F82EE5DEFB2FCLL, 0x78A5636F43172F60LL, 0x84C87814A1F0AB72LL, 0x8CC702081A6439ECLL,
  0x90BEFFFA23631E28LL, 0xA4506CEBDE82BDE9LL, 0xBEF9A3F7B2C67915LL, 0xC67178F2E372532BLL,
  0xCA273ECEEA26619CLL, 0xD186B8C721C0C207LL, 0xEADA7DD6CDE0EB1ELL, 0xF57D4F7FEE6ED178LL,
  0x06F067AA72176FBALL, 0x0A637DC5A2C898A6LL, 0x113F9804BEF90DAELL, 0x1B710B35131C471BLL,
  0x28DB77F523047D84LL, 0x32CAAB7B40C72493LL, 0x3C9EBE0A15C9BEBCLL, 0x431D67C49C100D4CLL,
  0x4CC5D4BECB3E42B6LL, 0x597F299CFC657E2ALL, 0x5FCB6FAB3AD6FAECLL, 0x6C44198C4A475817LL
};

//----------------------------------------------------------------
SHA512::SHA512()
: Hash{}
, A{0x6A09E667F3BCC908LL}
, B{0xBB67AE8584CAA73BLL}
, C{0x3C6EF372FE94F82BLL}
, D{0xA54FF53A5F1D36F1LL}
, E{0x510E527FADE682D1LL}
, F{0x9B05688C2B3E6C1FLL}
, G{0x1F83D9ABFB41BD6BLL}
, H{0x5BE0CD19137E2179LL}
{
  // Initialize chaining variables.
}

//----------------------------------------------------------------
void SHA512::update(const QByteArray& buffer, const unsigned long long message_length)
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
const QString SHA512::value()
{
  return QString("%1 %2 %3 %4 %5 %6 %7 %8").arg(A, 8, 16, QChar('0'))
                                           .arg(B, 8, 16, QChar('0'))
                                           .arg(C, 8, 16, QChar('0'))
                                           .arg(D, 8, 16, QChar('0'))
                                           .arg(E, 8, 16, QChar('0'))
                                           .arg(F, 8, 16, QChar('0'))
                                           .arg(G, 8, 16, QChar('0'))
                                           .arg(H, 8, 16, QChar('0'));
}

//----------------------------------------------------------------
void SHA512::process_block(const unsigned char* char_block)
{
  unsigned long long a, b, c, d, e, f, g, h, temp1, temp2;
  unsigned long long expanded_blk[80];
  register unsigned int loop;

  /* Rotational shift to the right */
  auto ROTR = [](unsigned long long x, unsigned int n) { return (((x & 0xFFFFFFFFFFFFFFFFLL) >> n) | (x << (64 - n))); };

  /* Shift to the right */
  auto SHR = [](unsigned long long x, unsigned int n) { return ((x & 0xFFFFFFFFFFFFFFFFLL) >> n); };

  /* SHA-512 uses six functions */
  auto SHA512_F1 = [](unsigned long long x, unsigned long long y, unsigned long long z) { return ((x & y) | (z & (x | y))); };
  auto SHA512_F2 = [](unsigned long long x, unsigned long long y, unsigned long long z) { return (z ^ (x & (y ^ z))); };
  auto SHA512_F3 = [ROTR](unsigned long long x) { return (ROTR(x,28) ^ ROTR(x,34) ^ ROTR(x,39)); };
  auto SHA512_F4 = [ROTR](unsigned long long x) { return (ROTR(x,14) ^ ROTR(x,18) ^ ROTR(x,41)); };
  auto SHA512_F5 = [ROTR, SHR](unsigned long long x) { return (ROTR(x, 1) ^ ROTR(x, 8) ^  SHR(x, 7)); };
  auto SHA512_F6 = [ROTR, SHR](unsigned long long x) { return (ROTR(x,19) ^ ROTR(x,61) ^  SHR(x, 6)); };

  /* convert the block from unsigned char to unsigned long long */
  for (loop = 0; loop < 16; loop++)
      expanded_blk[loop] =
          (((unsigned long long) (char_block[(loop*8)]))   << 56) |
          (((unsigned long long) (char_block[(loop*8)+1])) << 48) |
          (((unsigned long long) (char_block[(loop*8)+2])) << 40) |
          (((unsigned long long) (char_block[(loop*8)+3])) << 32) |
          (((unsigned long long) (char_block[(loop*8)+4])) << 24) |
          (((unsigned long long) (char_block[(loop*8)+5])) << 16) |
          (((unsigned long long) (char_block[(loop*8)+6])) << 8)  |
          (((unsigned long long) (char_block[(loop*8)+7])));

  /* expanding the block from 16 to 80 */
  for (loop = 16; loop < 80; loop++)
      expanded_blk[loop] = SHA512_F6(expanded_blk[loop-2]) +
                           expanded_blk[loop-7] +
                           SHA512_F5(expanded_blk[loop-15]) +
                           expanded_blk[loop-16];

  /* initialize working variables for this block */
  a = A;
  b = B;
  c = C;
  d = D;
  e = E;
  f = F;
  g = G;
  h = H;


  /* processing stuff */
  for (loop = 0; loop < 80; loop++)
  {
      temp1 = h + SHA512_F4(e) + SHA512_F2(e, f, g) + SHA512_CONSTANTS[loop] + expanded_blk[loop];
      temp2 = SHA512_F3(a) + SHA512_F1(a, b, c);

      h = g;
      g = f;
      f = e;
      e = d + temp1;
      d = c;
      c = b;
      b = a;
      a = temp1 + temp2;

  }

  /* set the hash value for next block */
  A += a;
  B += b;
  C += c;
  D += d;
  E += e;
  F += f;
  G += g;
  H += h;
}
