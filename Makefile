CC             = gcc
LD             = ld
CFLAGS         = -ggdb -MD -fno-strict-aliasing -I./include -O2 -Wno-unused-result -fno-stack-protector -lfl -ly
CFILES         = $(shell find ./src -name "*.c")
OBJS           = $(CFILES:.c=.o)
TEST_FILE_LIST = $(shell find ./test -name "2.*.cmm")
SRC_DIR        = ./src

all:
	bison -d -v $(SRC_DIR)/syntax.y
	flex $(SRC_DIR)/lexical.l
	$(CC) $(CFILES) syntax.tab.c $(CFLAGS) -g -o parser
	@git add -A --ignore-errors

testtree:
	$(CC) ./test/test_treegen.c $(SRC_DIR)/syntax_tree.c -I./include -o out
	
test: all $(TEST_FILE_LIST)
	@rm -f log.txt
	@for TEST_FILE in $(TEST_FILE_LIST); do \
		echo "\n@@@ TESTFILE: $$TEST_FILE @@@" | tee -a log.txt; \
		./parser $$TEST_FILE 2>&1 | tee -a log.txt;\
	done
	
gdb: all $(TEST_FILE_LIST)
	@rm -f log.txt
	gdb --args ./parser ./test/2.06.cmm | tee -a log.txt

clean:
	@rm -f $(OBJS) $(OBJS:.o=.d)
	@rm -f scanner parser parser.d out lex.yy.c syntax.tab.c syntax.tab.h log.txt syntax.output
	@rm -f .fuse*
	
commit:
	@make clean
	@git add -A --ignore-errors
	@git commit -a
	@git push origin master

