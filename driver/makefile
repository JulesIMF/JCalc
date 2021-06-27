CXX		= g++
LD		= ld
INC		= ../includes
CFLAGS	= -O0 -g -c -I$(INC)
LFLAGS	= -r

OBJS	= drvdebug.o lexer.o vertex.o
TASK	= driver

$(TASK).o: $(OBJS)
	$(LD) $(LFLAGS) $^ -o $@
	rm $^

.cpp.o:
	$(CXX) $(CFLAGS) $<

clean:
	rm $(TASK).o