set terminal svg font "CaskaydiaCove NFM"
set output 'euler.svg'

set yr[0:120]
plot [0:10] x**2, for [i=0:3] sprintf("appr-%d.csv", i) title sprintf("delta %.2g", 1. / (i + 1))
