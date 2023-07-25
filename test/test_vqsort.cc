#include "../src/lib/hwy-vqsort/sort/vqsort.h"
#include <stdexcept>
#include <stdint.h>
#include <iostream>

extern "C" {
// --- i32 ---
void vqsort_avx2_i32(int32_t* data, size_t len) {
	hwy::Sorter{}(data, len, hwy::SortAscending{});
}
// --- u64 ---
void vqsort_avx2_u64(uint64_t* data, size_t len) {
	hwy::Sorter{}(data, len, hwy::SortAscending{});
}
}  // extern "C"

int main(int argc, char *argv[])
{
    std::cout << "Hello world!" << argc << '\n';

    int32_t data[10] = { 10, 30, 50, 0, 80, 20, 90, 70, 40, 60 };

    vqsort_avx2_i32(data, 10);

    for (int i = 0; i < 10; i++) 
        std::cout << data[i] << " ";

    return  EXIT_SUCCESS;
}
