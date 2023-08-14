#
# Usage: gnuplot -c bench_plot.gp datafile
#

set dataf sep ","
set key autotitle columnhead

datafile = ARG1

set terminal epslatex
set output sprintf("%s.tex", datafile)

# Number of columns on file
N = system(sprintf("awk -F ',' 'NR==1{print NF}' %s", datafile))

# Plot all series (schemes)
plot for [i=2:N] datafile using 1:i with linespoints

set output
set terminal pop
