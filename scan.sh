#! /bin/bash

find . -type f -name "*$1" -printf '%p %s\n'| sort -t"," -k 2 -n -r | head -$2 | awk '{sum+=$2;print}; END {print "Total size in bytes is: " sum}'
