/*
 * Library Name: keyvalues
 *
 * Filename: kv.h
 * Description: library abstractKV interface/implementation
 *
 * Version: 1.0.2
 * Author: Joao Alves <jpralves@gmail.com>
 * Required files: tvec.h
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

#ifndef KV_H
#define KV_H

#include <stddef.h>
#include <stdint.h>
#include "WString.h"

#include "tvec.h"

#define KV_SEPARATOR '='
#define KVS_SEPARATOR '\n'

template <typename T> class abstractKV {

  protected:

    struct KV {
      String key;
      T value;
    };
    TVec<KV> keyValues;

  public:

    abstractKV() {};
    ~abstractKV() {
      int pos;
      while ((pos = keyValues.size() - 1) > 0) {
        keyValues.remove(pos);
      }
    };
    abstractKV(const abstractKV&);
    abstractKV& operator=(const abstractKV&);

    // Key / Value related

    int size() {
      return keyValues.size();
    };

    bool isIndexValid(int index) {
      return (index >= 0) &&  (index < size());
    };

    bool containsKey(const char* key) {
      int pos = getIndexKey(key);
      return (pos >= 0);
    };

    int getIndexKey(const char* key)  {
      for (size_t pos = 0; pos < keyValues.size(); pos++) {
        if (keyValues[pos].key == key) {
          return pos;
        }
      }
      return -1;
    };

    bool get(const int pos, T& value) {
      bool found = isIndexValid(pos);
      if (found) {
        value = keyValues[pos].value;
      }
      return found;
    };
    
    bool get(const char* key, T& value) {
      return get(getIndexKey(key), value);
    };

    bool get(const int pos, String& key, T& value) {
      bool found = isIndexValid(pos);
      if (found) {
        key = keyValues[pos].key;
        value = keyValues[pos].value;
      }
      return found;
    };

    virtual bool getKV(const int, String&) = 0;

    bool put(String key, T value) {
      return put(key.c_str(), value);
    }
	
    bool put(const char* key, T value) {
      int pos = getIndexKey(key);
      bool found = (pos >= 0);
      if (!found) {
        pos = keyValues.size();
        keyValues.resize(pos + 1);
        keyValues[pos].key = key;
      }
      keyValues[pos].value = value;
      return found;
    };
    
    virtual bool putKV(const char*) = 0;

    void putKVs(const __FlashStringHelper *ifsh) {
        const char *pvar = (const char PROGMEM *)ifsh;
        char keyvalue[100];
        uint16_t p = 0;
        uint8_t c;

        while( (c = pgm_read_byte(pvar++)) && (p<(sizeof(keyvalue)-1))) {
          keyvalue[p] = c;
          if (keyvalue[p] == KVS_SEPARATOR) {
            keyvalue[p] = '\0';
            p = 0;
            putKV(keyvalue);
          } else {
            p++;
          }
        }
        if (p) {
            keyvalue[p] = '\0';
            putKV(keyvalue);          
        }
    };

    void putKVs(const char* keyvalues) {
        char keyvalue[100];
        uint16_t p = 0;
        uint8_t c;
        uint16_t i = 0;

        while( (c = keyvalues[i++]) && (p<(sizeof(keyvalue)-1))) {
          keyvalue[p]=c;
          if (keyvalue[p] == KVS_SEPARATOR) {
            keyvalue[p] = '\0';
            p = 0;
            putKV(keyvalue);
          } else {
            p++;
          }
        }
        if (p) {
            keyvalue[p] = '\0';
            putKV(keyvalue);          
        }
    };

    bool remove(const char* key) {
      int pos = getIndexKey(key);
      bool found = (pos >= 0);
      if (found) {
        keyValues.remove(pos);
      }
      return found;
    };

    bool remove(const int pos) {
	  bool found = isIndexValid(pos);
      if (found) {
        keyValues.remove(pos);
      }
      return found;
    };

};

#endif
