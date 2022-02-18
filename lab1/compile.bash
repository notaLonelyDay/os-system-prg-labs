#!/bin/env bash
src=$1
dst=$2

# Check number of arguments
if [ $# -ne 2 ]; then
	{
		echo "Invalid number of args"
		echo "You should use syntax \"compile.bash src dst\""
		echo "Use args:"
		echo "\$1 - file to compile"
		echo "\$2 - output file"
	}>&2
	exit
fi

if [ -f $src ]; then
	gcc $src -o $dst && ./$dst
	if [ $? -ne 0 ]; then
		echo "Error: Compilation failed!">&2
	else
		echo " Compilation succeded!"
	fi
else
		echo "Error: File $src didn't exist!" >&2
fi
