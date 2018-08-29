#!/bin/bash 
stat $(find / ! -path "/*`basename "$PWD"`/*" -iname spot.c -type f 2>/dev/null -print -quit) -c %y 
