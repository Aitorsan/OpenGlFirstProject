#pragma once

#include <cmath>
namespace math
{
#define PI 3.141592653589793238462643383279202884L
	/*template <typename Type>
	struct vec4
	{
		union
		{
			struct {  Type x, y, z,w; };
			struct {  Type r, g, b,w;};
			Type n[4]{ 0,0,0,0 };
		};

		vec4() :x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 }
		{
		}

		explicit vec4(Type a, Type b, Type c): x{ a }, y{ b }, z{ c },w {0}
		{
		}

		explicit vec4(const vec4& v):x{v.x},y{v.y},z{v.z}
		~vec4() = default;

		template <typename OtherType> vec4& operator = (const vec4<OtherType>& v);
		template <typename OtherType> vec4& operator += (OtherType scalar);
		template <typename OtherType> vec4& operator += (const vec4<OtherType>& v);
		template <typename OtherType> vec4& operator -=(OtherType scalar);
		template <typename OtherType> vec4& operator -=(const vec4<OtherType>& v);
		template <typename OtherType> vec4& operator *= (OtherType scalar);


		vec4& operator /=(const vec4& v);
		friend bool operator==(const vec4& lhs, const vec4& rhs);
		friend bool operator!=(const vec4& lhs, const vec4& rhs);
		friend vec4 operator+(const vec4& lhs, const vec4& rhs);
		friend vec4 operator-(const vec4& lhs, const vec4& rhs);

		template <typename OtherType>
		vec4& operator*(Othertype scalar);
		template <typename OtherType>
		vec4& operator/(type scalar);
		float lenght() const;
		vec4 normal();
		template <typename OtherType>vec4 cross(const vec4& v);
		vec4 dot(const vec4& v);

	};


	template <typename Type>
	template <typename OtherType>
	inline vec4<Type>& vec4<Type>::operator=(const vec4<OtherType>& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	template <typename Type>
	template <typename OtherType>
	inline vec4<Type>& vec4<Type>::operator+=(const vec4<OtherType>& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	template <typename Type>
	template <typename OtherType>
	inline vec4<Type>& vec4<Type>::operator+=(OtherType scalar)
	{
		x += scalar;
		y += scalar;
		z += scalar;
		w += scalar;
		return *this;
	}

	template<typename Type>
	inline float vec4<Type>::lenght() const
	{
		return sqrt(((x*x) + (y*y) +(z*z)+(w*w));
	}

	template<typename Type>
	inline vec4<Type> vec4<Type>::normal()
	{
		return lenght() > 0 ? 1/lenght() : 0;
	}

	template <typename Type>
	template <typename OtherType>
	inline vec4<Type> vec4<Type>::cross(const vec4<OtherType>& v)
	{
	     vec4<Type> c;
	     c.x = y * v.z  - z * v.y;
		 c.y = z * v.x  - x * v.z;
		 c.z = x * v.y - y * v.x;
	     return c;
	}

	template<typename Type>
	inline vec4<Type> vec4<Type>::dot(const vec4 & v)
	{
		return vec4(x*v.x,y*v.y,z*v.z,w*v.w);
	}
	
	*/
	template <int Size = 4>
	void CreateProjectionMatrix(float(&array4x4)[Size][Size], float screenWidht, float screenHeight)
	{
		memset(array4x4, 0, sizeof(array4x4));
		constexpr float FOV = 90.0f*PI / 180.f;
		float AspectRatio = screenHeight / screenWidht;
		constexpr float Znear = 0.1f;
		constexpr float Zfar = 100.f;
		float q = 1.0f / tanf(FOV / 2);

		array4x4[0][0] = q * AspectRatio;
		array4x4[1][1] = q;
		array4x4[2][2] = (Znear + Zfar) / (Znear - Zfar);
		array4x4[2][3] = -1;
		array4x4[3][2] = 2 * (Znear*Zfar) / (Znear - Zfar);

	}

	void IdentityMatrix4x4(float (&I)[4][4])
	{
		memset(I, 0, sizeof(I));
		I[0][0] = 1;
		I[1][1] = 1;
		I[2][2] = 1;
		I[3][3] = 1;	
	}

	void CreateTranslationMatrix(float(&array4x4)[4][4],float Tx,float Ty, float Tz)
	{
		 IdentityMatrix4x4(array4x4);
		 array4x4[3][0] = Tx;
		 array4x4[3][1] = Ty;
		 array4x4[3][2] = Tz;
	}

	void CreateScaleMatrix(float(&array4x4)[4][4], float Sx, float Sy, float Sz)
	{
		IdentityMatrix4x4(array4x4);
		array4x4[0][0] = Sx;
		array4x4[1][1] = Sy;
		array4x4[2][2] = Sz;
	
	}
		
	void multiplyByM4(float(&array)[4], float(&array4x4)[4][4])
	{

	}

}