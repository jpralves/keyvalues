
// Based on:
// Embedis - Embedded Dictionary Server
//    Copyright (C) 2016 PatternAgents, LLC
// https://github.com/thingSoC/embedis/



#ifndef TVEC_H
#define TVEC_H

#include <stddef.h>
#include <stdint.h>

inline void * operator new(size_t size, void * ptr) {
      (void)size;
      return ptr;
}

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
