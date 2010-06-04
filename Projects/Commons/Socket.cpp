#include "Socket.h"

bool Socket::connect(const char *host, unsigned short port) {
	ACE_INET_Addr addr;
	ACE_Time_Value timeout(SOCKET_TIMEOUT);
	addr.set(port, host);
	return (m_connector.connect(m_socket, addr, &timeout) == 0);
}

void Socket::close() {
	//Socket already closed or not, we terminate it, screw the return value
	m_socket.close();
}

/* TODO: Should it be implemented or not? Still I coded it and will be uncommented if we ever need it
bool Socket::writeRaw(ByteArray &data) {
	return (m_sock.send_n(data.getBuffer(), data.size(), &timeout) > 0);
}
*/

bool Socket::write(Packet &data) {
	ACE_Time_Value timeout(SOCKET_TIMEOUT);
	data.printHex();
	//Return true if ssize_t is not 0 nor 1 (because both are error codes)
	//Possible returns:
	//	 len  The complete number of bytes transferred.
	//   0 	  EOF, i.e., the peer closed the connection.
    //   -1   An error occurred before the entire amount was transferred.
	
	//size+5 where 5 is the packet header size
	return (m_socket.send_n(data.genStringPacket().c_str(), data.size()+5, &timeout) > 0);
}

Packet Socket::read() {
	BYTE* buf = new BYTE[SOCKET_READ_BUFFER_SIZE];
	ssize_t bc ;
	if ((bc = m_socket.recv (buf, 1024)) > 0) {
		return Packet(buf, bc); //Create a byte array (packet) with BYTEs `buf` of size `bc`
	} else {
		return Packet(NULL, 0); //Special invalid constructor
	}
}

Socket::Socket() {
}

Socket::~Socket() {
}