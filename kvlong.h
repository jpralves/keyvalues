/*
 * Library Name: keyvalues
 *
 * Filename: kvlong.h
 * Description: library KVlong interface/implementation
 *
 * Version: 1.0.2
 * Author: Joao Alves <jpralves@gmail.com>
 * Required files: kv.h, tvec.h
 *
 * History:
 * 1.0.2 - 2017-03-14 - Initial Version
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
