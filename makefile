CXX		= g++
INC		= includes
CFLAGS	= -O2 -g -I$(INC)

OBJS= common/common.o driver/driver.o main.o cmd/cmd.o

jcalc: $(OBJS)
	$(CXX) $(CFLAGS) $^ -o $@

main.o:
	$(CXX) $(CFLAGS) -c main.cpp

%.task:
	(cd $(@D); make clean)
	make $*/$*.o

%.o:
	(cd $(@D); make)

clean:
	rm $(OBJS)



	