CC = gcc
CFLAGS = -Wall -g -Isrc/ast -Isrc/symbol_table -Isrc/semantic -Isrc/codegen -Ibuild
LEX = flex
YACC = bison

BUILD = build
SRC = src

all: $(BUILD)/compiler

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/parser.tab.c $(BUILD)/parser.tab.h: $(SRC)/parser/parser.y | $(BUILD)
	$(YACC) -d -o $(BUILD)/parser.tab.c $(SRC)/parser/parser.y

$(BUILD)/lex.yy.c: $(SRC)/lexer/lexer.l $(BUILD)/parser.tab.h | $(BUILD)
	$(LEX) -o $(BUILD)/lex.yy.c $(SRC)/lexer/lexer.l

$(BUILD)/compiler: $(BUILD)/lex.yy.c $(BUILD)/parser.tab.c \
                    $(SRC)/ast/ast.c \
                    $(SRC)/symbol_table/symbol_table.c \
                    $(SRC)/semantic/semantic.c \
                    $(SRC)/codegen/codegen.c \
                    $(SRC)/main.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all clean
