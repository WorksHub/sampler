CFLAGS = -O2 -fomit-frame-pointer
LDFLAGS = -lncurses -lm

# comment
all: julia

julia: julia.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
