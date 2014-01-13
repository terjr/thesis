#!/bin/sh

ODROID_IP=2001:700:300:2120:1864:b6ff:febf:3636

SOURCE=$1
SB=$(basename $SOURCE)
BINARY=$3
BB=$(basename $BINARY)


scp $SOURCE odroid@[$ODROID_IP]:/tmp/$SB
ssh odroid@$ODROID_IP gcc --static /tmp/$SB -o /tmp/$BB
scp odroid@[$ODROID_IP]:/tmp/$BB $BINARY
