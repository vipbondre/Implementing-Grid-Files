# Implementing-Grid-Files

We have implemented the functionalities of the Grid files which is used for the faster storage and retrieval of the spatial data in the secondary memory.       

The storage of spatial data points happens according to dynamically changing grid structure. 

The range query is also implemented to retrieve these spatial data points efficiently with the help of grid file.

Instructions for compiling and runing:

1. Compile the c++ file by using command g++ filename.
	In our case, use this: g++ grid_files.cpp
	This will create a.out file.

2. Use command ./filename_of_generated_output_file
	In our case, use this: ./a.out

3. The code will generate output.txt file consisting of the answer of range query.