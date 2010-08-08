#ifndef H_ATOMIC
#define H_ATOMIC

#include "windows.h"
#include <iostream>

class Atomic {
	protected:
	private:
        unsigned int m_target;
    public:

    Atomic::Atomic() {
        m_target = 0;
    }

    Atomic::~Atomic() {
    }

    Atomic::Atomic(unsigned int new_value) {
        operator=(new_value);
    }

    inline void Atomic::operator=(unsigned int new_value) {
        //non-voidyet
        m_target = new_value;
    }

    inline void Atomic::operator+=(unsigned int modifier) {
        #if defined(_MSC_VER)
            //Here's a fix for our "lovely" MSVC++
            InterlockedExchangeAdd(&m_target, modifier);
        #else
            //And the GNU's way, similar to fetch_and_add()
             __asm__ __volatile__(
                  "lock addl %1,%0 ;\n\t"
                  : "=m"  (m_target)
                  : "ir"  (modifier), "m" (m_target)
             );
        #endif
    }

    inline void Atomic::operator++() {
        #if defined(_MSC_VER)
            //Here's a fix for our "lovely" MSVC++
            InterlockedIncrement(&m_target);
        #else
             __asm__ __volatile__(
                  "lock incl %0 ;\n\t"
                  :"=m" (m_target)
                  :"m" (m_target)
             );
        #endif
    }

    inline void Atomic::operator--() {
        #if defined(_MSC_VER)
            //Here's a fix for our "lovely" MSVC++
            InterlockedDecrement(&m_target);
        #else
             __asm__ __volatile__(
                  "lock decl %0 ;\n\t"
                  : "=m" (m_target)
                  : "m" (m_target)
             );
        #endif
    }

    inline void Atomic::operator++(int) {
        operator++();
    }

    inline void Atomic::operator--(int) {
        operator--();
    }

    inline void Atomic::operator-=(unsigned int modifier) {
        #if defined(_MSC_VER)
            //Here's a fix for our "lovely" MSVC++
            //Btw this is taking advantage of overloading an int as decribed by MSVN docs
            InterlockedExchangeAdd(&m_target, -modifier);
        #else
            __asm__ __volatile__(
                  "lock subl %1,%0 ;\n\t"
                  : "=m"  (m_target)
                  : "ir"  (modifier), "m" (m_target)
             );
        #endif
    }

    inline Atomic Atomic::operator+(unsigned int modifier) {
        operator+=(modifier);
        return *this;
    }

    inline Atomic Atomic::operator-(unsigned int modifier) {
        operator-=(modifier);
        return *this;
    }

    inline unsigned int Atomic::getValue() {
        return m_target;
    }

    friend std::ostream &operator<<(std::ostream &stream, Atomic &o) {
      stream << o.m_target;
      return stream;
    }
};

#endif
