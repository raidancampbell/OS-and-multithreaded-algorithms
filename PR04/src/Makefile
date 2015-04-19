CC = gcc
# -g : allows use of GNU Debugger
# -Wall : show all warnings
# -std=gnu99 : I like declaring variables in the for loop's head
# -Wno-missing-braces : see GCC bug # 53119
# -Wno-unused-variable : these are all RPCGEN's fault

CFLAGS = -g -Wall -std=gnu99 -Wno-missing-braces -Wno-unused-variable
LIBS = # None yet...
CSOURCE = Client.c MailBox_clnt.c MailBox_xdr.c
SSOURCE = Server.c MailBox_svc.c MailBox_xdr.c
RPCSRC = MailBox.x
RPCGEN = MailBox.h MailBox_clnt.c MailBox_svc.c MailBox_xdr.c
CLIENT = client
SERVER = server

all: $(CLIENT) $(SERVER)

client: $(CSOURCE) $(RPCGEN)
	$(CC) $(LIBS) $(CFLAGS) -o $(CLIENT) $(CSOURCE)

server: $(SSOURCE) $(RPCGEN)
	$(CC) $(LIBS) $(CFLAGS) -o $(SERVER) $(SSOURCE)

$(RPCGEN): $(RPCSRC)
	rpcgen $(RPCSRC)

# 'clean' rule for remove non-source files
# To use, call 'make clean'
# Note: you don't have to call this in between every
# compilation, it's only in case you need to
# clean out your directory for some reason.
clean:
	@# Using the '@' sign suppresses echoing
	@# the line while the command is run
	rm -f $(CLIENT) $(SERVER) $(RPCGEN)
