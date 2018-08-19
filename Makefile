CC=clang
INC=-Isrc -Isrc/tests
FLAGS=-g

bin:
	mkdir bin

.PHONY: clean
clean:
	rm -rf bin

bin/%: src/tests/%.c src/cfg.h bin
	$(CC) $(INC) $(FLAGS) $< -o $@  

tests: bin/test_float bin/test_int
	@echo "Built tests"
