#$/bin/sh

set -e

./life ticker

diff any-ticker.life ./test/ticker/001
