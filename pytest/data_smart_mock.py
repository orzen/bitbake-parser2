#!/usr/bin/python3

import os
import re

VAR_PTN = re.compile(r'\$\{(\S+)\}', re.M)


class DataSmart:
    def __init__(self):
        self.values = {'BBPATH': '${TOPDIR}/pytest/test_data',
                       'TOPDIR': os.getcwd()}

    def _expand_vars(self, text):
        #matches = re.finditer(VAR_PTN, text, re.MULTILINE)
        matches = re.finditer(VAR_PTN, text)

        for matchNum, match in enumerate(matches, start=1):
            try:
                val = self.values[match.group(1)]
                text = text.replace(match.group(0), val)
            except KeyError:
                pass
        return text

    def getVar(self, var, expand=True):
        print("getVar '%s', ex '%d'" % (var, expand))
        try:
            val = self.values[var]
            if expand:
                return self._expand_vars(val)
            else:
                return self.values[var]
        except KeyError:
            return None

    def setVar(self, var, val):
        self.values[var] = val
