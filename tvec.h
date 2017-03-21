
// Based on:
// Embedis - Embedded Dictionary Server
//    Copyright (C) 2016 PatternAgents, LLC
// https://github.com/thingSoC/embedis/

#ifndef TVEC_H
#define TVEC_H

#include <stddef.h>
#include <stdint.h>

/* Architecture Specific */
#if defined(ARDUINO_ARCH_AVR)
   /* AVR specific code - break down further by cpu model if necessary */
   // #pragma message ( "AVR Architecture Selected" )
   #include "avr/pgmspace.h"
    /* it's NEW! */
	/* "<new>" doesn't seem to be supported on all platforms/compilers           */
	/* this should likely be selected per compiler and not by Arduino IDE...     */
	/* but this works, until I can track down all the difference in compilers... */
	inline void * operator new(size_t size, void * ptr) {
      (void)size;
      return ptr;
    }
#elif defined(ARDUINO_ARCH_SAM)
   /* SAM3X specific code - break down further by cpu model if necessary */
   // #pragma message ( "SAM3X Architecture Selected" )
  #include <new>
#elif defined(ARDUINO_ARCH_SAMD)
   /* SAMD specific code - break down further by cpu model if necessary */
   // #pragma message ( "SAMD Architecture Selected" )
  #include <new>
#elif defined(ARDUINO_ARCH_ARC32)
   /* Currie/101 specific code - break down further by cpu model if necessary */
   // #pragma message ( "ARC32/Currie Architecture Selected" )
    /* it's NEW! */
	/* "<new>" doesn't seem to be supported on all platforms/compilers           */
	/* this should likely be selected per compiler and not by Arduino IDE...     */
	/* but this works, until I can track down all the difference in compilers... */
	inline void * operator new(size_t size, void * ptr) {
      (void)size;
      return ptr;
    }
#elif defined(ARDUINO_ARCH_ESP8266)
   /* ESP8266 specific code - break down further by cpu model if necessary */
   // #pragma message ( "ESP8266 Architecture Selected" )
  #include <new>
#elif defined(CORE_TEENSY)
   /* Teensy3.x specific code - break down further by cpu model if necessary */
   /* this is not ideal, should change to use "_ARCH_" */ 
   // #pragma message ( "Teensy Core Architecture Selected" )
  #include "avr/pgmspace.h"
  #include <new>
#elif defined(__ARDUINO_X86__)
   /* Edison, Galileo, x86 specific code - break down further by cpu model if necessary */
   /* this is not ideal, should change to use "_ARCH_" */ 
   // #pragma message ( "X86 Architecture Selected" )
   #include "avr/pgmspace.h"
#else
  /* untested architecture, it might work, but likely not... */
  // #pragma message "Core Architecture not Recognized - untested... "
  #include "avr/pgmspace.h"
  #include <new>
#endif


// Tiny vector.
    template <typename T>
    class TVec {
        size_t length;
        T* ptr;
        void refinish (size_t n) {
          if (n == 0) {
            free(ptr);
            ptr = 0;
            length = 0;
            return;
          }
          T* reptr = (T*)realloc(ptr, sizeof(T) * n);
          if (reptr) {
            ptr = reptr;
            for (size_t i = length; i < n; i++) {
              new(&ptr[i]) T;
            }
            length = n;
          }
        }
      public:
        TVec () : length(0), ptr(0) {}
        ~TVec () {
          resize(0);
        }
        TVec(const TVec&);
        TVec& operator=(const TVec&);
        T& operator[] (const size_t index) {
          return ptr[index];
        }

        const T& operator[] (const size_t index) const {
          return ptr[index];
        }
        size_t size () const {
          return length;
        }

        void resize (size_t n) {
          if (n == length) return;
          for (size_t i = n; i < length; i++) {
            ptr[i].~T();
          }
          refinish(n);
        }
        void remove (size_t n) {
          if (n >= length) return;
          ptr[n].~T();
          memmove(&ptr[n], &ptr[n + 1], sizeof(T) * (length - n - 1));
          refinish(length - 1);
        }
    };


#endif
