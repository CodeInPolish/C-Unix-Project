CFLAGS = -D_DEFAULT_SOURCE -std=c11 -pedantic -Wvla -Wall -Werror

all : client gstat maint server 

client : client.o fileIO.o sockets.o check.o
	cc $(CFLAGS) client.o fileIO.o sockets.o check.o -o client

gstat : gstat.o ipc.o check.o
	cc $(CFLAGS) gstat.o ipc.o check.o -o gstat

maint : maint.o ipc.o check.o
	cc $(CFLAGS) maint.o ipc.o check.o -o gstat

server : server.o fileIO.o sockets.o ipc.o check.o
	cc $(CFLAGS) server.o fileIO.o sockets.o ipc.o check.o -o server

client.o : client.c global.h fileIO.h sockets.h
	cc $(CFLAGS) -c client.c 

gstat.o : gstat.c global.h ipc.h
	cc $(CFLAGS) -c gstat.c

maint.o : maint.c global.h ipc.h
	cc $(CFLAGS) -c maint.c

server.o : server.c global.h fileIO.h sockets.h ipc.h
	cc $(CFLAGS) -c server.c

check.o : check.c check.h
	cc $(CFLAGS) -c check.c

clean:
	rm -f *.o
	rm -f client
	rm -f gstat
	rm -f maint
	rm -f server
	ipcrm -a