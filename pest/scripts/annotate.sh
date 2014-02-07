#!/bin/sh
BINARY=$1
OBJDUMP=$(which arms-linux-gnueabi-objdump)
if [ -z "$OBJDUMP" ]
then
    OBJDUMP=$(find / -name 'arm-*-objdump' -print -quit 2>/dev/null )
fi

$OBJDUMP -S $BINARY | grep '<.*>:' | grep -v '<_' | tr -d '<>:'
