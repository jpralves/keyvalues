# keyvalues
This is an Arduino library for storing Key/Values in RAM.

See the examples to see how it works.

This Library uses TVEC Template Class from Embedis project (https://github.com/thingSoC/embedis/)

## abstractKV
The class abstractKV implements a template class for use with different types of values.

## KVString

This class implements a class with value of type String

## KVlong

This class implements a class with value of type long

## How to use it

- putting a key/value: objectname.put(keyname, value)
- obtaining a value: objectname.get(keyname, value variable)
- obtaining the number of keys/values: objectname,.size()
- deleting a key/value: objectname.remove(keyname)

The abstractKV implements methods for get/put/delete values in the list.

Simple example:

```
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
```

## Examples:

- simpleKV - The above example
- progmemKV - More complex example - uses PROGMEM for the source of the values
- persistentKV - dumps KVs to EEPROM and loads them. This example requires CRC16 library,


