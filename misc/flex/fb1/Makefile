fb1-5: fb1-5.l fb1-5.y
	bison -d fb1-5.y
	lex fb1-5.l
	cc -o $@ fb1-5.tab.c lex.yy.c -ll

clean:
	rm -f fb1-5 fb1-5.tab.c fb1-5.tab.h lex.yy.c