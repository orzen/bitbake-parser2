glib = dependency('glib-2.0')

srcs = ['main.c']

executable('bb_cparser',
           srcs,
           include_directories: includes,
           dependencies: [glib],
           link_with: libparser)
