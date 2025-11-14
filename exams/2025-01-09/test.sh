#!/bin/bash

testcases=()

for exercise in test-log/*; do
    for testcase in $exercise/*; do
        exercise_number=$(echo $exercise | grep -oEi "[0-9]+")
        if [ -d $testcase ]; then
            # cat $test_case/parameters.txt
            var=$(cat $testcase/parameters.txt)
            testcases+=("$var")
            # cat $testcase/summary.txt.log
        fi
    done
done

# TODO: based on current directory, choose which tests to execute?
# TODO: nah, in each directory there should be a "tests" file, and done
for testcase in testcases; do
    echo 
done
# echo "${testcases[@]}"

# cd test-log
# pwd
# ls
# echo $exercise
# ls $exercise
