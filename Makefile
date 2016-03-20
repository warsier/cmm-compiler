TEST_FILE_LIST = $(shell find ./test -name "*.cmm")
all:
	@flex lexical.l
	@gcc lex.yy.c -lfl -o scanner
	@git add -A --ignore-errors
test: all $(TEST_FILE_LIST)
	@rm -f log.txt
	@for TEST_FILE in $(TEST_FILE_LIST); do \
		echo "\n@@@ TESTFILE: $$TEST_FILE @@@" | tee -a log.txt; \
		./scanner $$TEST_FILE | tee -a log.txt;\
	done
clean:
	@rm -f scanner log.txt
commit:
	@make clean
	@read -p "Input commit message: " NAME; \
	git commit -m $$NAME;

