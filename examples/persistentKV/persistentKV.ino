#include <EEPROM.h>

#if (defined(__AVR__))
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

// This example requires CRC16 from https://github.com/jpralves/crc16

#include "kvstring.h"
#include "crc16.h"

KVString kvs;

#define EEPROM_SIZE 1024

const char *config_signature = "KVScfg";

void writeValuesToEEPROM() {
  char buffer[64];
  CRC16 crc;
  uint16_t configSize = 0;
  uint16_t startEEPROM = 0;
  uint16_t configpos = 0;
  uint8_t i;
  String kv;

#if (defined(ESP8266))
  EEPROM.begin(1024);
#endif

  crc.processBuffer(config_signature, sizeof(config_signature));
  for (i = 0; i < sizeof(config_signature); i++) {
    EEPROM.write(startEEPROM+i, config_signature[i]);
  }
  startEEPROM += sizeof(config_signature);
  for (i = 0; i < kvs.size(); i++) {
    if (kvs.getKV(i, kv)) {
      configSize += kv.length() + 1;
    }
  }
  buffer[0] = configSize >> 8;
  buffer[1] = configSize & 0xff;

  EEPROM.write(startEEPROM    , buffer[0]);
  EEPROM.write(startEEPROM + 1, buffer[1]);

  crc.processBuffer(buffer, 2);

  startEEPROM += 2;
  configpos = 0;
  for (i = 0; i < kvs.size(); i++) {
    if (kvs.getKV(i, kv)) {
      kv += "\n";
      strncpy(buffer, kv.c_str(), sizeof(buffer));
	    crc.processBuffer(buffer, kv.length());
      configpos += EEPROMwriteString(startEEPROM + configpos, kv);
    }
  }

  startEEPROM += configSize;
  uint16_t res_crc = crc.getCrc();	
  EEPROM.write(startEEPROM    , res_crc >> 8);
  EEPROM.write(startEEPROM + 1, res_crc & 0xff);

#if (defined(ESP8266))
  EEPROM.end();
#endif
  Serial.print(F("CRC=0x"));
  Serial.print(res_crc,HEX);
  Serial.println(F(". Configuration Written to EEPROM!"));
}

int EEPROMwriteString(int pos, String s) {
  uint16_t ss = s.length();
  for (uint16_t i = 0; i < ss; i++){
    EEPROM.write(pos + i, s[i]);
  }
  return ss;
}

bool readValuesFromEEPROM() {
  CRC16 crc;
  char buffer[64];
  uint16_t startEEPROM = 0;
  uint16_t configSize = 0;
  uint16_t configpos = 0;
  uint8_t i;
  
  for(i = 0; i < 8; i++) {
    buffer[i] = EEPROM.read(startEEPROM+i);
    if (buffer[i] != config_signature[i]) {
      Serial.println(F("Signature not found... empty configuration"));
      return false;
    }
  }
  crc.processBuffer(buffer, 8);
  startEEPROM += 8;
  
  buffer[0] = EEPROM.read(startEEPROM);
  buffer[1] = EEPROM.read(startEEPROM + 1);
  crc.processBuffer(buffer, 2);
  startEEPROM += 2;
  
  configSize = buffer[0] << 8 | buffer[1];
  configpos = 0;
  while (configpos < configSize) {
    for(i = 0; i < sizeof(buffer) && configpos < configSize; i++) {
       buffer[i] = EEPROM.read(startEEPROM + configpos);
       configpos++;
    }
    crc.processBuffer(buffer, i < sizeof(buffer) ? i : sizeof(buffer));
  }
  uint16_t res_crc = crc.getCrc();
  
  uint16_t testcrc = EEPROM.read(startEEPROM + configSize) << 8 | EEPROM.read(startEEPROM + configSize + 1);
  if (testcrc != res_crc) {
    Serial.println(F("Invalid CRC Configuration."));
    Serial.print(F("Stored CRC(0x"));
    Serial.print(testcrc,HEX);
    Serial.print(F(") != CalcCRC(0x"));
    Serial.print(res_crc,HEX);
    Serial.println(")");
    return false;
  }

  // The checksum is valid so read kv.
  configpos = 0;
  while(configpos < configSize) {
    for(i = 0; i<sizeof(buffer) && configpos < configSize; i++) {
       buffer[i] = EEPROM.read(startEEPROM + configpos);
       configpos++;
       if (buffer[i] == '\n') {
         break;
      }
    }
    buffer[i] = '\0';
    kvs.putKV(buffer);
  }
  Serial.println(F("Configuration Read from EEPROM!"));  
  return true;
}

void setup() {
  Serial.begin(9600);
  if (readValuesFromEEPROM()) {
    listKVs();
  } else { 
    // Try to create values and write then....
	  
    // Simple set value:
    kvs.put("key1","value1");
	// Pass a K=V:
    kvs.putKV("key2=value2");
	// Pass a bunch of KV from PROGMEM:
    kvs.putKVs(F("key3=value3\nkey4=value4\nkey5=value5"));
	// List all keys stored:
    writeValuesToEEPROM();
	Serial.println("Should reset now to test EEPROM read...");
  }
}

void listKVs() {
  String k, v;
  uint16_t keycount = kvs.size();
  
  Serial.print(keycount);
  Serial.println(F(" K/V found."));
  for (uint16_t i = 0; i < keycount; i++) {
    if (kvs.get(i, k, v)) {
  		Serial.print(k);
  		Serial.print(" = ");
  		Serial.println(v);  
    }
  }
}

void loop() {}