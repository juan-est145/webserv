#! /usr/bin/python3
import sys

if len(sys.argv) > 1:
    htmlFilePath = sys.argv[1]  # Obtiene el argumento
else:
    htmlFilePath = '../html/prueba.html'  # Valor predeterminado



file = open(htmlFilePath, "r")

content = file.read()

print(content, flush=True)

file.close()

sys.stdout.flush()

exit(0)