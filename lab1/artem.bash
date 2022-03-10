#!/bin/bash
if [ $# -ne 2 ]
then 
	echo "Usage: command [str to find] [start dir]"
	exit
fi
if ! [ -d $2 ]
then 
	echo "Error: 2nd argument is [start dir]" >&2
	exit
fi
find $2 -type f -exec grep -l $1 {} \; -exec wc -c {} \;| sort -n | grep ^[0-9]


