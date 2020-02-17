#!/bin/bash
cd openssl
make clean
export BONUS_FLAGS="-O2 -fno-omit-frame-pointer -gline-tables-only -fsanitize=address,fuzzer-no-link -fsanitize-address-use-after-scope"
CC="clang $BONUS_FLAGS" CCX="clang++ $BONUS_FLAGS" ./config -d
make
cd ../
clang -g -O2 -fsanitize=fuzzer,address libfuzz_cfunction.cc openssl/libssl.a openssl/libcrypto.a -o libfuzz_cfunction -I openssl/include -ldl
./libfuzz_cfunction
