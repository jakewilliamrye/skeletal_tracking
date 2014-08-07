 #ifndef EEPROM_INTERFACE_H_
#define EEPROM_INTERFACE_H_

//Include Libraries
#include <EEPROM.h>
#include <Arduino.h>

//Writes the given data structure, s, to EEPROM starting at eeprom_address. Returns the number of bytes written.
template<class T> int WriteEeprom(T& s, int eeprom_address) {
  const byte* p = (const byte*)(const void*)&s;
  unsigned int i;
  for(i=0;i < sizeof(s); i++)
    EEPROM.write(eeprom_address++, *p++);
  return i;
}

//Reads from EEPROM into data structure s, starting from eeprom_address. Returns the number of bytes read.
template<class T> int ReadEeprom(T& s, int eeprom_address) {
  byte* p = (byte*)(void*)&s;
  unsigned int i;
  for(i=0;i < sizeof(s); i++)
    *p++ = EEPROM.read(eeprom_address++);
  return i;
}

#endif
