CXX		= g++
LD		= ld
INC		= ../includes
CFLAGS	= -O2 -g -c -I$(INC)
LFLAGS	= -r

OBJS	= drvdebug.o lexer.o vertex.o parser.o
TASK	= driver

$(TASK).o: $(OBJS)
	$(LD) $(LFLAGS) $^ -o $@
	rm $^

.cpp.o:
	$(CXX) $(CFLAGS) $<

clean:
	rm $(TASK).o