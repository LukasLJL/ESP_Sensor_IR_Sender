#include <Arduino.h>
#include "stringToUint_64.h"

uint64_t stringToUint_64(String value)
{
  int stringLenght = value.length();

  uint64_t uint64Value = 0x0;
  for (int i = 0; i <= stringLenght - 1; i++)
  {
    char charValue = value.charAt(i);

    uint64Value = 0x10 * uint64Value;
    uint64Value += stringToHexInt(charValue);
  }

  return uint64Value;
}

int stringToHexInt(char value)
{
  switch (value)
  {
  case '0':
    return 0;
    break;
  case '1':
    return 0x1;
    break;
  case '2':
    return 0x2;
    break;
  case '3':
    return 0x3;
    break;
  case '4':
    return 0x4;
    break;
  case '5':
    return 0x5;
    break;
  case '6':
    return 0x6;
    break;
  case '7':
    return 0x7;
    break;
  case '8':
    return 0x8;
    break;
  case '9':
    return 0x9;
    break;
  case 'A':
  case 'a':
    return 0xA;
    break;
  case 'B':
  case 'b':
    return 0xB;
    break;
  case 'C':
  case 'c':
    return 0xC;
    break;
  case 'D':
  case 'd':
    return 0xD;
    break;
  case 'E':
  case 'e':
    return 0xE;
    break;
  case 'F':
  case 'f':
    return 0xF;
    break;
  }
}