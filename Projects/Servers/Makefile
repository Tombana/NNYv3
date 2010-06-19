##LD_LIBRARY_PATH = /home/nitrix/NNYv3/Projects/Dependencies/ACE_wrappers/build/ace/.libs
LINK = -Wall -I../Commons -I../Resources -I../Dependencies/ACE_wrappers -I../Dependencies/MySQL/include -lACE -lmysqlclient
COMMONS = obj/Packet.o obj/Socket.o obj/Thread.o obj/database.o
SRCS =  obj/Realm/main.o obj/Realm/PacketHandler.o obj/Realm/SessionMgr.o obj/Realm/PacketDispatcher.o

all : $(COMMONS) $(SRCS)
	g++ $(LINK) -o nnyv3_realm $(COMMONS) $(SRCS)

obj/database.o : ../Commons/database.cpp \
		 ../Commons/database.h
		 g++ $(LINK) -c ../Commons/database.cpp -o obj/database.o

obj/Packet.o : ../Commons/Packet.cpp \
	       ../Commons/Packet.h
	       g++ $(LINK) -c ../Commons/Packet.cpp -o obj/Packet.o

obj/Socket.o : ../Commons/Socket.cpp \
	       ../Commons/Socket.h
	       g++ $(LINK) -c ../Commons/Socket.cpp -o obj/Socket.o

obj/Thread.o : ../Commons/Thread.cpp \
	       ../Commons/Thread.h
	       g++ $(LINK) -c ../Commons/Thread.cpp -o obj/Thread.o

obj/Realm/main.o : src/Realm/main.cpp \
	           src/Realm/main.h
	           g++ $(LINK) -c src/Realm/main.cpp -o obj/Realm/main.o

obj/Realm/PacketDispatcher.o : src/Realm/PacketDispatcher.cpp \
			       src/Realm/PacketDispatcher.h
			       g++ $(LINK) -c src/Realm/PacketDispatcher.cpp -o obj/Realm/PacketDispatcher.o

obj/Realm/PacketHandler.o : src/Realm/PacketHandler.cpp \
			    src/Realm/PacketHandler.h
			    g++ $(LINK) -c src/Realm/PacketHandler.cpp -o obj/Realm/PacketHandler.o

obj/Realm/SessionMgr.o : src/Realm/SessionMgr.cpp \
			 src/Realm/SessionMgr.h
			 g++ $(LINK) -c src/Realm/SessionMgr.cpp -o obj/Realm/SessionMgr.o
