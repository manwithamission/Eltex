main : server.o clientsender.o clientreciever.o message.pb-c.c
	gcc -Wall server.o message.pb-c.c -lprotobuf-c -o server -pthread
	gcc -Wall clientsender.o message.pb-c.c -lprotobuf-c -o clientsender
	gcc -Wall clientreciever.o message.pb-c.c -lprotobuf-c -o clientreciever
		
clientsender.o : clientsender.c
	gcc -c clientsender.c

clientreciever.o : clientreciever.c
	gcc -c clientreciever.c

clean:
	rm -rf server
	rm -rf clientsender
	rm -rf clientreciever
	rm -rf *.o