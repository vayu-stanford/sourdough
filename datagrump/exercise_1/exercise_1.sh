#!/usr/bin/env sh
cp exercise_1/exercise_1.input.gnuplot.template exercise_1/exercise_1.input.gnuplot
#rm -f exercise_1/exercise_1.data

k=0

for i in 1 10 12 25 50 75 100 200 500
do
  sed -i "17s/=.*/ = $i;/" controller.cc
  make
  #for j in 1 2 3
  #do
    #./run-contest vayu 2>&1 | grep -E -w 'throughput|signal' | sed ':a;N;$!ba;s/\n/ /g' | cut -f 3,11 -d' ' >> exercise_1/exercise_1.data
  #done
  #echo "" >> exercise_1/exercise_1.data
  #echo "" >> exercise_1/exercise_1.data
  echo "\"exercise_1.data\" using 2:1 index $k title '$i' with points, \\" >> exercise_1/exercise_1.input.gnuplot
  k=$((k+1))
done

echo "" >> exercise_1/exercise_1.input.gnuplot
cd exercise_1
gnuplot < exercise_1.input.gnuplot
