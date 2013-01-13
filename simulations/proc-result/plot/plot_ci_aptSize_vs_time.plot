# Plot the CI and the Number of joined nodes vs. time

# --------- Output
   set terminal postscript eps color
   set output "ci_aptSize_150s.eps"

#unset 
unset log y2

#set style data lines
set style data points
set style line 2 pt 1 lc 4
set style line 3 pt 2 lc 5
#set style line 1 pt 3 lc 4
#set style line 2 pt 4 lc 2

set datafile separator ","

#set grid layerdefault   linetype -1 linecolor rgb "gray"  linewidth 0.200,  linetype -1 linecolor rgb "gray"  linewidth 0.200
#set key inside center top vertical Right noreverse enhanced autotitles nobox

# ---------- Log scale -----------
   #set grid x y2
   set key center top title " "
   #set logscale xy
   #set log x   
   #set log x2

# ---------- Label -----------
   set title "Continuity Index"
   set xlabel "Time (second)"
   set ylabel "Continuitity Index"
   set y2label "Number of active peers"

# ---------- Range -----------
   set xrange [1.0 : 150.0]
   set x2range [1.0 : 150.0]
   set yrange [0.9 : 1.0]
   set y2range [0.0 : 10.0]

# --------- Tic -------------
   set ytics nomirror
   set y2tics
   set tics out

# --------- Scale ------------
   #set autoscale  y
   #set autoscale y2

plot "ci_150s.dat" using 1:2 title "Continuity Index" axes x1y1,\
"aptSize.dat" using 1:2 title "Number of Active Peers" axes x2y2

