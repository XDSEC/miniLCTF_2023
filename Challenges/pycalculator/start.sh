#!/bin/sh
# Add your startup script
echo $FLAG > /home/ctf/flag
export FLAG= 
# DO NOT DELETE
socat tcp-l:9999,fork,reuseaddr exec:./chall.py && /bin/sh
sleep infinity;
