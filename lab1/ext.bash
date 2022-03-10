#!/bin/env bash
out=$1
dir=$2
ext=$3

if [ "$#" -ne 3 ]; then
	{
		echo "Invalid number of args"
		echo "You should use syntax \"ext.bash out dir ext\""
		echo "Use args:"
		echo "\$1 - output file"
		echo "\$2 - directory to search in"
		echo "\$3 - extension to find"
	}>&2
	exit
fi

if [ ! -d "$dir" ]; then
	{
		echo "Invalid argument"
		echo "$2 - is not a directory"
	}>&2
	exit
fi

find $dir -name "*.$ext" -type f  -printf "%f\n" | sort -f -o "$out"
