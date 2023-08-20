#! /bin/sh

# Usage: parse_bench <sp, cp, ip> [avx2]
#        parse_bench sui
#
#        where sp  = sample permutations,
#              cp  = compose permutations,
#              ip  = invert permutations,
#              sui = sample uniform integers

PARAM_N="32 64 128 256 512 1024 2048 4096 8192"
BENCH_DIR_MAIN="bench_dir"

# parseBenchmarkOutput operation file
#
# Parse execution times of an operation within the given file.
# operation: Sample, Compose or Invert
parseBenchmarkOutput()
{
	if [ -e "$2" ]; then
		awk -v op="$1" '$0 ~ op {flag=1; next} /cycles/ {if (flag==1) {print $(NF-1); exit}}' "$2"
	else
		echo ""
	fi
}

showUsage(){
	printf "Usage: %s <sp|cp|ip> [avx2]\n" "$(basename "$0")"
	printf "       %s sui\n\n" "$(basename "$0")"
	printf "where sp  = sample permutations\n"
	printf "      cp  = compose permutations\n"
	printf "      ip  = invert permutations\n"
	printf "      sui = sample uniform integers\n"
}

# ------------------------------------------------------------------------------
# Parse command line arguments
# ------------------------------------------------------------------------------

if [ "$#" -lt 1 ]; then
	showUsage
	exit 1
fi

case "$1" in
sp|sui)
	op="Sample" ;;
cp)
	op="Compose" ;;
ip)
	op="Invert" ;;
*)
	showUsage
	exit 1 ;;
esac

if [ -n "$2" ] && [ "$2" != "avx2" ]; then
	showUsage
	exit 1
fi

# ------------------------------------------------------------------------------
# Main execution
# ------------------------------------------------------------------------------

# Get list of schemes (from the first parameter N) and datafile name
n=$(echo "$PARAM_N" | awk '{print $1}')
dir="$BENCH_DIR_MAIN/param_n_$n"
case "$1" in
sp|cp|ip)
	if [ "$2" = "avx2" ]; then
		schemes=$(ctest --test-dir "$dir" -N | awk '/benchmark_api/ && /avx2/ {print $3}')
		variant="_$2"
	else
		schemes=$(ctest --test-dir "$dir" -N | awk '/benchmark_api/ && !/avx2/ {print $3}')
		variant=""
	fi
	file="$BENCH_DIR_MAIN/benchmark_api_$op$variant.dat"
	prefix="benchmark_api_"
	;;
sui)
	schemes=$(ctest --test-dir "$dir" -N | awk '/benchmark_uniform_sampling/ {print $3}')
	file="$BENCH_DIR_MAIN/benchmark_uniform_sampling.dat"
	prefix="benchmark_uniform_sampling_"
	;;
esac

# Data file - header
line="n"
for scheme in $schemes; do
	# Replace '_' for ' '
	line="$line,$(echo "${scheme#"$prefix"}" | tr '_' ' ')"
done
printf "%s\n" "$line" > "$file"
# Data file - data
for n in $PARAM_N; do
	line="$n"
	for scheme in $schemes; do
		bench_file="$BENCH_DIR_MAIN/param_${n}_$scheme"
		time=$(parseBenchmarkOutput "$op" "$bench_file")
		line="$line,$time"
	done
	printf "%s\n" "$line" >> "$file"
done
