TEST_FILE_LIST = $(shell find ./test -name "*.cmm")
SRC_DIR = ./src

bison:
	bison -d -v $(SRC_DIR)/syntax.y
	flex $(SRC_DIR)/lexical.l
	gcc $(SRC_DIR)/main.c syntax.tab.c -lfl -ly -o parser
	@git add -A --ignore-errors
	
flex:
	@flex $(SRC_DIR)/lexical.l
	@gcc $(SRC_DIR)/mainf.c lex.yy.c -lfl -o scanner
	@git add -A --ignore-errors
	
test: bison $(TEST_FILE_LIST)
	@rm -f log.txt
	@for TEST_FILE in $(TEST_FILE_LIST); do \
		echo "\n@@@ TESTFILE: $$TEST_FILE @@@" | tee -a log.txt; \
		./parser $$TEST_FILE | tee -a log.txt;\
	done

# if you want to test the function of flex, you should modify ./src/lexical.l to enable output
testf: flex $(TEST_FILE_LIST)
	@rm -f log.txt
	@for TEST_FILE in $(TEST_FILE_LIST); do \
		echo "\n@@@ TESTFILE: $$TEST_FILE @@@" | tee -a log.txt; \
		./scanner $$TEST_FILE | tee -a log.txt;\
	done
	
clean:
	@rm -f scanner parser lex.yy.c syntax.tab.c syntax.tab.h log.txt syntax.output
	
commit:
	@make clean
	@git add -A --ignore-errors
	@git commit -a
	@git push origin master

