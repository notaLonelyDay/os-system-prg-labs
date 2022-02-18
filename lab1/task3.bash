#!/bin/env bash
string=$1
dir=$2

if [ "$#" -ne 2 ]; then
	{
		echo "Invalid number of args"
		echo "You should use syntax \"task3.bash str dir\""
		echo "Use args:"
		echo "\$1 - string to search"
		echo "\$2 - directory to search in"
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

grep -r "$dir" -e "$string" -l | xargs ls -al | sort -k 5 -n | awk '{print $5,$9}'