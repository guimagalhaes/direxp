TARGET	 = teste
CC	 = g++
CFLAGS	 = -Wall -DLINUX
INCLUDES = -I..
LDLIBS	 = -ldirexp -L.. -L../../format_string -lformat_string
OBJS	 = teste.o

ifdef RELEASE
CFLAGS += -O2
else
CFLAGS += -g -DDEBUG
endif

all:	$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDLIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $*.o $*.cpp

clean:
	-rm -rf $(OBJS) *~ core $(TARGET) ./new "./new dir"

exec:
	LD_LIBRARY_PATH=../:../../format_string ./$(TARGET)

leak_check:
	LD_LIBRARY_PATH=../:../../format_string valgrind --leak-check=yes --show-reachable=yes -v ./$(TARGET)

debug:
	LD_LIBRARY_PATH=../:../../format_string gdb ./$(TARGET)
