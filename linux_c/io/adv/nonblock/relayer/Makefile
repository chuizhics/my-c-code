CFLAGS+=-pthread
LDFLAGS+=-pthread


all:relayer
relayer:relayer.o main.o
	gcc $^  $(CFLAGS)  -o $@ $(LDFLAGS)

clean:
	rm -rf *.o relayer