#!/usr/bin/env python

from __future__ import print_function, with_statement
from sys import argv, exit, stdout
from re import match, compile

rxp = compile("int\\s+test_(.+)\\s*\\(\\)")
test_names = list()

def scan_sourcefile(fn):
    with open(fn) as fd:
        for line in fd.readlines():
            m = match(rxp, line)
            if m is None:
                continue
            test_names.append(m.groups()[0])

if __name__ == "__main__":
    dump_externals = False
    dump_mapping = False
    output = stdout
    args = argv[1:]
    for i, fn in enumerate(args):
        if fn == "--externals":
            dump_externals = True
            continue
        if fn == "--mapping":
            dump_mapping = True
            continue
        if fn == "-o":
            output = open(args[i+1], "w")
            continue
        scan_sourcefile(fn)
    if not test_names:
        print("usage: %s [--mapping | --externals] test_file1.c [test_file2.c [...]]" % argv[0])
        exit(1)
    test_names.sort()
    if dump_externals:
        for tn in test_names:
            output.write("int test_%s();\n" % tn)
    elif dump_mapping:
        for tn in test_names:
            output.write("{\"%s\", test_%s},\n" % (tn, tn))
    else:
        for tn in test_names:
            output.write("%s;" % tn)
    output.flush()
    output.close()
