#include <string.h>
static char base64_table[] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G','H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 
  'a', 'b', 'c', 'd', 'e', 'f', 'g','h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '='
};
int base64_to_bin(char c) {
  for (int i = 0; i < 300; i++) {
    if (base64_table[i] == c) {
      return i;
    }
  }
  return -1;
}
int decode_base64(char *text, char* buffer) {
  int times = (strlen(text) + 4 - 1) / 4;

  char c1,c2,c3;
  for (int i = 0; i < times; i++) {
    // 6 from first char
    c1 = base64_to_bin(text[i * 4]) << 2;
    // 2 from second
    c1 |= (base64_to_bin(text[i * 4 + 1]) >> 4 & 0b011);
    // 4 from second
    c2 = base64_to_bin(text[i* 4 + 1]) << 4;
    // 4 from third
    if (text[i*4 + 2] == '=') {
      c3 = '\0';
    } else {
      c2 |= (base64_to_bin(text[i * 4 + 2]) >> 2 & 0b01111);

      // 2 from third
      c3 = base64_to_bin(text[i * 4 + 2]) << 6 & 0b11000000;
      if (text[i * 4 + 3] != '=') {
        // 6 from fourth
        c3 |= base64_to_bin(text[i * 4 + 3]);
      }
    }
    buffer[i * 3] = c1;
    buffer[i * 3 + 1] = c2;
    buffer[i * 3 + 2] = c3;
  }
  buffer[times * 3] = '\0';
  return 1;
}
int encode_base64(char *text, char* buffer) {

  int times = (strlen(text) + 3 - 1) / 3;
  // split into char triplets
  for (int i = 0; i < times; i++) {
    int index1, index2, index3;
    index1 = 3 * i;
    index2 = 3 * i + 1;
    index3 = 3 * i + 2;

    // map to 4 * 6-bit
    unsigned int c1, c2, c3, c4;
    // c1 = first 6 bits from first char
    c1 = text[index1] >> 2 & 0b00111111;
    // c2 = last 2 bits from first char, first 4 bits from second char
    c2 = text[index1] << 4;
    if (text[index2] == '\0') {
      c3 = 0b01000000;
      c4 = 0b01000000;
    } else {
      c2 |= text[index2] >> 4;
      c3 = text[index2] << 2;
      c3 &= 0b00111111;
      if (text[index3] == '\0') {
        c4 = 0b01000000;
      } else {
        c3 |= text[index3] >> 6;
        c4 = text[index3];
        c4 &= 0b00111111;
      }
    }
    c2 &= 0b00111111;

    buffer[4 * i] = base64_table[c1];
    buffer[4 * i + 1] = base64_table[c2];
    buffer[4 * i + 2] = base64_table[c3];
    buffer[4 * i + 3] = base64_table[c4];
  }
  buffer[4 * times] = '\0';
  return 1;
}
