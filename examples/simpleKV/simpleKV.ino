/*
 * Filename: simpleKV.ino
 * Description: Example for library keyvalues
 *
 * Version: 1.0.0
 * Author: Joao Alves <jpralves@gmail.com>
 * Required files: -
 * Required Libraries: keyvalues
 * Tested on: Arduino Nano, Arduino Uno, ESP8266
 *
 * History:
 * 1.0.0 - 2017-03-14 - Initial Version
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
