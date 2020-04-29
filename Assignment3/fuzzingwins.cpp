#include <deepstate/DeepState.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "fuzzgoat.h"

using namespace deepstate;

TEST(FuzzingWins, JSONTest){
    size_t our_size = DeepState_SizeInRange(1,2048);
    // random string from deepstate
    char* deepStateString = DeepState_CStr(our_size);
    // test copy function
    json_value* value = json_parse((json_char*) deepStateString, our_size);
    ASSUME_NE(value, NULL);
    json_value_free(value);
}
