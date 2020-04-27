#include <deepstate/DeepState.hpp>
#include <stdio.h>
#include <stdlib.h>
extern "C" {
#include "libutf8/utf8.h"
}

using namespace deepstate;

TEST(LibUTF8, EncodeDecode){
    // random string from deepstate
    size_t our_size = DeepState_SizeInRange(1,2048);
    char* deepStateString = DeepState_CStr_C(our_size, NULL);
    deepStateString[our_size-1] = '\0';
    // test copy function
    char* response = utf8_encode(deepStateString);
    response = utf8_decode(response);
    for(int i = 0; i < our_size; i++) {
      ASSERT_EQ(deepStateString[i], response[i]);
    }
    free(response);
}

