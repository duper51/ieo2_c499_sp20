#include <deepstate/DeepState.hpp>
#include <stdio.h>
#include <stdlib.h>
extern "C" {
#include "libutf8/utf8.h"
}
#include <errno.h>

using namespace deepstate;

TEST(LibUTF8, EncodeDecode){
    // random string from deepstate
    char* deepStateString = DeepState_CStrUpToLen(2048);
    // test copy function
    char* response = utf8_encode(deepStateString);
    ASSUME_EQ(response != NULL, true);
    char* response2 = utf8_decode(response);
    LOG(TRACE) << "EncodeDecode finished";
    for(int i = 0; deepStateString[i] != '\0'; i++) {
      ASSERT_EQ(deepStateString[i], response2[i]);
    }
    free(response);
    free(response2);
}

TEST(LibUTF8, DecodeEncode){
    // random string from deepstate
    char* deepStateString = DeepState_CStrUpToLen(2048);
    // test copy function
    char* response = utf8_decode(deepStateString);
    ASSUME_EQ(response != NULL, true);
    char* response2 = utf8_encode(response);
    LOG(TRACE) << "EncodeDecode finished";
    for(int i = 0; deepStateString[i] != '\0'; i++) {
      ASSERT_EQ(deepStateString[i], response2[i]);
    }
    free(response);
    free(response2);
}


