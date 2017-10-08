#$/bin/sh

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
  ./life ticker $@ > /dev/null
  ./life convert ./test/convert/$number
  if diff newfile.life any-ticker.life; then
    echo "$file ok"
  else
    echo "$file failed"
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
