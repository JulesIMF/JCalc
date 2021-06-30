CXX		= g++
LD		= ld
INC		= ../includes
CFLAGS	= -O2 -g -c -I$(INC)
LFLAGS	= -r

OBJS	= exit.o handlecmd.o help.o
TASK	= cmd

$(TASK).o: $(OBJS)
	$(LD) $(LFLAGS) $^ -o $@
	rm $^

.cpp.o:
	$(CXX) $(CFLAGS) $<

clean:
	rm $(TASK).o