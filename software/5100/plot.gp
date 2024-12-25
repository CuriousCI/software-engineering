set terminal x11 title 'GNUPlot'

plot 'log.csv' using 1:2 with steps title 'state'
