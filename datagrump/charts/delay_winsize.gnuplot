set term png
set output "delay_windowsize.png"
#set key outside
set multiplot layout 2,1 
set xrange[0 to 43000]
set yrange[40 to 1000]
set logscale y
set xlabel "seqno"
set ylabel "\\rotatebox{90}{packet delay (ms)}" 
set ylabel "packet delay (ms)" 
set title "Delays and thresholds vs received ack seqno"

plot \
"delay_window_size" using 1:2 title 'delay' with lines,\
"delay_window_size" using 1:4 title 'upper-threshold' with lines,\
"delay_window_size" using 1:5 title 'lower-threshold' with lines

unset logscale y
set ylabel "window size" 
set yrange[0 to 100]
set title "Window size vs received ack seqno"
plot \
"delay_window_size" using 1:3 title 'window size' with lines axes x1y2
unset multiplot

