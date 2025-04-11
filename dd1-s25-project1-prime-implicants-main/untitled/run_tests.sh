#!/bin/bash

INPUT_DIR="./input_tests"
OUTPUT_DIR="./expected_outputs"
EXEC="./cmake-build-debug/untitled.exe"

# Check if the executable exists
if [ ! -f "$EXEC" ]; then
    echo "Executable not found. Please build your project first."
    exit 1
fi

# Function to run a single test
run_test() {
    input_file="$1"
    base_name=$(basename "$input_file" .txt)
    expected_output="$OUTPUT_DIR/output${base_name/input/}.txt"

    if [ ! -f "$expected_output" ]; then
        echo "❌ Expected output file $expected_output not found!"
        return
    fi

    # Run the program and capture the last non-empty line after "Final expressions with letters are:"
    actual_output=$("$EXEC" "$input_file")

    # Extract everything after "Final expressions with letters are:"
    expressions=$(echo "$actual_output" | sed -n '/Final expressions with letters are:/,$p' | sed '1d')

    # Read and trim expected output
    expected=$(<"$expected_output")

    # Trim both actual and expected to avoid issues with trailing spaces
    expressions_trimmed=$(echo "$expressions" | sed 's/[[:space:]]*$//')
    expected_trimmed=$(echo "$expected" | sed 's/[[:space:]]*$//')

    if [ "$expressions_trimmed" == "$expected_trimmed" ]; then
        echo "✅ $base_name passed"
    else
        echo "❌ $base_name failed"
        echo "  Expected: $expected_trimmed"
        echo "  Got     : $expressions_trimmed"
    fi
}

export -f run_test
export EXEC OUTPUT_DIR

# Run tests in parallel (up to 8 at a time)
find "$INPUT_DIR" -name '*.txt' | xargs -P 8 -I {} bash -c 'run_test "$@"' _ {}
