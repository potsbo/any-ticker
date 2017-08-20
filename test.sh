#$/bin/sh

set -e

perform_test() {
  local file=$1
  shift
  ./life ticker $@
  diff any-ticker.life ./test/ticker/$file
}

perform_test 001
perform_test 002 -m "abcdefghijklmnopqrstuvwxyz"
