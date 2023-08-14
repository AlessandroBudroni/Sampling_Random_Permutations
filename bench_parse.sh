#! /bin/sh

# Usage: parse_bench operation [avx2]
#
# operation: Sample, Compose or Invert

PARAM_N="32 64 128 256 512"
BENCH_DIR_MAIN="bench_dir"

# parseBenchmarkOutput operation file
#
# Parse execution times of an operation within the given file.
# operation: Sample, Compose or Invert
parseBenchmarkOutput()
{
	awk -v op="$1" '$0 ~ op {flag=1; next} /cycles/ {if (flag==1) {print $5; exit}}' "$2"
}

# ------------------------------------------------------------------------------
# Main execution
# ------------------------------------------------------------------------------

if [ "$#" -lt 1 ]; then
	printf "Usage: %s operation [avx2]\n\n" "$0"
	printf "       operation = Sample | Compose | Invert\n"
	exit 1
fi

op="$1"
variant="$2"

# Get list of schemes (from the first parameter N)
n=$(echo "$PARAM_N" | awk '{print $1}')
dir="$BENCH_DIR_MAIN/param_n_$n"
case "$variant" in
avx2)
	schemes=$(ctest --test-dir "$dir" -N | awk '/benchmark_api/ && /avx2/ {print $3}')
	variant="_$variant"
	;;
*)
	schemes=$(ctest --test-dir "$dir" -N | awk '/benchmark_api/ && !/avx2/ {print $3}')
	variant=""
	;;
esac

# Data file
file="$BENCH_DIR_MAIN/benchmark_api$variant.dat"
# Data file - header
line="n"
prefix="benchmark_api_"
for scheme in $schemes; do
	# Replace '_' for ' '
	line="$line,$(echo ${scheme#"$prefix"} | tr '_' ' ')"
done
printf "%s\n" "$line" > $file
# Data file - data
for n in $PARAM_N; do
	line="$n"
	for scheme in $schemes; do
		bench_file="$BENCH_DIR_MAIN/param_${n}_$scheme"
		time=$(parseBenchmarkOutput "$op" "$bench_file")
		line="$line,$time"
	done
	printf "%s\n" "$line" >> $file
done
