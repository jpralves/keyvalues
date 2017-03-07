#ifndef KVSTRING_H
#define KVSTRING_H

#include <stddef.h>
#include <stdint.h>

#include "kv.h"

class KVString : public abstractKV<String> {
public:

    bool getKV(const int pos, String& keyvalue) override {
        bool found = isIndexValid(pos);
        if (found) {
            keyvalue = keyValues[pos].key + KV_SEPARATOR + keyValues[pos].value;
        }
        return found;
    };

    bool putKV(const char* keyvalue) override {
        char key[100];
        char *value;

        strncpy(key, keyvalue, sizeof (key) - 1);
        key[sizeof (key) - 1] = 0;
        char* separator = strchr(key, KV_SEPARATOR);
        if (separator != NULL) {
            *separator = 0;
            value = separator + 1;
            return put(key, value);
        }
        return false;
    };
};

#endif
