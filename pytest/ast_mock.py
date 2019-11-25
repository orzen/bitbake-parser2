#!/usr/bin/python3

stats = {
'handleData': 0,
'handleMethod': 0,
'handlePythonMethod': 0,
'handleInherit': 0,
'handleInclude': 0,
'handleAddTask': 0,
'handleDelTask': 0,
'handleBBHandlers': 0,
'handleExportFuncs': 0,
'handleUnset': 0,
'handleUnsetFlag': 0,
'handleExport': 0}

def count(func):
    global stats
    stats[func.__name__] += 1

@count
def handleData(statements, filename, lineno, groupd):
    pass
@count
def handleMethod(statements, filename, lineno, func_name, body, python):
    pass
@count
def handlePythonMethod(statements, filename, lineno, funcname, modulename,body):
    pass
@count
def handleInherit(statements, filename, lineno, m):
    pass
@count
def handleInclude(statements, filename, lineno, m, force):
    pass
@count
def handleAddTask(statements, filename, lineno, m):
    pass
@count
def handleDelTask(statements, filename, lineno, m):
    pass
@count
def handleBBHandlers(statements, filename, lineno, m):
    pass
@count
def handleExportFuncs(statements, filename, lineno, m, classname):
    pass
@count
def handleUnset(statements, filename, lineno, m):
    pass
@count
def handleUnsetFlag(statements, filename, lineno, m):
    pass
@count
def handleExport(statements, filename, lineno, m):
    pass

#def parse(filename):
