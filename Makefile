CC:=gcc
LDFLAGS:=
LDLIBS:=-lpthread

all: test_scoped

test_scoped: test_scoped.o defer.o 
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f *.o defer

