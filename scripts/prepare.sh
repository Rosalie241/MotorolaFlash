#!/usr/bin/env bash
set -e

# init all modules
git submodule update --init

# reset all modules
git submodule foreach --recursive 'git reset --hard'
git submodule foreach --recursive 'git clean -fdx'

# run all prepare scripts
for dep in ThirdParty/*
do
    if [ -f "./$dep/prepare.sh" ]
    then
        echo "-> $dep"
        ./$dep/prepare.sh ./$dep
    fi
done
