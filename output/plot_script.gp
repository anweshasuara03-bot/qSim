# Set the output to be a PNG image file.
# The size is set to 800x600 pixels.
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'bank_performance_graph.png'

# Set the title for the graph and the labels for the X and Y axes.
# [cite_start]This is based on the assignment's requirement to plot average time vs. number of tellers[cite: 86].
set title 'Bank Performance: Average Customer Time vs. Number of Tellers (Single Queue)'
set xlabel 'Number of Tellers'
set ylabel 'Average Time in Bank (minutes)'

# Add a grid to make the graph easier to read.
set grid

# Since there is only one line of data, we can remove the legend/key.
unset key

# This is the main command. It tells gnuplot to:
# - Use the file 'data.txt'.
# - Use the first column (1) for the X-axis and the second column (2) for the Y-axis.
# - Draw the graph with both lines connecting the points and markers at each point ('with linespoints').
plot 'data.txt' using 1:2 with linespoints

# This command is good practice to properly close the output file.
set output