#!/usr/bin/env bash
set -e

pdir="$1"

for patch_dir in "$pdir/patches/"*
do
    dir="$(basename "$patch_dir")"
    
    for patch in "$pdir/patches/$dir"/*
    do
        echo "Applying $patch"
        patch -p1 -f -s -d "$pdir/$dir" < $patch
    done
done