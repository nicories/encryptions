#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 999

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
int encrypt_caesar(char *text, int key, char* buffer) {
  int i = 0;
  while (text[i] != '\0') {
    int new = text[i] + key;
    // TODO improve this garbage
    if (new > 0x7A) {
      new = new - 0x7A + 0x60;
    } else if (new < 0x61) {
      new = 0x7A - new + 0x60;
    }
    buffer[i] = new;
    i++;
  }
  buffer[i] = '\0';
  return 1;
}
int decrypt_caesar(char *text, int key, char* buffer) { encrypt_caesar(text, -key, buffer); return 1; }

#ifdef TEST
void run_tests();
#endif
int main(int argc, char *argv[]) {
#ifdef TEST
  run_tests();
#endif
}

int test_string_equal(char *str1, char *str2) {
  if (strcmp(str1, str2) == 0) {
    return 1;
  }
  printf("%s != %s\n", str1, str2);
  return 0;
}

#ifdef TEST
void run_tests() {
  printf("running tests\n");
  char buffer[BUFFER_SIZE];

  // encode base64 tests
  // test values from https://www.cryptool.org/en/cto-codings/base64
  encode_base64("a", buffer);
  assert(test_string_equal(buffer, "YQ=="));

  encode_base64("abc", buffer);
  assert(test_string_equal(buffer, "YWJj"));

  encode_base64("KDA", buffer);
  assert(test_string_equal(buffer, "S0RB"));

  encode_base64("gaAfb", buffer);
  assert(test_string_equal(buffer, "Z2FBZmI="));

  encode_base64("ABBA", buffer);
  assert(test_string_equal(buffer, "QUJCQQ=="));

  encode_base64("sajkASjfhkAShjsdajha", buffer);
  assert(test_string_equal(buffer, "c2Fqa0FTamZoa0FTaGpzZGFqaGE="));

  // decode base64 tests
  decode_base64("YWJj", buffer);
  assert(test_string_equal(buffer, "abc"));

  decode_base64("YQ==", buffer);
  assert(test_string_equal(buffer, "a"));

  decode_base64("QUJCQQ==", buffer);
  assert(test_string_equal(buffer, "ABBA"));

  decode_base64("YTV4MQ==", buffer);
  assert(test_string_equal(buffer, "a5x1"));

  decode_base64("c2Fqa0FTamZoa0FTaGpzZGFqaGE=", buffer);
  assert(test_string_equal(buffer, "sajkASjfhkAShjsdajha"));

  // caesar encrypt tests
  encrypt_caesar("abc", 3, buffer);
  assert(test_string_equal(buffer, "def"));

  encrypt_caesar("yza", 1, buffer);
  assert(test_string_equal(buffer, "zab"));

  // caesar decrypt tests
  decrypt_caesar("def", 3, buffer);
  assert(test_string_equal(buffer, "abc"));

  decrypt_caesar("zab", 1, buffer);
  assert(test_string_equal(buffer, "yza"));

  printf("tests finished\n");
}

#endif
