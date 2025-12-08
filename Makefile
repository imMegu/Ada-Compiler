# --- Makefile Atualizado para a Parte 2 ---

OBJS=scanner.o parser.o ast.o symboltable.o linkedlist.o printAbsTree.o tac.o mips.o compiler.o
INTERM=scanner.c parser.c parser.h
PROGRAM=./compiler
CFLAGS=-g

all: $(PROGRAM)

# Gera o scanner em C a partir do Flex
scanner.c: scanner.flex parser.h
	flex -o scanner.c scanner.flex

# Gera o parser em C e o parser.h a partir do Bison
parser.c parser.h: parser.bison
	bison --defines=parser.h -o parser.c parser.bison
tac.o: tac.c tac.h parser.h
	$(CC) $(CFLAGS) -c tac.c -o tac.o

mips.o: mips.c mips.h tac.h symboltable.h
	$(CC) $(CFLAGS) -c mips.c -o mips.o

compiler.o: compiler.c parser.h tac.h mips.h
	$(CC) $(CFLAGS) -c compiler.c -o compiler.o

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJS)

clean:
	rm -f $(PROGRAM) $(OBJS) $(INTERM) *.asm

tests: $(PROGRAM)
	@mkdir -p assembly # Ensure the output directory exists
	@for f in examples/*; do \
		echo "Compiling $$f..."; \
		$(PROGRAM) $$f assembly/$$(basename $${f}).asm; \
	done

test:
	@echo "Usage: make test <example>"
	@false

test-%: $(PROGRAM)
	@echo "Compiling examples/$*..."
	$(PROGRAM) examples/$* assembly/$*.asm

