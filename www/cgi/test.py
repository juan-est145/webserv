#! /usr/bin/python3

import os

message = f'''<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Document</title>
</head>
<body>
	<h1>Hello world</h1>
	<p>The HTTP_USER_AGENT env is {os.getenv('HTTP_USER_AGENT')}</p>
</body>
</html>'''

print(message)