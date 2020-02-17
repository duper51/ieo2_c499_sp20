#!/bin/bash
cd openssl
make clean
CC=afl-clang-fast CCX=afl-clang-fast++ ./config -d
AFL_USE_ASAN=1 make
cd ..
AFL_USE_ASAN=1 afl-clang-fast++ -g afl_cfunction.cc openssl/libssl.a openssl/libcrypto.a -o afl_cfunction -I openssl/include -ldl
AFL_SKIP_CPUFREQ=1 sudo -E ~/afl-2.52b/experimental/asan_cgroups/limit_memory.sh -u fuzzer afl-fuzz -i inputs -o out -m none ./afl_cfunction
