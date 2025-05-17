#!/bin/bash
#(echo -en "4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\n") | curl -v -X POST http://localhost:3000/upload   -H "Transfer-Encoding: chunked" --data-binary @-
#curl -v -X POST http://localhost:3000/upload -H "Transfer-Encoding: chunked" -d "4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\n"
curl -v -X POST http://localhost:3000/upload -H "Transfer-Encoding: chunked" -d "Wikipedia"