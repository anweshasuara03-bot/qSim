# Set the output to be a PNG image file
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'bank_performance_graph.png'

# Set the title and axis labels
set title 'Bank Performance: Average Customer Time vs. Number of Tellers'
set xlabel 'Number of Tellers'
set ylabel 'Average Time in Bank (minutes)'

# Add a grid
set grid

# Remove the legend/key
unset key

# Plot the clean data from 'plot_data.txt'
plot 'plot_data.txt' using 1:2 with linespoints