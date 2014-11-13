#!/bin/bash

aclocal
autoconf
automake --foreign --add-missing

echo "=================="
echo " Ready, now run :"
echo "    ./configure"
echo "    make"