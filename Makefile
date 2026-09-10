CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Iinclude

SOURCES = src/main.c src/compiler.c src/lexer.c src/parser.c src/semantic.c src/codegen.c src/ast.c src/util.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean test install
all: black

black: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

src/%.o: src/%.c include/black.h src/black_internal.h
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

test: black
	./black examples/mmio.black /tmp/black-mmio.c
	$(CC) -std=c11 -ffreestanding -fsyntax-only /tmp/black-mmio.c
	./black examples/spec.black /tmp/black-spec.c
	$(CC) -std=c11 -ffreestanding -fsyntax-only /tmp/black-spec.c
	@echo "Black tests passed"

clean:
	rm -f black $(OBJECTS)

install: black
	install -m 755 black /usr/local/bin/black

# Architecture-specific final images are intentionally outside the core compiler.
# Example: use the generated C with your kernel's freestanding cross compiler and linker script.
