a.out: parsetest.o y.tab.o lex.yy.o errormsg.o ast.o past.o symbol.o table.o utils.o
	gcc -g parsetest.o y.tab.o lex.yy.o errormsg.o ast.o past.o symbol.o table.o utils.o

parsetest.o: parsetest.c errormsg.h utils.h past.h
	gcc -g -c parsetest.c

y.tab.o: y.tab.c
	gcc -g -c y.tab.c

y.tab.c: parser.y
	yacc -dv parser.y

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

errormsg.o: errormsg.c errormsg.h utils.h
	gcc -g -c errormsg.c

lex.yy.o: lex.yy.c y.tab.h errormsg.h utils.h
	gcc -g -c lex.yy.c

lex.yy.c: tiger.l
	lex tiger.l

past.o: past.c past.h
	gcc -g -c past.c

ast.o: ast.c ast.h
	gcc -g -c ast.c

symbol.o: symbol.c symbol.h
	gcc -g -c symbol.c

table.o: table.c table.h
	gcc -g -c table.c

utils.o: utils.c utils.h
	gcc -g -c utils.c

clean: 
	rm -f a.out utils.o parsetest.o lex.yy.o errormsg.o y.tab.c y.tab.h y.tab.o
