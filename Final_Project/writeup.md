# System Under Test
As with previous projects, my primary method of finding a good project to test was to use the GitHub project search feature and subsequently limit the results to projects containing "lib" in the name with under 5 forks.
This was fairly successful, though there was a lot of noise to filter through. I chose `libutf8`, since coincidentally I had read an article that day talking about how developers rarely develop for wide character capable systems, which ends up creating security vulnerabilities in the long run. `libutf8` is a plain C library that allows for the encode and decode of C strings into wide-char UTF8 capable strings and vise-versa.

# Fuzzing/Testing Approaches
As I'll discuss later, due to the limitations of the various fuzzer implementations, I had different results and used different workarounds to get the results that I discribe later. I wrote up a simple test harness to test both the encode function and the decode function in varying orders.
I ended up using the deepstate variants of AFL, LibFuzzer and the default DeepState bruteforce fuzzer. Due to a quirk of the `libutf8` library, I had to add an abort sequence in the harness if certain UTF8 validity constraints are not met. For the purposes of the test, I set a timeout of 10 minutes. For AFL, since it requires an input seed, in order to give it enough randomness to be able to execute the program fully, I used the `dd` utility to generate 4KB of random data (ex. `dd if=/dev/random of=./input/seed bs=1k count=4`). This seemed to resolve a bug that I was initially having with AFL, where the test case wouldn't succeed due to some constraints of the library.

For libFuzzer, I enabled the AddressSanitizer functionality in order to catch HeapOverflows and other useful information. I also left LeakSanitizer enabled, which initially helped me debug a memory leak present in my test harness. All three fuzzers found different bugs which I will talk about in the next section.

# Bug Information

## Bug number 1 (AddressSanitizer HeapOverflow)
AddressSanitizer found the heap overflow that I initially injected into the `libutf8` source code. Basically it's an off-by-one bug that forces the while loop to read past the edge of the string buffer. Nothing super interesting, other than the bruteforce fuzzer finding the bug enormously faster than the rest. Unfortunately, I had to modify the source code of the harness in order to allow this to happen.

## Bug number 2 (HeapOverflow)
AddressSanitizer detected a Heap Overflow in both the encode and decode functions of `libutf8`. This was interesting because under normal circumstances, the bug could have been resolved simply by implementing a null check on the parameter. I believe we can consider this a bug, since it results in a segfault if passed a null value. The software itself returns NULLs in the condition of invalid inputs are detected!

# Code Coverage
In an effort to figure out if the tests were actually executing successfully, I decided to use the LLVM coverage profiler to generate coverage data. Setting this system up was actually quite simple, though a little strange remembering the steps. First off, I adjusted my Makefile, as well as the third-party Makefile, to include the `-fprofile-instr-generate -fcoverage-mapping` flags. This enabled the profiler to execute upon program run. Then I setup the `LLVM_PROFILE_FILE` environment variable to save to `dank.profraw`. Following this, I used `llvm-profdata-9 merge` in order to index and prepare the raw profiler data for usage in the next step. Finally, I used `llvm-cov-9 report ./main --instr-profile="dank.profdata"` to display a pretty coverage report. For some odd reason, the Test.cpp file had a 50% coverage across all the different fuzzer implementations. That said, the actual library had a (nearly) 100% coverage across the board.

# Comparison
Ultimately, by far my favorite tool in terms of versatility and bug-finding firepower, I loved AFL. The fact I could get it to load libFuzzer's AddressSanitizer to aid in testing just further improved the experience. In this odd case, the bruteforce fuzzer from Deepstate found the "obvious" bug faster, but AFL found *more* bugs than the one I injected. `libFuzzer` is a close second, only because it found the same bugs, albeit a tad slower than the AFL fuzzer. I found that AFL ended up generally being easier to debug with GDB, especially than libFuzzer, but the easiest by far was the deepstate builtin fuzzer. My workflow ended up using the deepstate fuzzer to actually test my testing harness, and using AFL to test and debug the actual library to get results.

That said, I believe the results would be stable across the board if I reran the same test again. The library is fairly compact and because of that is simple to fuzz. I find the liklihood of a timing attack/etcetera to be fairly small, and since it's all single threaded, this even further reduces that risk. I believe because the coverage is high, and the library is simple, the reality is 10 minutes might have actually been too long for the library.

### A final note on DeepState
I love DeepState's ability to build a common testing framework for various fuzzers, but various little gotchas get in the way of building tests efficiently.

For example, while DeepState does a pretty good job in general of keeping the syntax similar between frameworks, some behavior is different between them and relatively undocumented. For one, on AFL, the behavior between a seed and an actual runtime test can be different depending on the situation. This mostly comes down to when a seed test runs out of entropy. It can cause actual random tests to function properly where the seed test doesn't.

Also, DeepState requires double quotes around it in order to be even remotely searchable on Google without getting news articles about the Deep State(TM). That is all.
