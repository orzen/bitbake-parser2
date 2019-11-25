py_mod = import('python')
py3 = py_mod.find_installation()
py3_dep = py3.dependency(required: true)

d_glib = dependency('glib-2.0')
d_python = dependency('python-3.7-dbg')

deps = [d_glib, d_python]

headers = [
]

sources = [
	'bbcompat.c',
	'convert.c',
	'pylog.c',
	'pyo_ast2.c',
	'pyo_bb_parse.c',
	'pyo_d.c',
	'pyo_match.c',
	'python_api.c',
	'python_utils.c'
]

py3.extension_module('bbcparser',
                     sources,
                     include_directories: includes,
                     link_whole: [libparser],
                     dependencies: deps,
                     c_args: ['-O0', '-ggdb3'])
