#!/bin/bash

# Build and run all tests
echo "======================================"
echo "Building and running all tests..."
echo "======================================"

# Navigate to the project directory
cd "$(dirname "$0")"

# Create logs directory if it doesn't exist
mkdir -p logs

# Clean first
echo "Cleaning previous builds..."
make clean

# Build the test executables
echo "Building test executables..."
make wall_transform_test
make visual_test

# Run the tests
echo -e "\n======================================"
echo "Running wall transformation test..."
echo "======================================" 
make test | tee logs/transform_test.log

echo -e "\n======================================"
echo "Running visual representation test..."
echo "======================================"
make run_visual_test | tee logs/visual_test.log

# Run the game with the test map
echo -e "\n======================================"
echo "Running the game with test map..."
echo "======================================"
make 
./tanks_game -g inputs/wall_transformation_test.txt

echo -e "\n======================================"
echo "Test summary in logs/transform_test.log and logs/visual_test.log"
echo "======================================"
