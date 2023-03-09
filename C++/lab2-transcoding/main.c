#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define UTF8_WITHOUT_BOM 0
#define UTF8_WITH_BOM 1
#define UTF16LE 2
#define UTF16BE 3
#define UTF32LE 4
#define UTF32BE 5

#define MODE_LE 0
#define MODE_BE 1

int getEncoding(FILE *inputFile);
void writeBOM(FILE *outputFile, int encode);
void copyRemainingBytes(FILE *inputFile, FILE *outputFile);

void swap(int *first, int *second) {
  int save = *first;
  *first = *second;
  *second = save;
}

int getSizeBOM(const int encode) {
  switch (encode) {
    default:
    case UTF8_WITHOUT_BOM:
      return 0;
    case UTF8_WITH_BOM:
      return 3;
    case UTF16LE:
    case UTF16BE:
      return 2;
    case UTF32LE:
    case UTF32BE:
      return 4;
  }
}

int getEncoding(FILE *inputFile) {
  unsigned char buffer[4] = {
          fgetc(inputFile),
          fgetc(inputFile),
          fgetc(inputFile),
          fgetc(inputFile)
  };
  int result;
  if (buffer[0] == 0xFF && buffer[1] == 0xFE && buffer[2] == 0x00 && buffer[3] == 0x00) {
    result = UTF32LE;
  } else if (buffer[0] == 0x00 && buffer[1] == 0x00 && buffer[2] == 0xFE && buffer[3] == 0xFF) {
    result = UTF32BE;
  } else if (buffer[0] == 0xFF && buffer[1] == 0xFE) {
    result = UTF16LE;
  } else if (buffer[0] == 0xFE && buffer[1] == 0xFF) {
    result = UTF16BE;
  } else if (buffer[0] == 0xEF && buffer[1] == 0xBB && buffer[2] == 0xBF) {
    result = UTF8_WITH_BOM;
  } else {
    result = UTF8_WITHOUT_BOM;
  }
  fseek(inputFile, getSizeBOM(result), SEEK_SET);
  return result;
}

void writeBOM(FILE *outputFile, int encode) {
  switch (encode) {
    case UTF32LE:
      fputc(0x00, outputFile);
      fputc(0x00, outputFile);
      fputc(0xFE, outputFile);
      fputc(0xFF, outputFile);
      break;
    case UTF32BE:
      fputc(0xFF, outputFile);
      fputc(0xFE, outputFile);
      fputc(0x00, outputFile);
      fputc(0x00, outputFile);
      break;
    case UTF16LE:
      fputc(0xFF, outputFile);
      fputc(0xFE, outputFile);
      break;
    case UTF16BE:
      fputc(0xFE, outputFile);
      fputc(0xFF, outputFile);
      break;
    case UTF8_WITH_BOM:
      fputc(0xEF, outputFile);
      fputc(0xBB, outputFile);
      fputc(0xBF, outputFile);
      break;
    case UTF8_WITHOUT_BOM:
    default:
      break;
  }
}

unsigned int readUTF8(FILE *inputFile, int *errorCode) {
  int array[4];
  unsigned int result = 0;
  *errorCode = 1;
  array[0] = fgetc(inputFile);
  if (array[0] == -1) {
    return result;
  }
  if ((array[0] & 0x80) == 0) { // один байт
    result = array[0];
  } else if ((array[0] & 0xE0) == 0xC0) { // два байта
    array[1] = fgetc(inputFile);
    result = ((array[0] & (0x1F)) << 6) |
             (array[1] & (0x3F));
  } else if ((array[0] & 0xF0) == 0xE0) { // три байта
    array[1] = fgetc(inputFile);
    array[2] = fgetc(inputFile);
    result = ((array[0] & (0x1F)) << 12) |
             ((array[1] & (0x3F)) << 6) |
             (array[2] & (0x3F));
  } else if ((array[0] & 0xF8) == 0xF0) { // четыре байта
    array[1] = fgetc(inputFile);
    array[2] = fgetc(inputFile);
    array[3] = fgetc(inputFile);
    result = ((array[0] & (0x1F)) << 18) |
             ((array[1] & (0x3F)) << 12) |
             ((array[2] & (0x3F)) << 6) |
             (array[2] & (0x3F));
  }
  *errorCode = 0;
  return result;
}

void writeUTF8(FILE *f, const unsigned int symbol) {
  union {
      unsigned int s;
      unsigned char b[4];
  } result;
  result.s = 0;
  if (symbol <= 0x7F) { // 1 байт
    fputc(symbol, f);
  } else if (symbol <= 0x7FF) { // 2 байта
    result.b[0] = (symbol >> 6) | 0xC0;
    result.b[1] = (symbol & 0x3f) | 0x80;
    fputc(result.b[0], f);
    fputc(result.b[1], f);
  } else if (symbol <= 0xFFFF) { // 3 байта
    result.b[0] = (symbol >> 12) | 0xE0;
    result.b[1] = ((symbol >> 6) & 0x3F) | 0x80;
    result.b[2] = (symbol & 0x3F) | 0x80;
    fputc(result.b[0], f);
    fputc(result.b[1], f);
    fputc(result.b[2], f);
  } else if (symbol <= 0x1FFFFF) { // 4 байта
    result.b[0] = (symbol >> 18) | 0xF0;
    result.b[1] = ((symbol >> 12) & 0x3F) | 0x80;
    result.b[2] = ((symbol >> 6) & 0x3F) | 0x80;
    result.b[3] = (symbol & 0x3F) | 0x80;
    fputc(result.b[0], f);
    fputc(result.b[1], f);
    fputc(result.b[2], f);
    fputc(result.b[3], f);
  }
}

unsigned int read2Bytes(FILE *inputFile, const int mode, int *errorCode) {
  int array[2] = {
          fgetc(inputFile),
          fgetc(inputFile)
  };
  unsigned int result = 0;
  if (array[1] == -1) {
    *errorCode = 1;
    return result;
  }
  if (mode == MODE_LE) {
    swap(&array[0], &array[1]);
  }
  result = (array[0] << 8) | (array[1]);
  *errorCode = 0;
  return result;
}

unsigned int read4Bytes(FILE *inputFile, const int mode, int *errorCode) {
  int array[4] = {
          fgetc(inputFile),
          fgetc(inputFile),
          fgetc(inputFile),
          fgetc(inputFile)
  };
  unsigned int result = 0;
  if (array[3] == -1) {
    *errorCode = 1;
    return result;
  }
  if (mode == MODE_LE) {
    swap(&array[0], &array[3]);
    swap(&array[1], &array[2]);
  }
  result = (array[0] << 24) | (array[1] << 16) | (array[2] << 8) | (array[3]);
  *errorCode = 0;
  return result;
}

void write2Bytes(FILE *outputFile, const int bytes, const int mode) {
  if (mode == MODE_LE) {
    fputc(bytes & 0xFF, outputFile);
    fputc(bytes >> 8, outputFile);
  } else {
    fputc(bytes >> 8, outputFile);
    fputc(bytes & 0xFF, outputFile);
  }
}

void write4Bytes(FILE *outputFile, const int bytes, const int mode) {
  if (mode == MODE_LE) {
    fputc(bytes & 0xFF, outputFile);
    fputc((bytes >> 8) & 0xFF, outputFile);
    fputc((bytes >> 16) & 0xFF, outputFile);
    fputc((bytes >> 24) & 0xFF, outputFile);
  } else {
    fputc((bytes >> 24) & 0xFF, outputFile);
    fputc((bytes >> 16) & 0xFF, outputFile);
    fputc((bytes >> 8) & 0xFF, outputFile);
    fputc(bytes & 0xFF, outputFile);
  }
}

unsigned int readUTF16(FILE *inputFile, const int mode, int *errorCode) {
  unsigned int result = 0;
  *errorCode = -1;
  unsigned int leading = read2Bytes(inputFile, mode, errorCode);
  if (*errorCode != 0) {
    return result;
  }
  if (leading < 0xD800 || leading > 0xDFFF) {
    result = leading;
  } else if (leading >= 0xDC00) {
    *errorCode = 1;
    return result;
  } else {
    result = (leading & 0x3FF) << 10;
    unsigned int tailing = read2Bytes(inputFile, mode, errorCode);
    if (*errorCode != 0) {
      return result;
    }
    if (tailing < 0xDC00 || tailing > 0xDFFF) {
      *errorCode = 1;
      return result;
    } else {
      result = result | (tailing & 0x3FF);
      result += 0x10000;
    }
  }
  *errorCode = 0;
  return result;
}

unsigned int readUTF16BE(FILE *inputFile, int *errorCode) {
  return readUTF16(inputFile, MODE_BE, errorCode);
}

unsigned int readUTF16LE(FILE *inputFile, int *errorCode) {
  return readUTF16(inputFile, MODE_LE, errorCode);
}

void writeUTF16(FILE *outputFile, const unsigned int symbol, const int mode) {
  if (symbol < 0x10000) {
    write2Bytes(outputFile, (int) symbol & 0xFFFF, mode);
  } else {
    int t = (int) symbol - 0x10000;
    int low = (t & 0x3FF) & 0xFFFF;
    int high = (t >> 10) & 0xFFFF;
    write2Bytes(outputFile, 0xD800 | high, mode);
    write2Bytes(outputFile, 0xDC00 | low, mode);
  }
}

void writeUTF16LE(FILE *outputFile, const unsigned int symbol) {
  writeUTF16(outputFile, symbol, MODE_LE);
}

void writeUTF16BE(FILE *outputFile, const unsigned int symbol) {
  writeUTF16(outputFile, symbol, MODE_BE);
}

unsigned int readUTF32LE(FILE *inputFile, int *errorCode) {
  return read4Bytes(inputFile, MODE_LE, errorCode);
}

unsigned int readUTF32BE(FILE *inputFile, int *errorCode) {
  return read4Bytes(inputFile, MODE_BE, errorCode);
}

void writeUTF32LE(FILE *outputFile, const unsigned int symbol) {
  write4Bytes(outputFile, symbol, MODE_LE);
}

void writeUTF32BE(FILE *outputFile, const unsigned int symbol) {
  write4Bytes(outputFile, symbol, MODE_BE);
}

void copyRemainingBytes(FILE *inputFile, FILE *outputFile) {
  int ch;
  while ((ch = fgetc(inputFile)) != EOF) {
    fputc(ch, outputFile);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "wrong count arguments");
    return 1;
  }
  int encodeOut = argv[3][0] - '0';
  if (encodeOut < 0 || 5 < encodeOut) {
    fprintf(stderr, "unknown output encoding");
    return 1;
  }
  FILE *inputFile = fopen(argv[1], "rb");
  if (inputFile == NULL) {
    fprintf(stderr, "error open read input file: %s", argv[1]);
    return 1;
  }
  FILE *outputFile = fopen(argv[2], "wb");
  if (outputFile == NULL) {
    fprintf(stderr, "error open write output file: %s", argv[2]);
    fclose(inputFile);
    return 1;
  }
  int encodeIn = getEncoding(inputFile);
  unsigned int (*readSymbol)(FILE *, int *);
  void (*writeSymbol)(FILE *, unsigned int);
  switch (encodeIn) {
    default:
    case UTF8_WITHOUT_BOM:
    case UTF8_WITH_BOM:
      readSymbol = readUTF8;
      break;
    case UTF16LE:
      readSymbol = readUTF16LE;
      break;
    case UTF16BE:
      readSymbol = readUTF16BE;
      break;
    case UTF32LE:
      readSymbol = readUTF32LE;
      break;
    case UTF32BE:
      readSymbol = readUTF32BE;
      break;
  }
  switch (encodeOut) {
    default:
    case UTF8_WITHOUT_BOM:
    case UTF8_WITH_BOM:
      writeSymbol = writeUTF8;
      break;
    case UTF16LE:
      writeSymbol = writeUTF16LE;
      break;
    case UTF16BE:
      writeSymbol = writeUTF16BE;
      break;
    case UTF32LE:
      writeSymbol = writeUTF32LE;
      break;
    case UTF32BE:
      writeSymbol = writeUTF32BE;
      break;
  }
  int errorCode = 0;
  writeBOM(outputFile, encodeOut);
  if (encodeIn == encodeOut ||
      (encodeIn == UTF8_WITHOUT_BOM && encodeOut == UTF8_WITH_BOM) ||
      (encodeIn == UTF8_WITH_BOM && encodeOut == UTF8_WITHOUT_BOM)) {
    copyRemainingBytes(inputFile, outputFile);
  } else {
    while (true) {
      unsigned int symbol = readSymbol(inputFile, &errorCode);
      if (ferror(inputFile)) {
        fprintf(stderr, "error read input file: %s", argv[1]);
        fclose(inputFile);
        fclose(outputFile);
        return 2;
      }
      if (errorCode != 0) {
        break;
      }
      writeSymbol(outputFile, symbol);
      if (ferror(outputFile)) {
        fprintf(stderr, "error write output file: %s", argv[2]);
        fclose(inputFile);
        fclose(outputFile);
        return 2;
      }
    }
  }
  fclose(inputFile);
  fclose(outputFile);
  return 0;
}
