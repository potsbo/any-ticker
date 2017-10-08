#!/bin/bash

set -e

perform_test() {
  local file=$1
  shift
  ./life ticker $@ > /dev/null
  if diff any-ticker.life ./test/ticker/$file; then
    echo "$file ok"
  else
    echo "$file failed"
    exit 1
  fi
}

test_convert() {
  local number=$1
  shift
  rm -f newfile.life
  ./life convert ./test/convert/$number.rle > /dev/null
  if diff newfile.life ./test/convert/$number; then
    echo "$number ok"
  else
    echo "$number failed"
    exit 1
  fi
}

echo "Ticker test cases"
perform_test 001
perform_test 002 -m "abcdefghijklmnopqrstuvwxyz"
echo ""

echo "Convert test cases"
test_convert 001
echo ""

echo "Finished successfully"
