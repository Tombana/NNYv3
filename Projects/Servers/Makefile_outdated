LINK = -Wall -I../Common -lpthread -I/usr/include/mysql -lmysqlclient
COMMONS = ../Common/ByteArray.o ../Common/ZSocket.o ../Common/ConfigFile.o
SRCS =  src/Grid.o src/realmConnector.o src/threadHandler.o src/threadPoolHandler.o src/main.o src/Database.o

all : $(COMMONS) $(SRCS)
	g++ $(LINK) -o nnyv3_world $(COMMONS) $(SRCS)

../Common/ByteArray.o : ../Common/ByteArray.cpp \
			../Common/ByteArray.h
		g++ $(LINK) -c ../Common/ByteArray.cpp -o ../Common/ByteArray.o

../Common/ZSocket.o : ../Common/ZSocket.cpp \
			../Common/ZSocket.h
		g++ $(LINK) -c ../Common/ZSocket.cpp -o ../Common/ZSocket.o

../Common/ConfigFile.o : ../Common/ConfigFile.cpp \
			../Common/ConfigFile.h
		g++ $(LINK) -c ../Common/ConfigFile.cpp -o ../Common/ConfigFile.o

src/main.o : 	src/main.cpp \
		src/main.h
		g++ $(LINK) -c src/main.cpp -o src/main.o

src/threadPoolHandler.o : src/threadPoolHandler.cpp \
			src/threadPoolHandler.h
		g++ $(LINK) -c src/threadPoolHandler.cpp -o src/threadPoolHandler.o

src/realmConnector.o : 	src/realmConnector.cpp \
			src/realmConnector.h
		g++ $(LINK) -c src/realmConnector.cpp -o src/realmConnector.o

src/Grid.o : src/Grid.cpp \
		src/Grid.h
		g++ $(LINK) -c src/Grid.cpp -o src/Grid.o

src/threadHandler.o : 	src/threadHandler.cpp \
			src/threadHandler.h
		g++ $(LINK) -c src/threadHandler.cpp -o src/threadHandler.o

src/Database.o : src/Database.cpp \
			src/Database.h
		g++ $(LINK) -c src/Database.cpp -o src/Database.o