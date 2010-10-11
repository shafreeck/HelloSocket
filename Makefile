all:server client
server:server.cpp HelloSocket.cpp HelloSocket.h
	g++ -g -o server server.cpp HelloSocket.cpp
client:client.cpp HelloSocket.cpp HelloSocket.h
	g++ -g -o client client.cpp HelloSocket.cpp
clean:
	rm *.o rm server
