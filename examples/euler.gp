set terminal png size 600,400
set output 'euler.png'

plot [0:10] x**2, for [i=0:3] sprintf("approx-%d.csv", i) title sprintf("delta %.2f", 1. / (i + 1))
