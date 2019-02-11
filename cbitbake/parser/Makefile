PROG = cbitbake

BISON_SRC = parser.y
BISON_GEN = parser.tab.c parser.tab.h

LEX_SRC = lexer.l
LEX_GEN = lex.yy.c

SRCS = $(BISON_GEN) $(LEX_GEN) files.c main.c node.c utils.c

PKGS = glib-2.0

CFLAGS += -ggdb -O0 -Wall
CFLAGS += $(shell pkg-config --cflags $(PKGS))

LIBS += -lfl
LIBS += $(shell pkg-config --libs $(PKGS))

GENERATED=$(BISON_GEN) $(LEX_GEN)

all: $(LEX)
	gcc $(CFLAGS) $(SRCS) $(LIBS) -o $(PROG)

$(LEX): $(LEX_SRC) $(BISON_GEN)
	flex --bison-bridge --reentrant $(LEX_SRC)

$(BISON_GEN): $(BISON_SRC)
	bison --report=state -d $(BISON_SRC)

$(SETUP_PY): $(LEX)
	python3 setup.py build_ext

clean:
	rm $(GENERATED) $(PROG) parser.output
