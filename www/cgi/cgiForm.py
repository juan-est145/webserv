#! /usr/bin/python3

import cgi

form = cgi.FieldStorage()
input = form.getvalue("input", "No value")

print("<body>")
print(" ")
print(f"<h1>Your input was {input}</h1>")
print(" ")
print("</body>")
