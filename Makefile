BIN=Gallery
CC=gcc
OBJDIR=objs
LIBS=glm glfw3
CFLAGS=-Wall -Wextra -DGL_SILENCE_DEPRECATION -g $(shell pkg-config --cflags $(LIBS)) -I$(OBJDIR) -Isrc
LDFLAGS=$(shell pkg-config --libs $(LIBS)) -framework OpenGL -framework Cocoa -framework IOKit -lm
SRC=$(shell find src -name '*.c' -type f)
OBJ=$(patsubst %.c,$(OBJDIR)/%.o, $(SRC))

.SILENT: $(OBJ) $(BIN) $(OBJDIR) compile_flags

all: $(OBJDIR) $(BIN)

$(OBJDIR):
	[ -d $@ ] || mkdir -p $@

$(BIN): $(OBJ)
	printf "  LD %s\n" $@
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.c %.h
	mkdir -p $(dir $@)
	printf "  CC %s\n" $@
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	printf "  CC %s\n" $@
	$(CC) $(CFLAGS) -c $< -o $@

compile_flags:
	echo $(CFLAGS) | tr ' ' '\n' > compile_flags.txt

clean:
	rm -rf $(BIN) $(OBJDIR)

run: all
	./Gallery

.PHONY: all clean run compile_flags
