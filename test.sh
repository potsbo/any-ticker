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

echo "Ticker test cases"
perform_test 001
perform_test 002 -m "abcdefghijklmnopqrstuvwxyz"

echo ""
echo "Finished successfully"
