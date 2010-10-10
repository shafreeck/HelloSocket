all:server
server:server.cpp HelloSocket.cpp HelloSocket.h
	g++ -g -o server server.cpp HelloSocket.cpp

clean:
	rm *.o rm server
