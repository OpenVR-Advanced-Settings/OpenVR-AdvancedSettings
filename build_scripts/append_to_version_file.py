#!/usr/bin/env python
from sys import argv
from os import path

def get_version_string():
    dir_path = path.dirname(path.realpath(__file__))
    with open(path.join(dir_path, "compile_version_string.txt"), "r") as file:
        contents = file.readline()
        return contents.strip('"')    
    
if __name__ == "__main__":
    current_version = get_version_string()
    new_version = current_version + "-" + argv[1]
    print("\"" + new_version + "\"")