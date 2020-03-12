#include <deepstate/DeepState.hpp>
#include <stdio.h>
#include <stdlib.h>
extern "C" {
#include "c_printf/sources/c_printf.h"
}

using namespace deepstate;

TEST(CPrintF, PrintString){
    size_t our_size = DeepState_SizeInRange(1,4096);
    LOG(TRACE) << "String size = " << our_size;
    // random string from deepstate
    char* deepStateString = DeepState_CStrUpToLen(our_size);
    // test copy function
    ::c_printf(deepStateString);
}
