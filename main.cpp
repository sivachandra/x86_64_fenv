
#include "fenv.h"
#include <stdio.h>

long double __myDiv(volatile long double a, volatile long double b) {
  return a / b;
}

int main() {

    constexpr int allExcepts = FE_DIVBYZERO | FE_INVALID | FE_INEXACT | FE_OVERFLOW | FE_UNDERFLOW;

    int e = FE_DIVBYZERO;

    __llvm_libc::fputil::disableExcept(FE_ALL_EXCEPT);
    __llvm_libc::fputil::enableExcept(e);
    __llvm_libc::fputil::clearExcept(FE_ALL_EXCEPT);
    // Raising all exceptions except |e| should not call the
    // SIGFPE handler. They should set the exception flag though,
    // so we verify that.
    int others = allExcepts & ~e;
    __llvm_libc::fputil::raiseExcept(others);
    __llvm_libc::fputil::testExcept(others);

    // We don't check the return value when raising |e| as
    // feraiseexcept will not return when it raises an enabled
    // exception.
    // __llvm_libc::feraiseexcept(e);
    volatile long double a = 1.0;
    volatile long double b = 0.0;
    volatile long double c = __myDiv(a,b);
    // __asm__ __volatile__("fwait");
    volatile long double d = 5.0;
    if(d/c==0.0) {
        printf("idk\n");
    }
    return 0;
}
    
