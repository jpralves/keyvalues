
#if (defined(__AVR__))
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

#include "kvstring.h"

KVString kvs;

void setup() {
  Serial.begin(9600);
  // Simple set value:
  kvs.set("key1","value1");
	// Pass a K=V:
  kvs.set("key2=value2");
	// Pass a bunch of KV from PROGMEM:
  kvs.set(F("key3=value3\nkey4=value4\nkey5=value5"));
	// List all keys stored:
  listKVs();
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
