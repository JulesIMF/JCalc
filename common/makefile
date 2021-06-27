CXX		= g++
LD		= ld
INC		= ../includes
CFLAGS	= -O0 -g -c -I$(INC)
LFLAGS	= -r

OBJS	= messages.o colors.o
TASK	= common

$(TASK).o: $(OBJS)
	$(LD) $(LFLAGS) $^ -o $@
	rm $^

.cpp.o:
	$(CXX) $(CFLAGS) $<

clean:
	rm $(TASK).o