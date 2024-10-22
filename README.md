# Methods for Sampling Random Permutations in Cryptography

This library accompany the SoK: Methods for Sampling Random Permutations in Cryptography

The code is released under the [MIT licence](https://en.wikipedia.org/wiki/MIT_License). 

This project makes use of the following external libraries with corresponding licence:

- [_djbsort_](https://sorting.cr.yp.to/) - not specified, possibly [unlicensed](http://unlicense.org)
- [dudect](https://github.com/oreparaz/dudect) - [unlicensed](http://unlicense.org)
- [highway](https://github.com/google/highway) - [Apache 2.0](http://www.apache.org/licenses/LICENSE-2.0) and [BDS-3](https://en.wikipedia.org/wiki/BSD_licenses)
- [xkcp](https://keccak.team/) - [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/)

The code has been tested to work correctly on UNIX systems with compiler `clang version 14.0.3`.

### Benchmarks

Build and run all benchmarks
```
python bench_run.py
```
NOTE: modify the variable `N_ITERATIONS` in `/src/definitions.h` to specify the number of iterations
to be run in the benchmark tests.

To parse the result, see the usage of
```
./bench_parse.sh [sp,cp,ip,sui] [avx2]
```
To generate the tikz plot for LaTex
```
gnuplot -c bench_plot.gp ./bench_dir/[NAME_FILE]
```
`[NAME_FILE]` stands for the name of the file generated by the parsing script, and it starts with `benchmark_...`.

