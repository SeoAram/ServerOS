#pragma once
enum class RoomType : char;

enum class PacketType : unsigned char{
	LOGIN_PACKET = 1,
	MOVE_PACKET,
	LOGOUT_PACKET
};

enum class EventType : unsigned char{
	MONSTER_MOVE = 1
};

struct PacketHeader{
	int packetSize;
	PacketType protocol;
};

struct LoginPacket :public PacketHeader{
	int id;
};
//
//struct PlayerPacket : public PacketHeader{
//	int id;
//	float x, y, z;
//	float dx, dy, dz;
//	short direct;
//};

struct PlayerPacket : public PacketHeader{
	int id;
	float x, y, z;
	float dx, dy, dz;
	short direct;
};