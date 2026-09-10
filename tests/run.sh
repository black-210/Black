#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$ROOT"
make test >/dev/null
./black examples/mmio.black /tmp/black-test-mmio.c
cc -std=c11 -ffreestanding -fsyntax-only /tmp/black-test-mmio.c
cat >/tmp/black-test-bad.black <<'SRC'
kernel { fn main() { let x: u32 = missing; return; } }
SRC
if ./black /tmp/black-test-bad.black /tmp/black-test-bad.c 2>/dev/null; then
    echo "semantic failure: undefined identifier accepted" >&2
    exit 1
fi
echo "all tests passed"
