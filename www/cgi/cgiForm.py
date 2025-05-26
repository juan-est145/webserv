#! /usr/bin/python3

import cgi

form = cgi.FieldStorage()
input = form.getvalue("input", "No value")

message = f'''<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Document</title>
</head>
<body>
	<h1>Your input was {input}</h1>
</body>
</html>'''

print(message)
