PROG=cbitbake

BISON_SRC=bitbake.y
BISON_GEN=bitbake.tab.c bitbake.tab.h

LEX_SRC=bitbake.l
LEX_GEN=lex.yy.c

SRCS=$(BISON_GEN) $(LEX_GEN) files.c

GENERATED=$(BISON_GEN) $(LEX_GEN)

all: $(LEX)
	gcc -ggdb -O0 $(SRCS) -lfl -o $(PROG)

$(LEX): $(LEX_SRC) $(BISON_GEN)
	flex $(LEX_SRC)

$(BISON_GEN): $(BISON_SRC)
	bison --report=state -d $(BISON_SRC)

clean:
	rm $(GENERATED) $(PROG)
