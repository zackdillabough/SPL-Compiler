#!/bin/bash

# set -x
# trap read debug

if [ ! -d "./testcases" ]; then
    echo "Error: tests directory not found!"
    exit 1
fi

if [ ! -e "./a.out" ]; then
    echo "Error: a.out not found!"
    exit 1
fi

if [ ! -x "./a.out" ]; then
    echo "Error: a.out not executable!"
    exit 1
fi

usage()
{
    echo
    echo "Usage: $0 n"
    echo
    echo "Where n is the desired task number in range [1..5]"
    echo
    exit 1
}

# if no arguments are specified
#if [ "$#" -lt "1" ]; then
testNum=-1

if [ "$#" -ge "1" ]; then
    testNum=$1
fi

let count=0
let all=0

mkdir -p ./output

for test_file in $(find ./testcases -type f -name "*.txt" | sort); do
    all=$((all+1))

    if [[ $testNum -eq -1 || $testNum -eq $all ]]; then
        name=`basename ${test_file} .txt`
        expected_file=${test_file}.expected
        output_file=./output/${name}.output
        diff_file=./output/${name}.diff
        ./a.out < ${test_file} > ${output_file}
        diff -Bw ${expected_file} ${output_file} > ${diff_file}
        echo
        echo "Test $all: ${name}"
        if [ -s ${diff_file} ]; then
            echo "Output does not match expected:"
            echo "--------------------------------------------------------"
            cat ${diff_file}
        else
            count=$((count+1))
            echo "OK"
        fi
        echo "========================================================"
        rm -f ${output_file}
        rm -f ${diff_file}
    fi
done

echo 

rmdir ./output
