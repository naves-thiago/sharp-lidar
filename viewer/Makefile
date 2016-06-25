bin_E=main
bin_O=main.o tty.o
bin_C=${bin_O:.o=.c}

CFLAGS=-march=native -std=gnu99 -O2 -Wall -pedantic -g -I /usr/local/include/
LDLIBS=-O2 -lm -g -lSDL2 -L /usr/local/include/

#PKG=sdl2 glew gl
#CFLAGS +=`pkg-config $(PKG) --cflags`
#LDLIBS +=`pkg-config $(PKG) --libs` -ldl

C=$(bin_C) $(perf_C)
O=$(bin_O) $(perf_O)

.PHONY: all clean
all: .depend.mk $(bin_E)
zip: Makefile $(SRC) $(HDR)
	zip -r mypackage.zip $^

run: bin_E
	./$(bin_E)

$(bin_E): $(bin_O)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS) -rdynamic
$(bin_O): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.depend.mk: $(C) Makefile
	$(CC) -MM $(CFLAGS) $(C) > .depend.mk
clean:
	rm -f $(bin_E) $(O)
-include .depend.mk
