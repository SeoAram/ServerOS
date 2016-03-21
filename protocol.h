#pragma once
enum class RoomType : char;

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
