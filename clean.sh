#!/bin/bash

# Clean the project
echo "Removing datastructures..."
cd c_data_structures
make clean
echo "Done removing datastructures."
echo "Removing webserver library..."
cd ../
make clean
echo "Done removing webserver library."