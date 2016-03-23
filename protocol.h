#pragma once

#define MAX_RECEIVE_BUFFER_LEN 128

enum class PacketType : unsigned char{
	LOGIN_PACKET = 1,
	MOVE_PACKET,
	LOGOUT_PACKET
};

enum class EventType : unsigned char{
};

struct PacketHeader{
	int packetSize;
	PacketType protocol;
};

struct PacketLogin{
	PacketHeader head;
	unsigned int id;
};
