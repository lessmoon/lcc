#!/bin/bash

xsource="testcase/s"
target_code="testcase/code.cpp"
target_err="testcase/err.info"
mycc="./mycc"

while [[ $# -gt 0 ]]; do
    if [[ $1 == -o ]]; then
        shift
        target_code=$1
    elif [[ $1 == -e ]]; then
        shift
        target_err=$1
    else
        xsource=$1;
    fi
    shift
done


cat $xsource | $mycc 1>$target_code 2>$target_err

printf "Finished!\n"
exit
