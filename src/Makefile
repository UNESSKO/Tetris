CC:=gcc
CFLAGS:= -std=c11 -ggdb -c -Wall -Wextra -Werror -lgcov --coverage -fprofile-arcs -ftest-coverage
OUTPUT_DIR := .
TESTNAME := s21_tests
WORKING_FILES := $(filter-out $(TESTNAME).c, *.c)
LIBNAME := s21_brick
UNAME := $(shell uname)
TESTDIR := tests
GCOVDIR := gcov_report
LIBRARY := $(LIBNAME).a

all: clean format_project $(LIBNAME).a gui dvi

$(LIBNAME).a: $(LIBNAME).o
	ar rcs $(LIBNAME).a *.o

$(LIBNAME).o:
	find ./brick_game/tetris/*.c ! -name "$(TESTNAME)" -exec $(CC) $(CFLAGS) {} \;

gui:
ifeq ($(UNAME),  Darwin)
else
	$(CC) -std=c11 -ggdb -no-pie -L. -o $(LIBNAME) gui/cli/*.c brick_game/tetris/*.c game.c -ggdb -fsanitize=address -lncursesw;
endif

clean:
	find ./../ -type f -name '*.gch'  			-delete
	find ./../ -type f -name '*.tar'  			-delete
	find ./../ -type f -name '*.o'    			-delete
	find ./../ -type f -name '*.a'      			-delete
	find ./../ -type f -name '*.gcda'      			-delete
	find ./../ -type f -name '*.gcov'      			-delete
	find ./../ -type f -name '*.gcno'      			-delete
	find ./../ -name '*.dSYM'      				-exec rm -rv {} +
	find ./../ -name 'doxygen_output'      			-exec rm -rv {} +
	find ./../ -type f -name '*.out'  			-delete
	find ./../ -type f -name 'vgcore*'  			-delete
	find ./../ -type f -name 'brick_hs'  			-delete
	rm -rf $(GCOVDIR)
	rm -rf $(LIBNAME)
	rm -rf $(TESTNAME)

retest: retest_no_start
	./$(TESTNAME)

#CLion debug only
retest_no_start: clean $(LIBNAME).a test

launch_gui: clean all
	./s21_brick

test: format_project
ifeq ($(UNAME),  Darwin)
	find ../src/brick_game/ -type f -name '*.c' -exec $(CC) --coverage -fprofile-arcs -ftest-coverage -c -g2 -Wall -Wextra -Werror  {} \;
	ar rcs $(LIBNAME).a *.o
	$(CC) $(CFLAGS) -o ./$(TESTNAME)  $(TESTDIR)/$(TESTNAME).c $(LIBNAME).a -lcheck -lpthread
else
#	find ../src/brick_game/ -type f -name '*.c' ! -name "$(TESTNAME).c" -exec $(CC) $(CFLAGS) --coverage -lgcov {} \;
	$(CC) $(CCFLAGS) *.o -o $(OUTPUT_DIR)/$(TESTNAME)  ../src/brick_game/tetris/tests/*.c -lcheck -lm -lpthread -lsubunit -lgcov -L. -ggdb
endif
	#./$(TESTNAME)

gcov_report: retest
	mkdir -p $(GCOVDIR)
	mv *.gcda $(GCOVDIR) #2>/dev/null; true
	mv *.gcno $(GCOVDIR) #2>/dev/null; true
	lcov --capture --directory $(GCOVDIR) --output-file $(GCOVDIR)/coverage.info
	genhtml $(GCOVDIR)/coverage.info --output-directory $(GCOVDIR)/html

dvi:
	#sudo apt-get update -y && sudo apt-get install texinfo graphviz doxygen texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra -y
	doxygen Doxyfile
	@sed -i 's/$$(LATEX_CMD) refman/$$(LATEX_CMD) refman || true/g' ./doxygen_output/latex/Makefile
	make -C ./doxygen_output/latex/ || true

install: clean $(LIBNAME).a gui

uninstall: clean

dist: clean
	tar -cf s21_brick_game.tar .

format_project:
	find . -iname '*.c' | xargs clang-format -i
	find . -iname '*.h' | xargs clang-format -i
	cppcheck --check-config --enable=all --suppress=missingIncludeSystem ./

.PHONY: all clean test gcov_report gui launch_gui
