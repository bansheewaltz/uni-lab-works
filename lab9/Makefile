t:
	test/testlab9_mac build/lab9
static:
	cppcheck --enable=warning,style,performance,portability,unusedFunction \
	--std=c99 \
	--verbose \
	--error-exitcode=1 \
	--suppressions-list=test/suppressions_local.txt \
	--language=c \
	-DMAIN=main \
	-I include \
	src/*.c test/*.c
# --inline-suppr
#--suppress=unusedFunction:src/debug.c:46
container:
	bash test/run_docker_image.sh
