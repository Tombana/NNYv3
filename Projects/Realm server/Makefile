LINK = -Wall -I../Common -lpthread -I/usr/include/mysql -lmysqlclient
COMMONS = ../Common/ByteArray.o ../Common/ZSocket.o ../Common/ConfigFile.o
SRCS =  src/Database.o src/Dispatcher.o src/threadHandler.o src/main.o

all : $(COMMONS) $(SRCS)
	g++ $(LINK) -o nnyv3_realm $(COMMONS) $(SRCS)

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
		src/main.h \
		src/config.hpp \
		../Common/ConfigFile.h \
		../Common/ZSocket.h
		g++ $(LINK) -c src/main.cpp -o src/main.o

src/Database.o: 	src/Database.cpp \
			src/Database.h
		g++ $(LINK) -c src/Database.cpp -o src/Database.o

src/Dispatcher.o : 	src/Dispatcher.cpp \
			src/Dispatcher.h
		g++ $(LINK) -c src/Dispatcher.cpp -o src/Dispatcher.o

src/threadHandler.o : 	src/threadHandler.cpp \
			src/threadHandler.h
		g++ $(LINK) -c src/threadHandler.cpp -o src/threadHandler.o

force_look :
	true