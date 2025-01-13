#!/usr/bin/env nu

let cc = "clang"
let flags = [-Wall -Wextra -g]
let files = ("./*.c" | into glob)

def main [] {
    exec $cc ...$flags $files -o ./huffman
}

