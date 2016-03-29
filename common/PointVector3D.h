#pragma once

//3DVector
class PointVector3D
{
private:
public:
	float x, y, z;

	PointVector3D() : x(0), y(0), z(0){	}

	~PointVector3D(){}

	PointVector3D(float x, float y, float z) :x(x), y(y), z(z){}

	PointVector3D& operator+(PointVector3D& p){
		this->x += p.x;
		this->y += p.y;
		this->z += p.z;
		return *this;
	}

	PointVector3D& operator*(float k){
		this->x *= k;
		this->y *= k;
		this->z *= k;
		return *this;
	}

	void vectorNormalization(){
		float tmp = InvSqrt(x*x + y*y + z*z);
		x = x * tmp;
		y = y * tmp;
		z = z * tmp;
	}

	float InvSqrt(float x){
		float xhalf = 0.5f * x;
		int i = *(int*)&x;            // store floating-point bits in integer
		i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
		x = *(float*)&i;              // convert new bits into float
		x = x*(1.5f - xhalf*x*x);     // One round of Newton's method
		return x;
	}
	
	ostream& operator<<(ostream& os)
	{
		os << "(" << this->x << ", " << this->y << ", " << this->z << ")";
		return os;
	}
};

