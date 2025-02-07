#! /usr/bin/python3
import sys

file = open("./html/prueba.html", "r")

content = file.read()

print(content, flush=True)

file.close()

sys.stdout.flush()

exit(0)