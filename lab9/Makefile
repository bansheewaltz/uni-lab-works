t:
	test/testlab9_mac build/lab9
static:
	cppcheck --enable=warning,style,performance,portability,unusedFunction \
	--std=c99 --verbose --error-exitcode=1 --language=c -DMAIN=main -I include \
	include/*.h src/*.c test/*.c