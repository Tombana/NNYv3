CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lpthread -lmysqlclient
SOURCES=main.cpp ByteArray.cpp threadHandler.cpp ZSocket.cpp updater.cpp database.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=nnyserver

all: $(addprefix src/,$(SOURCES)) $(EXECUTABLE) 

$(EXECUTABLE):		$(addprefix obj/,$(OBJECTS))
			$(CC) $(LDFLAGS) $(addprefix obj/,$(OBJECTS)) -o $@
			rm -rf obj

obj/main.o: 		src/main.cpp \
			src/main.h \
			src/threadHandler.h \
			src/database.h \
			src/Dispatcher.h
			php tool/AutoVersioning/gen.php
			mkdir obj
			$(CC) $(CFLAGS) src/main.cpp -o obj/main.o

obj/threadHandler.o: 	src/threadHandler.cpp \
			src/threadHandler.h \
			src/updater.h \
			src/ZSocket.h \
			src/ByteArray.h
			$(CC) $(CFLAGS) src/threadHandler.cpp -o obj/threadHandler.o

obj/ByteArray.o: 	src/ByteArray.cpp \
			src/ByteArray.h \
			src/ZSocket.h
			$(CC) $(CFLAGS) src/ByteArray.cpp -o obj/ByteArray.o

obj/ZSocket.o:		src/ZSocket.cpp \
			src/ZSocket.h \
			src/ByteArray.h
			$(CC) $(CFLAGS) src/ZSocket.cpp -o obj/ZSocket.o

obj/updater.o:		src/updater.cpp \
			src/updater.h \
			src/ZSocket.h \
			src/ByteArray.h
			$(CC) $(CFLAGS) src/updater.cpp -o obj/updater.o

obj/database.o:		src/database.cpp \
			src/database.h \
			src/ZSocket.h
			$(CC) $(CFLAGS) src/database.cpp -o obj/database.o

clean:
	rm -rf obj