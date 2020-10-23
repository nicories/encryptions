#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "base64.c"
#include "caesar.c"
#define BUFFER_SIZE 999

int test_string_equal(char *str1, char *str2) {
  if (strcmp(str1, str2) == 0) {
    return 1;
  }
  printf("%s != %s\n", str1, str2);
  return 0;
}
int main() {
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

  return 0;
}
