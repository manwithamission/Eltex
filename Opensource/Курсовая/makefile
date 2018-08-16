server : server.o message.pb-c.c
	gcc -Wall server.o message.pb-c.c -lprotobuf-c -o server -pthread
		
server.o : server.c
	gcc -c server.c

clientsender : clientsender.o
	gcc -Wall clientsender.o message.pb-c.c -lprotobuf-c -o clientsender
		
clientsender.o : clientsender.c
	gcc -c clientsender.c

clientreciever : clientreciever.o
	gcc -Wall clientreciever.o message.pb-c.c -lprotobuf-c -o clientreciever

clientreciever.o : clientreciever.c
	gcc -c clientreciever.c