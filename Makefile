OBJS=scanner.o parser.o ast.o interpreter.o printAbsTree.o linkedlist.o symboltable.o
INTERM=scanner.c parser.c parser.h
PROGRAM=./interpreter
CFLAGS=-g 

all: $(PROGRAM)

scanner.c: scanner.flex parser.h
	flex -o scanner.c scanner.flex

parser.c parser.h: parser.bison
	bison --defines=parser.h -o parser.c parser.bison

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)

clean:
	rm -f $(PROGRAM) $(OBJS) $(INTERM)

test: $(PROGRAM)
	@for f in examples/example*.txt; do \
		echo "Running $$f..."; \
		$(PROGRAM) $$f; \
	done

test%: $(PROGRAM)
	@FILE=examples/$*; \
	if [ -f $$FILE ]; then \
		echo "Running $$FILE..."; \
		$(PROGRAM) $$FILE; \
	else \
		echo "Test $$FILE not found."; \
	fi
