#!/usr/bin/env bash
set -e

pdir="$1"

for patch in "$pdir/patches/"*
do
    echo "Applying $patch"
    patch -p1 -f -s -d "$pdir/tinyxml2" < $patch
done
