#!/bin/bash

TIDY='tidy --show-body-only 1 --quiet 1 --show-warnings 0'
SCRIPT="$1"
TESTDIR="$2"
PASSED=0
FAILED=0

abort() {
    echo "Error: $*"
    exit 1
}

test -f "$SCRIPT"  || abort "argument #1 invalid; not a file"
test -x "$SCRIPT"  || abort "argument #1 invalid; not executable"
"$SCRIPT" <<< ""   || abort "argument #1 invalid; script failed to run"
test -d "$TESTDIR" || abort "argument #2 invalid; not a directory"

for TEXT in "$TESTDIR"/*.text; do
    test -f "$TEXT" || abort "empty or invalid test directory"
    printf "$(basename "$TEXT" .text) ... "
    HTML="${TEXT/%.text/.html}"
    DIFF=`diff <($SCRIPT "$TEXT" | $TIDY) <($TIDY "$HTML")`
    if test $? == 0; then
        ((PASSED++))
        echo OK
    else
        ((FAILED++))
        echo FAILED
        printf "\n$DIFF\n\n"
    fi
done

printf "\n\n$PASSED passed; $FAILED failed.\n"
