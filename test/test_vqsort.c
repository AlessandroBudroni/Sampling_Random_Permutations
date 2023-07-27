#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

void vqsort_avx2_i32_asc(int32_t* data, size_t len);

int main()
{
    int32_t data[10] = { 10, 30, 50, 0, 80, 20, 90, 70, 40, 60 };

    vqsort_avx2_i32_asc(data, 10);

    for (int i = 0; i < 10; i++) 
        printf("%"PRIi32" ", data[i]);

    return 0;
}
