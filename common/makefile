CXX		= g++
LD		= ld
INC		= ../includes
CFLAGS	= -O2 -g -c -I$(INC)
LFLAGS	= -r

OBJS	= messages.o colors.o terminate.o
TASK	= common

$(TASK).o: $(OBJS)
	$(LD) $(LFLAGS) $^ -o $@
	rm $^

.cpp.o:
	$(CXX) $(CFLAGS) $<

clean:
	rm $(TASK).o