
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
