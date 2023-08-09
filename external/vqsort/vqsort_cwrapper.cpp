#include "vqsort_cwrapper.h"
#include "hwy/contrib/sort/vqsort.h"

void vqsort_avx2_u32_asc(uint32_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortAscending());
}

void vqsort_avx2_u64_asc(uint64_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortAscending());
}