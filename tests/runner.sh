#!/usr/bin/env bash

SUCCESS="\033[0;32m✓\033[0m"
FAIL="\033[0;31mfailed\033[0m"

echo "Running Teeton tests"

for f in $(ls ttn); do
    file=${f%%.*}
    echo -n $file"... "

    if [ ! -f in/$file.in ]; then
        ../build/teeton ttn/$file.ttn | diff out/$file.out - > /dev/null && echo -e $SUCCESS || echo -e $FAIL
    else
        ../build/teeton ttn/$file.ttn < in/$file.in  | diff out/$file.out - > /dev/null && echo -e $SUCCESS || echo -e $FAIL
    fi
done
