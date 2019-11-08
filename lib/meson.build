flex = find_program('flex', required: true)
bison = find_program('bison', required: true)

if not flex.found()
	error('Meson failed to find flex')
endif

if not bison.found()
	error('Meson failed to find bison')
endif

lgen = generator(flex,
                 output: '@PLAINNAME@.yy.c',
                 arguments: ['-o', '@OUTPUT@', '@INPUT@'])
lfiles = lgen.process('lexer.l')

bgen = generator(bison,
                 output: ['@BASENAME@.tab.c', '@BASENAME@.tab.h'],
                 arguments: ['@INPUT@', '--defines=@OUTPUT1@', '--output=@OUTPUT0@'])
bfiles = bgen.process('parser.y')

d_glib = dependency('glib-2.0')

deps = [d_glib]

headers = [
	'args.h',
	'files.h',
	'log.h',
	'node.h'
	'parser_types.h'
]

sources = [
	'args.c',
	'files.c',
	'log.c',
	'node.c',
	headers,
	lfiles,
	bfiles
]

libparser = shared_library('libparser', sources, dependencies: deps)
