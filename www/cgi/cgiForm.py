#! /usr/bin/python3
import sys
import os

content_length = os.getenv("CONTENT_LENGTH")
print(f"Starting reading, content length is {content_length}")
if content_length is not None:
    content_length = int(content_length)
    input_data = sys.stdin.read(content_length)
    print(f"The input is: {input_data}")
else:
    print("CONTENT_LENGTH is not set.")
print("We have finsished reading")