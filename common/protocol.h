#pragma once

#define MAX_RECEIVE_BUFFER_LEN 128

enum class PacketType : unsigned char{
	LOGIN_PACKET = 1,
	INIT_PACKET,
	MOVE_PACKET,
	LOGOUT_PACKET
};

enum class EventType : unsigned char{
};

struct PacketHeader{
	int packetSize;
	unsigned int id;
	PacketType protocol;
};

struct PacketLogin : public PacketHeader{
	void Init()
	{
		protocol = PacketType::LOGIN_PACKET;
		packetSize = sizeof(PacketLogin);
		id = 0;
	}
};

struct PacketInit : public PacketHeader{
	void Init()
	{
		protocol = PacketType::INIT_PACKET;
		packetSize = sizeof(PacketInit);
		id = 0;
	}
	float pos_x, pos_y, pos_z;
	float dir_x, dir_y, dir_z;
};