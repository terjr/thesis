#!/bin/sh
BEST=$(cat ~/ga-results.* | tr -d ",'[]" | sort -n -k32 -u -r | grep -v '^$' | tail -n1 )
echo $BEST | xargs -n2

