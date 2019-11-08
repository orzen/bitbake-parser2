PROG = bb_cparser

BISON_SRC = parser.y
BISON_GEN = parser.tab.c parser.tab.h

LEX_SRC = lexer.l
LEX_GEN = lex.yy.c

#SRCS = $(BISON_GEN) $(LEX_GEN) \
#       convert.c pyobj_ast.c pyobj_d.c pyobj_match.c python_utils.c \
#       files.c main.c node.c parser_types.c utils.c

SRCS = $(BISON_GEN) $(LEX_GEN) \
       $(wildcard *.c)

PKGS = glib-2.0 python-3.7

CFLAGS += -ggdb -O0 -Wall
CFLAGS += $(shell pkg-config --cflags $(PKGS))

LIBS += -lfl
LIBS += $(shell pkg-config --libs $(PKGS))

GENERATED=$(BISON_GEN) $(LEX_GEN)

all: $(LEX) python
	gcc $(CFLAGS) $(SRCS) $(LIBS) -o $(PROG)

$(LEX): $(LEX_SRC) $(BISON_GEN)
	flex --bison-bridge --reentrant $(LEX_SRC)

$(BISON_GEN): $(BISON_SRC)
	bison --report=state -d $(BISON_SRC)

python: $(LEX)
	python3 setup.py build_ext

install:
	install $(shell find build/ -name "*.so") pytest/

uninstall:
	rm pytest/*.so

clean:
	rm $(GENERATED) $(PROG) parser.output
	rm -rf build
