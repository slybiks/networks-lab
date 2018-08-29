#!/bin/bash 
find / ! -path "/*`basename "$PWD"`/*" -iname spot.c -type f 2>/dev/null -print -quit
