#
# Usage: gnuplot -c bench_plot.gp datafile
#

datafile = ARG1

set terminal tikz size 16cm,11cm
set output sprintf("%s.tex", datafile)

set dataf sep ","

set key top left autotitle columnhead

set xlabel "N"
set ylabel "Time (clock cycles)"

set log x 2
set format x "$2^{%L}$"

set log y 10

# Number of columns in the data file
N = system(sprintf("awk -F ',' 'NR==1{print NF}' %s", datafile))

# Plot all series (columns)
plot for [i=2:N] datafile using 1:i with linespoints lw 2 ps 1.5

#pause -1
set output
set terminal pop
