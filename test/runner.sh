#!/bin/sh

POSIXLY_CORRECT=1
export POSIXLY_CORRECT

TIDY='tidy --show-body-only 1 --quiet 1 --show-warnings 0'
SCRIPT="$1"
TESTDIR="$2"
PASSED=0
FAILED=0

abort() {
    echo "Error: $*"
    exit 1
}

test -f "$SCRIPT"   || abort "argument #1 invalid; not a file"
test -x "$SCRIPT"   || abort "argument #1 invalid; not executable"
echo "" | "$SCRIPT" || abort "argument #1 invalid; script failed to run"
test -d "$TESTDIR"  || abort "argument #2 invalid; not a directory"

for TEXT in "$TESTDIR"/*.text; do
    test -f "$TEXT" || abort "empty or invalid test directory"
    printf "$(basename "$TEXT" .text) ... "
    HTML=$(echo "$TEXT" | sed 's/\.text$/.html/')
    mkfifo .pipe
    $SCRIPT "$TEXT" | $TIDY > .pipe &
    DIFF=$($TIDY "$HTML" | diff .pipe -)
    if test "$?" = 0; then
        PASSED=$(expr $PASSED + 1)
        echo OK
    else
        FAILED=$(expr $FAILED + 1)
        echo FAILED
        printf "\n$DIFF\n\n"
    fi
    rm .pipe
done

printf "\n\n$PASSED passed; $FAILED failed.\n"
