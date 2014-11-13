#!/bin/bash

aclocal
autoconf
automake -a -c

echo "=================="
echo " Ready, now run :"
echo "    ./configure"
echo "    make"