#!/bin/bash

# Simple script to test wall transformation functionality
echo "======================================"
echo "Testing Wall Transformation Feature"
echo "======================================"

# Navigate to the project directory
cd "$(dirname "$0")"

# Create logs directory if it doesn't exist
mkdir -p logs

# Clean and build the tests
echo "Building wall tests..."
make clean >/dev/null 2>&1

# Build and run the basic wall test
echo -e "\n======================================"
echo "Running the simple wall test..."
echo "======================================"
make test_wall
./simple_wall_test | tee logs/wall_test.log

# Build and run the wall collision test
echo -e "\n======================================"
echo "Running the wall collision test..."
echo "======================================"
make test_wall_collision
./wall_collision_test | tee logs/wall_collision_test.log

# Build and run the wall boundary test
echo -e "\n======================================"
echo "Running the wall boundary test..."
echo "======================================"
make test_wall_boundary
./wall_boundary_test | tee logs/wall_boundary_test.log

echo -e "\n======================================"
echo "Building the game..."
echo "======================================"
make

# Verify wall transformation test map exists
if [ -f "inputs/wall_transformation_test.txt" ]; then
    echo -e "\n======================================"
    echo "Running the game with the test map..."
    echo "======================================"
    echo "To test the visual appearance of weak walls, observe the map and look for:"
    echo "1. Regular walls displayed as 🟩"
    echo "2. Weak walls displayed as 🧱" 
    echo "3. When a regular wall is hit once, it should change to 🧱"
    echo "Press any key to continue..."
    read -n 1

    ./tanks_game -g inputs/wall_transformation_test.txt
elif [ -f "inputs/input_weak_wall.txt" ]; then
    echo -e "\n======================================"
    echo "Running the game with the weak wall test map..."
    echo "======================================"
    echo "To test the visual appearance of weak walls, observe the map and look for:"
    echo "1. Regular walls displayed as 🟩"
    echo "2. Weak walls displayed as 🧱"
    echo "3. When a regular wall is hit once, it should change to 🧱"
    echo "Press any key to continue..."
    read -n 1

    ./tanks_game -g inputs/input_weak_wall.txt
else
    echo "Warning: Test map not found at inputs/wall_transformation_test.txt"
fi

echo -e "\n======================================"
echo "Test completed. Results in logs/wall_test.log"
echo "======================================" 
