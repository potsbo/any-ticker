#$/bin/sh

set -e

perform_test() {
  ./life ticker ${2:""}
  diff any-ticker.life ./test/ticker/$1
}

perform_test 001
