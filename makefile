COMP= g++
FLAGS= -O2 -g

OBJS= obj/common.o obj/driver.o obj/main.o

all: $(OBJS)
	$(COMP) $(FLAGS) \
	$(OBJS) -o jcalc

obj/common.o: messages.c.o


.c.o:
	$(COMP) $(FLAGS) \
	-c -o $*.o $<
	mv $*.o obj/$*.o