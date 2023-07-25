#!/bin/sh

VQSORT_DIR="src/lib/hwy-vqsort"

HIGHWAY_SRC="../highway"
# highway/hwy/
HIGHWAY_FILES_ROOT='base.h
cache_control.h
detect_compiler_arch.h
detect_targets.h
foreach_target.h
highway_export.h
highway.h
per_target.h
per_target.cc
targets.h
targets.cc
'
# highway/hwy/ops/
HIGHWAY_FILES_OPS='arm_neon-inl.h
arm_sve-inl.h
emu128-inl.h
generic_ops-inl.h
ppc_vsx-inl.h
rvv-inl.h
scalar-inl.h
set_macros-inl.h
shared-inl.h
tuple-inl.h
wasm_128-inl.h
wasm_256-inl.h
x86_128-inl.h
x86_256-inl.h
x86_512-inl.h
'
# highway/hwy/contrib/algo/
HIGHWAY_FILES_ALGO='copy-inl.h
'
# highway/hwy/contrib/sort/
HIGHWAY_FILES_SORT='order.h
shared-inl.h
sorting_networks-inl.h
traits128-inl.h
traits-inl.h
vqsort.h
vqsort.cc
vqsort_128a.cc
vqsort_128d.cc
vqsort_f16a.cc
vqsort_f16d.cc
vqsort_f32a.cc
vqsort_f32d.cc
vqsort_f64a.cc
vqsort_f64d.cc
vqsort_i16a.cc
vqsort_i16d.cc
vqsort_i32a.cc
vqsort_i32d.cc
vqsort_i64a.cc
vqsort_i64d.cc
vqsort-inl.h
vqsort_kv128a.cc
vqsort_kv128d.cc
vqsort_kv64a.cc
vqsort_kv64d.cc
vqsort_u16a.cc
vqsort_u16d.cc
vqsort_u32a.cc
vqsort_u32d.cc
vqsort_u64a.cc
vqsort_u64d.cc
'

rm -rf "${VQSORT_DIR:?}/"*

# Copy files from highway and fix includes
#
# hwy/
dir="$VQSORT_DIR"
mkdir -p "$dir"
printf '%s' "$HIGHWAY_FILES_ROOT" | while IFS='' read -r file; do
	sed -e 's|#include "hwy/ops/|#include "ops/|g' \
	    -e 's|#include "hwy/|#include "|g' \
	    -e 's|#define HWY_TARGET_INCLUDE "hwy/|#define HWY_TARGET_INCLUDE "|g' \
	    "$HIGHWAY_SRC/hwy/$file" > "$dir/$file"
done
# hwy/ops/
dir="$VQSORT_DIR/ops"
mkdir -p "$dir"
printf '%s' "$HIGHWAY_FILES_OPS" | while IFS='' read -r file; do
	sed -e 's|#include "hwy/ops/|#include "|g' \
	    -e 's|#include "hwy/|#include "../|g' \
	    "$HIGHWAY_SRC/hwy/ops/$file" > "$dir/$file"
done
# hwy/algo/
dir="$VQSORT_DIR/algo"
mkdir -p "$dir"
printf '%s' "$HIGHWAY_FILES_ALGO" | while IFS='' read -r file; do
	sed -e 's|#include "hwy/|#include "../|g' \
	    "$HIGHWAY_SRC/hwy/contrib/algo/$file" > "$dir/$file"
done
# hwy/sort/
dir="$VQSORT_DIR/sort"
mkdir -p "$dir"
printf '%s' "$HIGHWAY_FILES_SORT" | while IFS='' read -r file; do
	sed -e 's|#include "hwy/contrib/sort/|#include "|g' \
	    -e 's|#include "hwy/contrib/algo/|#include "../algo/|g' \
	    -e 's|#include "hwy/|#include "../|g' \
	    -e 's|#define HWY_TARGET_INCLUDE "hwy/contrib/sort/|#define HWY_TARGET_INCLUDE "sort/|g' \
	    "$HIGHWAY_SRC/hwy/contrib/sort/$file" > "$dir/$file"
done
