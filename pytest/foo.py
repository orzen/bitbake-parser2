#!/usr/bin/python3

import bb_cparser

class StubD:
    values = {}
    def __init__(self):
        self.values['FILE'] = "foobar.bbclass"

    def getVar(self, var, expand=False):
        print("\nvar: '{}', expand '{}'\n".format(var, expand))
        if var == "FILE":
            return self.values[var]

    def setVar(self, var, value):
        self.values[var] = value


d = StubD()
filename = "abc.bb"
include = 1
ctx = {}

cbbparser.handle(ctx, filename, d, include)
