set terminal x11 title 'GNUPlot'

plot for [col=2:11] 'log.csv' using 1:col with steps
