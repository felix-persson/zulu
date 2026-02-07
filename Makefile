include config.mk

SRC = parser.tab.o lex.yy.c zulu.c node.c util.c
OBJ = ${SRC:%.c=%.o}

all: zulu

zulu: ${SRC}
	${CC} ${CFLAGS} $^ -o zulu

parser.tab.o: parser.tab.c
	${CC} ${CFLAGS} -c parser.tab.c

parser.tab.c: parser.y
	${YACC} parser.y

lex.yy.c: lexer.l parser.tab.c
	${LEX} lexer.l

clean:
	rm -f zulu ${OBJ} *.pdf parser.tab.* lex.yy.c

tree:
	dot -Tpdf tree.dot -o tree.pdf
