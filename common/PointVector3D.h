#pragma once

class Matrix{
public:
	Matrix() 
	{
		for (int i = 0; i < 3; ++i){
			for (int j = 0; j < 3; ++j)
				cube[i][j] = 0;
		}
		cube[0][0] = cube[2][2] = 1;
		cube[0][2] = cube[2][0] = 1;
	};
	float cube[3][3];
};

//3DVector
class PointVector3D
{
private:
public:
	float x, y, z;

	PointVector3D() : x(0), y(0), z(0){	}

	~PointVector3D(){}

	PointVector3D(float x, float y, float z) :x(x), y(y), z(z){}

	PointVector3D& operator+(PointVector3D* p){
		this->x += p->x;
		this->y += p->y;
		this->z += p->z;
		return *this;
	}

	PointVector3D& operator+=(PointVector3D* p){
		this->x += p->x;
		this->y += p->y;
		this->z += p->z;
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

	PointVector3D& operator*(Matrix& m){
		PointVector3D tmp;
		tmp.x = this->x * m.cube[0][0] + this->y * m.cube[1][0] + this->z * m.cube[2][0];
		tmp.y = this->x * m.cube[0][1] + this->y * m.cube[1][1] + this->z * m.cube[2][1];
		tmp.z = this->x * m.cube[0][2] + this->y * m.cube[1][2] + this->z * m.cube[2][2];
		return tmp;
	}
};

