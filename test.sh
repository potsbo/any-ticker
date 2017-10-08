#!/bin/bash

set -e

perform_test() {
  local file=$1
  shift
  ./life ticker $@ > /dev/null
  if diff <(sort any-ticker.life) <(sort ./test/ticker/$file); then
    echo "$file ok"
  else
    echo "$file failed"
    exit 1
  fi
}

echo "Ticker test cases"
perform_test 001
perform_test 002 -m "abcdefghijklmnopqrstuvwxyz"
perform_test 003 -s 187 -m "a" -f apple_logo

echo ""
echo "Finished successfully"
