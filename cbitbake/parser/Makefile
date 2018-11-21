PROG = cbitbake

BISON_SRC = bitbake.y
BISON_GEN = bitbake.tab.c bitbake.tab.h

LEX_SRC = bitbake.l
LEX_GEN = lex.yy.c

SRCS = $(BISON_GEN) $(LEX_GEN) args.c files.c parser_types.c utils.c utils2.c

PKGS = glib-2.0

CFLAGS += -ggdb -O0 -Wall
CFLAGS += $(shell pkg-config --cflags $(PKGS))

LIBS += -lfl
LIBS += $(shell pkg-config --libs $(PKGS))

GENERATED=$(BISON_GEN) $(LEX_GEN)

all: $(LEX)
	gcc $(CFLAGS) $(SRCS) $(LIBS) -o $(PROG)

$(LEX): $(LEX_SRC) $(BISON_GEN)
	flex $(LEX_SRC)

$(BISON_GEN): $(BISON_SRC)
	bison --report=state -d $(BISON_SRC)

clean:
	rm $(GENERATED) $(PROG)
