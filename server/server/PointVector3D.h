#pragma once

//3DVector
class PointVector3D
{
public:
	unsigned int x, y, z;
	PointVector3D();
	PointVector3D(unsigned int x, unsigned int y, unsigned int z) :x(x), y(y), z(z){}
	~PointVector3D();

	PointVector3D& operator+(PointVector3D& p){
		this->x += p.x;
		this->y += p.y;
		this->z += p.z;
		return *this;
	}

	PointVector3D& operator*(unsigned int k){
		this->x *= k;
		this->y *= k;
		this->z *= k;
		return *this;
	}
};

