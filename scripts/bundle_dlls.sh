#!/bin/bash

#
# ./bundle_dlls.sh "./bin" "./test,exe" "/mingw53/bin"
#
BINDIR="$1"
EXE="$2"

paths=("$3")

function findAndCopyDLL() {
    for i in "${paths[@]}"
    do
        FILE="$i/$1"
        if [ -f $FILE ] && [ ! -f "$BINDIR/$1" ]; then
           cp $FILE $BINDIR
           echo "Found $1 in $i"
           copyForOBJ $FILE
           return 0
        fi
    done

    return 0
}

function copyForOBJ() {
    dlls=`objdump.exe -p $1 | grep 'DLL Name:' | sed -e "s/\t*DLL Name: //g"`
    while read -r filename; do
        findAndCopyDLL $filename
    done <<< "$dlls"
}

copyForOBJ $EXE
