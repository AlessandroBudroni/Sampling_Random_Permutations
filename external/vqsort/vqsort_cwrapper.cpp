#include "vqsort_cwrapper.h"

void vqsort_avx2_u16_asc(uint16_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortAscending{});
}

void vqsort_avx2_u16_des(uint16_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortDescending{});
}

void vqsort_avx2_u32_asc(uint32_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortAscending{});
}

void vqsort_avx2_u32_des(uint32_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortDescending{});
}

void vqsort_avx2_u64_asc(uint64_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortAscending{});
}

void vqsort_avx2_u64_des(uint64_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortDescending{});
}

void vqsort_avx2_i16_asc(int16_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortAscending{});
}

void vqsort_avx2_i16_des(int16_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortDescending{});
}

void vqsort_avx2_i32_asc(int32_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortAscending{});
}

void vqsort_avx2_i32_des(int32_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortDescending{});
}

void vqsort_avx2_i64_asc(int64_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortAscending{});
}

void vqsort_avx2_i64_des(int64_t* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortDescending{});
}

void vqsort_avx2_float_asc(float* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortAscending{});
}

void vqsort_avx2_float_des(float* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortDescending{});
}

void vqsort_avx2_double_asc(double* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortAscending{});
}

void vqsort_avx2_double_des(double* data, size_t len)
{
    hwy::VQSort(data, len, hwy::SortDescending{});
}