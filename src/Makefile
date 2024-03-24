.PHONY: all test gcov_report clean valgrind_check leaks style
CC=gcc -std=c11 -D_GNU_SOURCEs
CFLAGS=-c -Wall -Wextra -Werror
BUILD_DIR=build
INSTALL_DIR=3d_View
APP=3d_View
GCOV=-fprofile-arcs -ftest-coverage
LIBC:=./sources/parser.c ./sources/affine.c
LIBCP:=./sources/parser.c
LIBH:=./headers/parser.h
LIBO:=./parser.o ./affine.o
TESTC=./tests/unit_tests.c
LIBS=-lcheck
LINUX_FLAGS=-lsubunit -lrt -lpthread -lm
OPEN_CMD = xdg-open

OS=$(shell uname)

ifeq ($(OS), Linux)
	OS_FLAGS=-lcheck -lsubunit_test -lrt -lpthread -lm
	CC+=-D OS_LINUX -g -s
	DIR=
	QTFLAGS=-spec linux-g++
	CHECK_LEAKS=CK_FORK=no valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=log.txt
	OPEN=firefox
else
	OS_FLAGS=-lcheck
	CC+=-D OS_MAC
	DIR=/$(APP).app/Contents/MacOS/
	QTFLAGS=
	CHECK_LEAKS=CK_FORK=no leaks --atExit --
	OPEN=open
endif

all: build

build: 
	qmake -o $(BUILD_DIR)/Makefile ./$(APP).pro $(QTFLAGS) CONFIG+=qtquickbuilder
	make -C $(BUILD_DIR)/ first

rebuild: clean all

install: build
	mkdir -p $(INSTALL_DIR)
	cp -r $(BUILD_DIR)$(DIR)/$(APP) $(INSTALL_DIR)/$(APP)

dist: install
	tar -czvf $(INSTALL_DIR).tgz $(INSTALL_DIR)/

uninstall:
	rm -rf $(INSTALL_DIR)

parser.a:
	$(CC) $(CFLAGS) $(LIBC)
	ar -rcs parser.a $(LIBO)
	ranlib $@
	cp $@ lib$@
	rm -rf *.o

test: clean parser.a
	$(CC) $(CFLAGS) $(TESTC)
	$(CC) ./unit_tests.o parser.a -o Test $(OS_FLAGS)
	./Test
	rm -rf *.o *.a ./Test

gcov_report: $(LIBC) $(TESTC) $(LIBH)
	$(CC) $(TESTC) $(LIBC) -o Test $(OS_FLAGS) $(GCOV)
	./Test
	lcov -c -d . -o coverage.info
	genhtml coverage.info -o coverage
	$(OPEN) ./coverage/index.html

valgrind_check: test
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./Test

	
leaks: clean parser.a
	$(CC) $(TESTC) parser.a $(OS_FLAGS) -o ./Test
	$(CHECK_LEAKS) ./Test
	rm -f *.o *.a Test

style:	
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -style=Google -n ./sources/*.c ./headers/*.h ./sources/*.cpp ./headers/*.h
	rm -rf .clang-format

cppcheck:
	cppcheck $(LIBC) $(LIBH)

clean:
	rm -rf *.o *.a *.out *.txt *.gcno *.gch *.gcda *.info *.tgz coverage Test $(BUILD_DIR) $(INSTALL_DIR) 
	rm -rf build* 
	rm -rf ../build*
	rm -rf 3d_View.pro.*

add_obj:
	@rm -rf obj
	@rm -rf obj/.DS_Store obj/.git