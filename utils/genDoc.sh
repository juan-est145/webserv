#! /bin/bash
head -c $1 /dev/urandom | tr -dc 'a-zA-Z0-9' > ./utils/scramble.txt