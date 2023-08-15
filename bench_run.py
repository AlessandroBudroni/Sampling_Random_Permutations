
import os, sys

cmake_compilers = '-D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++'


param_n = [32,64,128,256,512,1024,2048]
# bits_n = 7
# bits_n_mask = 0x7F

main_dir = 'bench_dir'

for n in param_n:
    dir = f'{main_dir}/param_n_'+str(n)

    command = 'mkdir -p ' + dir
    print(command)
    os.system(command)

    command = 'cmake -DCMAKE_BUILD_TYPE=Release -B ' + dir + ' ' + cmake_compilers
    command = command + ' -D PARAM_N=' + str(n) #+ ' -D BITS_PARAM_N=' + str(bits_n) + ' -D BITS_PARAM_N_MASK=' + str(0x3F)
    print(command)
    os.system(command)

    command = 'cmake --build ./'+dir+' --target all -- -j'
    print(command)
    os.system(command)

    command = f"for scheme in $(ctest --test-dir ./{dir} -N | awk '/benchmark_api/ {{print $3}}'); do echo $scheme; ./{dir}/test/$scheme -R $scheme > {main_dir}/param_{n}_$scheme; done"
    print(command)
    os.system(command)





