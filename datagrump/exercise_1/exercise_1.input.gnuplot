set term latex
set xrange [4000 to 0]
set output "exercise_1.latex"
set key outside
set xlabel "delay (ms)"
set ylabel "\\rotatebox{90}{throughput (Mbits/s)}" 

plot \
"exercise_1.data" using 2:1 index 0 title '1' with points, \
"exercise_1.data" using 2:1 index 1 title '10' with points, \
"exercise_1.data" using 2:1 index 2 title '12' with points, \
"exercise_1.data" using 2:1 index 3 title '25' with points, \
"exercise_1.data" using 2:1 index 4 title '50' with points, \
"exercise_1.data" using 2:1 index 5 title '75' with points, \
"exercise_1.data" using 2:1 index 6 title '100' with points, \
"exercise_1.data" using 2:1 index 7 title '200' with points, \
"exercise_1.data" using 2:1 index 8 title '500' with points, \

