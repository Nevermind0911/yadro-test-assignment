#!/bin/bash

EXEC="./build/dungeon_bot"
TESTS_DIR="tests"
PASS=0
FAIL=0

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

if [ ! -x "$EXEC" ]; then
    echo "Executable not found at $EXEC. Build first: cmake -B build && cmake --build build"
    exit 1
fi

for input in "$TESTS_DIR"/*-input.txt; do
    name="${input%-input.txt}"
    name="${name##*/}"
    expected="$TESTS_DIR/${name}-output.txt"

    if [ ! -f "$expected" ]; then
        echo "  skip $name (no expected output)"
        continue
    fi

    actual_stdout=$("$EXEC" "$input" 2>/dev/null)
    exit_code=$?

    if [ $exit_code -eq 0 ]; then
        actual="$(cat result.txt)"
    else
        actual="$actual_stdout"
    fi

    expected_content="$(cat "$expected")"

    if [ "$actual" = "$expected_content" ]; then
        echo -e "${GREEN}PASS${NC} $name"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}FAIL${NC} $name"
        diff <(echo "$expected_content") <(echo "$actual") | sed 's/^/    /'
        FAIL=$((FAIL + 1))
    fi
done

echo
echo "Passed: $PASS, Failed: $FAIL"
[ $FAIL -eq 0 ]
