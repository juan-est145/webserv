#! /usr/bin/python3
import sys

if len(sys.argv) > 1:
    htmlFilePath = sys.argv[1]  # Obtiene el argumento
else:
    htmlFilePath = '../html/index.html'  # Valor predeterminado


try:
    file = open(htmlFilePath, "r")

    content = file.read()

    print(content, flush=True)

    file.close()
except FileNotFoundError:
    print("404 Not Found file not found.")
    sys.stdout.flush()
    exit(404)
except PermissionError:
    print("403 Forbidden permission denied.")
    sys.stdout.flush()
    exit(403)
except Exception as e:
    print("500 Internal Server Error")
    sys.stdout.flush()
    exit(500)


sys.stdout.flush()
exit(0)