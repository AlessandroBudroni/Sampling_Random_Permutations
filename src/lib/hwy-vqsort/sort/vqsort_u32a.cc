// Copyright 2021 Google LLC
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "vqsort.h"  // VQSort

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "sort/vqsort_u32a.cc"
#include "../foreach_target.h"  // IWYU pragma: keep

// After foreach_target
#include "vqsort-inl.h"

HWY_BEFORE_NAMESPACE();
namespace hwy {
namespace HWY_NAMESPACE {

void SortU32Asc(uint32_t* HWY_RESTRICT keys, size_t num) {
  return VQSortStatic(keys, num, SortAscending());
}

// NOLINTNEXTLINE(google-readability-namespace-comments)
}  // namespace HWY_NAMESPACE
}  // namespace hwy
HWY_AFTER_NAMESPACE();

#if HWY_ONCE
namespace hwy {
namespace {
HWY_EXPORT(SortU32Asc);
}  // namespace

void VQSort(uint32_t* HWY_RESTRICT keys, size_t n, SortAscending) {
  HWY_DYNAMIC_DISPATCH(SortU32Asc)(keys, n);
}

}  // namespace hwy
#endif  // HWY_ONCE
