#ifndef KVLONG_H
#define KVLONG_H

#include <stddef.h>
#include <stdint.h>
//#include <stdio.h>

#include "kv.h"

class KVlong : public abstractKV<long int> {
public:

    bool getKV(const int pos, String& keyvalue) override {
        char buffer[15];
        bool found = isIndexValid(pos);
        if (found) {
            //snprintf(buffer, sizeof(buffer), "%ld", keyValues[pos].value);
            ltoa(keyValues[pos].value, buffer, sizeof(buffer)-1);
            keyvalue = keyValues[pos].key + KV_SEPARATOR + buffer;
        }
        return found;
    };

    bool putKV(const char* keyvalue) override {
        char key[30];
        long value;

        strncpy(key, keyvalue, sizeof(key) - 1);
        key[sizeof (key) - 1] = 0;
        char* separator = strchr(key, KV_SEPARATOR);
        if (separator != NULL) {
            *separator = 0;
            value = atol(separator + 1);
            return put(key, value);
        }
        return false;
    };
};

#endif
