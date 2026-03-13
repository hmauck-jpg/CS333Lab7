CC = gcc
DEBUG = -g
DEFINES = -DNOISY_DEBUG
LDFLAGS = -lncurses
DEPENDS = ASCIIlosaurus_world.h  
CFLAGS = $(DEBUG) -Wall -Wextra -Wshadow -Wunreachable-code \
	-Wredundant-decls -Wmissing-declarations \
	-Wold-style-definition -Wmissing-prototypes \
	-Wdeclaration-after-statement -Wno-return-local-addr \
	-Wunsafe-loop-optimizations -Wuninitialized -Werror \
	-Wno-unused-parameter $(DEFINES)
PROG1 = ASCIIlosaurus_server
PROG2 = ASCIIlosaurus_client
PROG3 = ASCIIlosaurus_world

all: $(PROG1) $(PROG2)

$(PROG1): $(PROG1).o  
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(PROG1).o: $(PROG1).c $(DEPENDS)
	$(CC) $(CFLAGS) -c $< 

$(PROG2): $(PROG2).o $(PROG3).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(PROG2).o: $(PROG2).c $(DEPENDS)
	$(CC) $(CFLAGS) -c $< 

$(PROG3).o: $(PROG3).c $(DEPENDS)
	$(CC) $(CFLAGS) -c $< 
 
clean cls:
	rm -f $(PROG1) $(PROG2) *.o *~ \#*

tar:
	tar cvfa ASCIIlosaurus_${LOGNAME}.tar.gz $(PROG1).c $(PROG2).c $(PROG3).c $(PROG3).h [mM]akefile

git-checkin:
	if [ ! -d .git ] ; then git init; fi
	git add *.[ch] ?akefile
	git commit -m "automatic commit"






