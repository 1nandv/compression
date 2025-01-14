#!/usr/bin/env nu

let cc = "clang"
let flags = [-Wall -Wextra -g]
let files = ("./*.c" | into glob)

def main [] {
    print "Usage: build.nu [subcommand]\n"
    print "Commands:"
    print " huffman   compile huffman algorithm"
    print " clean     remove obselete files"
    # todo: lz77/lz78 algorithm
    # todo: stress test ht
}

def "main huffman" [] {
    exec $cc ...$flags $files -o ./huffman
}

def "main clean" [] {
    rm vgcore* coredump* ./huffman
}
