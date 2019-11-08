py_mod = import('python')
py3 = py_mod.find_installation()
py3_dep = py3.dependency(required: true)

d_glib = dependency('glib-2.0')
d_python = dependency('python3')

deps = [d_glib, d_python]

headers = [
]

sources = [
	'bbcompat.c',
	'convert.c',
	'pylog.c',
	'pyo_ast.c',
	'pyo_bb_parse.c',
	'pyo_d.c',
	'pyo_match.c',
	'python_api.c',
	'python_utils.c'
]

py3.extension_module('bbc_parser',
                     sources,
                     include_directories: includes,
                     dependencies: deps)
