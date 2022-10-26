# Secure Fisher Yates
Secure implementations of Fisher Yates permutation sampling

Build and run the code with optimizaiton `-o3`
```
mkdir cmake-build-release
cd cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./Benchmark
```

Build and run the code without optimizaiton `-o0`
```
mkdir cmake-build-debug
cd cmake-build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
./Benchmark
```
