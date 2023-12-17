#!/bin/bash

# Build the project
echo "Building necessary datastructures..."
cd c_data_structures
make all
cd ../
echo "Done building datastructures."
echo "Building the webserver library..."
make all
echo "Done building webserver library."
