#include "kvstring.h"

KVString kvs;
String value;

void setup() {
  Serial.begin(9600);
  kvs.put("key", "value");
  kvs.get("key", value);
  Serial.print("key=");
  Serial.println(value);
}

void loop() {}
