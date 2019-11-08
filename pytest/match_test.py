#!/usr/bin/python3

import remock

print("REMOCK '{}'".format(dir(remock)))

match = remock.compile("foo")

A = {"type":"block", "foo":"bar", "bar": "baz"}
match.set(A)
