CC ?= gcc
CFLAGS ?= -Wall -O2
LDFLAGS ?=

VPATH=src

objects := builtin.o main.o parser.o cmd.o

.PHONY: clean

minishell: $(objects)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

builtin.o: builtin.c builtin.h cmd.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c parser.h cmd.h
	$(CC) $(CFLAGS) -c $<

parser.o: parser.c parser.h cmd.h
	$(CC) $(CFLAGS) -c $<

processus.o: cmd.c cmd.h builtin.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(objects)
