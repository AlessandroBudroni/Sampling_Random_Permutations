#!/bin/sh

VQSORT_DIR="external/vqsort"

HIGHWAY_SRC="$1"
# $HIGHWAY_SRC/hwy/
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
# $HIGHWAY_SRC/hwy/ops/
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
# $HIGHWAY_SRC/hwy/contrib/algo/
HIGHWAY_FILES_ALGO='copy-inl.h
'
# $HIGHWAY_SRC/hwy/contrib/sort/
HIGHWAY_FILES_SORT='order.h
shared-inl.h
sorting_networks-inl.h
traits128-inl.h
traits-inl.h
vqsort.h
vqsort-inl.h
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

rm -rf "${VQSORT_DIR:?}/hwy/"*

# Copy files from highway and fix includes
#
# hwy/
dir="hwy"
mkdir -p "$VQSORT_DIR/$dir"
printf '%s' "$HIGHWAY_FILES_ROOT" | while IFS='' read -r file; do
	sed -e 's|#include "hwy/ops/|#include "ops/|g' \
	    -e 's|#include "hwy/|#include "|g' \
	    -e 's|#define HWY_TARGET_INCLUDE "hwy/|#define HWY_TARGET_INCLUDE "|g' \
	    "$HIGHWAY_SRC/$dir/$file" > "$VQSORT_DIR/$dir/$file"
done
# hwy/ops/
dir="hwy/ops"
mkdir -p "$VQSORT_DIR/$dir"
printf '%s' "$HIGHWAY_FILES_OPS" | while IFS='' read -r file; do
	sed -e 's|#include "hwy/ops/|#include "|g' \
	    -e 's|#include "hwy/|#include "../|g' \
	    "$HIGHWAY_SRC/$dir/$file" > "$VQSORT_DIR/$dir/$file"
done
# hwy/contrib/algo/
dir="hwy/contrib/algo"
mkdir -p "$VQSORT_DIR/$dir"
printf '%s' "$HIGHWAY_FILES_ALGO" | while IFS='' read -r file; do
	sed -e 's|#include "hwy/|#include "../../|g' \
	    "$HIGHWAY_SRC/$dir/$file" > "$VQSORT_DIR/$dir/$file"
done
# hwy/contrib/sort/
dir="hwy/contrib/sort"
mkdir -p "$VQSORT_DIR/$dir"
printf '%s' "$HIGHWAY_FILES_SORT" | while IFS='' read -r file; do
	sed -e 's|#include "hwy/contrib/sort/|#include "|g' \
	    -e 's|#include "hwy/contrib/algo/|#include "../algo/|g' \
	    -e 's|#include "hwy/|#include "../../|g' \
	    -e 's|#define HWY_TARGET_INCLUDE "hwy/contrib/sort/|#define HWY_TARGET_INCLUDE "contrib/sort/|g' \
	    "$HIGHWAY_SRC/$dir/$file" > "$VQSORT_DIR/$dir/$file"
done
