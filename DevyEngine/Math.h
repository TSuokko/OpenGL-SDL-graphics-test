#pragma once
#include <math.h>	//for sqrt
#include <float.h>
namespace DevyEngine
{
	typedef float r;

	float const REAL_MAX = FLT_MAX;
	float const REAL_MIN = FLT_MIN;
	float const PI = r(3.14159265358979323846);
	float const Epsilon = r(0.00001);

	struct vector_2
	{
		float X, Y;

		inline vector_2(void);
		inline vector_2(float X, float Y);

		inline vector_2 &operator+=(vector_2 const &A);
		inline vector_2 &operator-=(vector_2 const &A);
	};

	struct matrix_2x2
	{
		float aElements[2][2];

		inline matrix_2x2(void);
		inline matrix_2x2(float RotateByGivenRadians);
	};

	inline float RadiansFrom(float Degrees);
	inline float DegreesFrom(float Radians);

	inline vector_2 operator-(vector_2 const &A, vector_2 const &B);
	inline vector_2 operator+(vector_2 const &A, vector_2 const &B);
	inline vector_2 operator*(float A, vector_2 const &B);
	inline vector_2 operator*(vector_2 const &A, float B);
	inline vector_2 operator/(vector_2 const &A, float B);

	inline float DotProduct(vector_2 const &A, vector_2 const &B);

	// A-perp dot B
	inline float PerpDotProduct(vector_2 const &A, vector_2 const &B);

	inline vector_2 GetPerpendicular(vector_2 const &A);
	inline float GetLength(vector_2 const &A);
	inline vector_2 GetNormal(vector_2 const &A);	// @todo need this?

	inline vector_2 operator*(matrix_2x2 const &A, vector_2 const &B);

	/*----------------------------------------------------------------------------

	Inline function definitions.

	*/

	inline float RadiansFrom(float Degrees)
	{
		return (Degrees * PI) / r(180);
	}

	inline float DegreesFrom(float Radians)
	{
		return (Radians * r(180)) / PI;
	}

	inline vector_2 operator-(vector_2 const &A, vector_2 const &B)
	{
		return vector_2(A.X - B.X, A.Y - B.Y);
	}

	inline vector_2 operator+(vector_2 const &A, vector_2 const &B)
	{
		return vector_2(A.X + B.X, A.Y + B.Y);
	}

	inline vector_2 operator*(float A, vector_2 const &B)
	{
		return vector_2(A*B.X, A*B.Y);
	}

	inline vector_2 operator*(vector_2 const &A, float B)
	{
		return vector_2(B*A.X, B*A.Y);
	}

	inline vector_2 operator/(vector_2 const &A, float B)
	{
		return vector_2(A.X / B, A.Y / B);
	}

	inline float DotProduct(vector_2 const &A, vector_2 const &B)
	{
		return A.X*B.X + A.Y*B.Y;
	}

	inline float PerpDotProduct(vector_2 const &A, vector_2 const &B)
	{
		return A.X*B.Y - A.Y*B.X;
	}

	inline vector_2 GetPerpendicular(vector_2 const &A)
	{
		return vector_2(-A.Y, A.X);
	}

	inline float GetLength(vector_2 const &A)
	{
		return r(sqrt(A.X*A.X + A.Y*A.Y));
	}

	inline vector_2 GetNormal(vector_2 const &A)
	{
		float OneOverLength = r(1) / GetLength(A);
		return vector_2(OneOverLength*A.X, OneOverLength*A.Y);
	}

	inline vector_2::vector_2(void) : X(r(0)), Y(r(0))
	{
	}

	inline vector_2::vector_2(float X_, float Y_) : X(X_), Y(Y_)
	{
	}

	inline vector_2 &vector_2::operator+=(vector_2 const &A)
	{
		X += A.X;
		Y += A.Y;
		return *this;
	}
	inline vector_2 &vector_2::operator-=(vector_2 const &A)
	{
		X -= A.X;
		Y -= A.Y;
		return *this;
	}

	inline vector_2 operator*(matrix_2x2 const &A, vector_2 const &B)
	{
		return vector_2(A.aElements[0][0] * B.X + A.aElements[0][1] * B.Y,
			A.aElements[1][0] * B.X + A.aElements[1][1] * B.Y);
	}

	inline matrix_2x2::matrix_2x2(void)
	{
		aElements[0][0] = aElements[0][1] = aElements[1][0] = aElements[1][1] = r(0);
	}

	inline matrix_2x2::matrix_2x2(float Radians)
	{
		float const CosAngle = (float)cos(Radians);
		float const SinAngle = (float)sin(Radians);

		aElements[0][0] = CosAngle; aElements[0][1] = -SinAngle;
		aElements[1][0] = SinAngle; aElements[1][1] = CosAngle;
	}
	
}

