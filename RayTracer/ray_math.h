#ifndef RAY_MATH_H
#define RAY_MATH_H

#include <math.h>

#define Math_Pi 6.28318530717958647692528676655900576f
#define Math_Tau 3.14159265358979323846264338327950288f

typedef s32 b32;

inline u32
RoundF32ToU32(f32 Value) {
	u32 Result = (u32)roundf(Value);
	return(Result);
}


/*Simple operations*/
inline float Sqrt(float Value) {
	float Result;
	Result = sqrtf(Value);
	return(Result);
}

inline float RSqrt(float Value) {
	float Result;
	Result = 1.0f / sqrtf(Value);
	return(Result);
}

inline float Sin(float Rad) {
	float Result = sinf(Rad);
	return(Result);
}

inline float Cos(float Rad) {
	float Result = cosf(Rad);
	return(Result);
}

inline float Tan(float Rad) {
	float Result = tanf(Rad);
	return(Result);
}

inline float ASin(float Value) {
	float Result = asinf(Value);
	return(Result);
}

inline float ACos(float Value) {
	float Result = acosf(Value);
	return(Result);
}

inline float ATan(float Value) {
	float Result = atan(Value);
	return(Result);
}

inline float ATan2(float Y, float X) {
	float Result = atan2f(Y, X);
	return(Result);
}

inline float Exp(float Value) {
	float Result = expf(Value);
	return(Result);
}

inline float Log(float Value) {
	float Result = logf(Value);
	return(Result);
}

inline float Pow(float a, float b) {
	float Result = powf(a, b);
	return(Result);
}


struct v2 {
	float x;
	float y;
};

struct v3 {
	float x;
	float y;
	float z;
};

struct v4 {
	float x;
	float y;
	float z;
	float w;
};

inline v2 V2(float Value) {
	v2 Result;
	Result.x = Value;
	Result.y = Value;
	return(Result);
}

inline v2 V2(float x, float y) {
	v2 Result;

	Result.x = x;
	Result.y = y;

	return(Result);
}

/*v3 constructors*/
inline v3 V3(float Value) {
	v3 Result;
	Result.x = Value;
	Result.y = Value;
	Result.z = Value;
	return(Result);
}


inline v3 V3(float x, float y, float z) {
	v3 Result;

	Result.x = x;
	Result.y = y;
	Result.z = z;

	return(Result);
}

inline v4 V4(float Value) {
	v4 Result;
	Result.x = Value;
	Result.y = Value;
	Result.z = Value;
	Result.w = Value;
	return(Result);
}

inline v4 V4(float x, float y, float z, float w) {
	v4 Result;

	Result.x = x;
	Result.y = y;
	Result.z = z;
	Result.w = w;

	return(Result);
}

inline v4 V4(v3 InitVector, float w) {
	v4 Result;
	Result.x = InitVector.x;
	Result.y = InitVector.y;
	Result.z = InitVector.z;
	Result.w = w;
	return(Result);
}


inline float Dot(v2 v0, v2 v1) { 
	return v0.x * v1.x + v0.y * v1.y; 
}

inline float Dot(v3 v0, v3 v1) { 
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z; 
}

inline float Dot(v4 v0, v4 v1) { 
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w; 
}

inline v3 Hadamard(v3 A, v3 B) {
	v3 Result;

	Result.x = A.x * B.x;
	Result.y = A.y * B.y;
	Result.z = A.z * B.z;

	return(Result);
}

inline v3 Cross(v3 v0, v3 v1) {
	v3 v;
	v.x = v0.y * v1.z - v1.y * v0.z;
	v.y = v0.z * v1.x - v1.z * v0.x;
	v.z = v0.x * v1.y - v1.x * v0.y;
	return(v);
}


/*Add operation*/
inline v2 Add(v2 a, v2 b) {
	a.x += b.x;
	a.y += b.y;
	return(a);
}

inline v3 Add(v3 a, v3 b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return(a);
}

inline v4 Add(v4 a, v4 b) {
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;
	return(a);
}

/*Subtract operation*/
inline v2 Sub(v2 a, v2 b) {
	a.x -= b.x;
	a.y -= b.y;
	return(a);
}

inline v3 Sub(v3 a, v3 b) {
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	return(a);
}

inline v4 Sub(v4 a, v4 b) {
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	a.w -= b.w;
	return(a);
}

/*Multiply operation*/
inline v2 Mul(v2 a, float s) {
	a.x *= s;
	a.y *= s;
	return(a);
}

inline v3 Mul(v3 a, float s) {
	a.x *= s;
	a.y *= s;
	a.z *= s;
	return(a);
}

inline v4 Mul(v4 a, float s) {
	a.x *= s;
	a.y *= s;
	a.z *= s;
	a.w *= s;
	return(a);
}

/*Divide operation*/
inline v2 Div(v2 a, float s) {
	float OneOverS = 1.0f / s;
	a.x *= OneOverS;
	a.y *= OneOverS;
	return(a);
}

inline v3 Div(v3 a, float s) {
	float OneOverS = 1.0f / s;
	a.x *= OneOverS;
	a.y *= OneOverS;
	a.z *= OneOverS;
	return(a);
}

inline v4 Div(v4 a, float s) {

	float OneOverS = 1.0f / s;
	a.x *= OneOverS;
	a.y *= OneOverS;
	a.z *= OneOverS;
	a.w *= OneOverS;
	return(a);
}

/*v2 operator overloading*/
inline bool operator==(v2 a, v2 b) { return((a.x == b.x) && (a.y == b.y)); }
inline bool operator!=(v2 a, v2 b) { return((a.x != b.x) || (a.y != b.y)); }

inline v2 operator+(v2 a) { return(a); }
inline v2 operator-(v2 a) { v2 r = { -a.x, -a.y }; return(r); }

inline v2 operator+(v2 a, v2 b) { return Add(a, b); }
inline v2 operator-(v2 a, v2 b) { return Sub(a, b); }

inline v2 operator*(v2 a, float s) { return Mul(a, s); }
inline v2 operator*(float s, v2 a) { return Mul(a, s); }
inline v2 operator/(v2 a, float s) { return Div(a, s); }

inline v2 operator*(v2 a, v2 b) { v2 r = { a.x * b.x, a.y * b.y }; return(r); }
inline v2 operator/(v2 a, v2 b) { v2 r = { a.x / b.x, a.y / b.y }; return(r); }

inline v2 &operator+=(v2& a, v2 b) { return(a = a + b); }
inline v2 &operator-=(v2& a, v2 b) { return(a = a - b); }
inline v2 &operator*=(v2& a, float s) { return(a = a * s); }
inline v2 &operator/=(v2& a, float s) { return(a = a / s); }

/*v3 operator overloading*/
inline bool operator==(v3 a, v3 b) { return((a.x == b.x) && (a.y == b.y) && (a.z == b.z)); }
inline bool operator!=(v3 a, v3 b) { return((a.x != b.x) || (a.y != b.y) || (a.z != b.z)); }

inline v3 operator+(v3 a) { return(a); }
inline v3 operator-(v3 a) { v3 r = { -a.x, -a.y, -a.z }; return(r); }

inline v3 operator+(v3 a, v3 b) { 
	v3 Result;
	Result.x = a.x + b.x;
	Result.y = a.y + b.y;
	Result.z = a.z + b.z;
	return(Result);
}
inline v3 operator-(v3 a, v3 b) { 
	v3 Result;
	Result.x = a.x - b.x;
	Result.y = a.y - b.y;
	Result.z = a.z - b.z;
	return(Result);
}

inline v3 operator*(v3 a, float s) { 
	a.x *= s;
	a.y *= s;
	a.z *= s;
	return(a);
}
inline v3 operator*(float s, v3 a) {
	a.x *= s;
	a.y *= s;
	a.z *= s;
	return(a);
}
inline v3 operator/(v3 a, float s) {
	float OneOverS = 1.0f / s;
	a.x *= OneOverS;
	a.y *= OneOverS;
	a.z *= OneOverS;
	return(a);
}

inline v3 operator*(v3 a, v3 b) { v3 r = { a.x * b.x, a.y * b.y, a.z * b.z }; return(r); }
inline v3 operator/(v3 a, v3 b) { v3 r = { a.x / b.x, a.y / b.y, a.z / b.z }; return(r); }

inline v3 &operator+=(v3& a, v3 b) { return(a = a + b); }
inline v3 &operator-=(v3& a, v3 b) { return(a = a - b); }
inline v3 &operator*=(v3& a, float s) { return(a = a * s); }
inline v3 &operator/=(v3& a, float s) { return(a = a / s); }

/*v4 operator overloading*/
inline bool operator==(v4 a, v4 b) { return((a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w)); }
inline bool operator!=(v4 a, v4 b) { return((a.x != b.x) || (a.y != b.y) || (a.z != b.z) || (a.w != b.w)); }

inline v4 operator+(v4 a) { return(a); }
inline v4 operator-(v4 a) { v4 r = { -a.x, -a.y, -a.z, -a.w}; return(r); }

inline v4 operator+(v4 a, v4 b) { 
	v4 Result;
	Result.x = a.x + b.x;
	Result.y = a.y + b.y;
	Result.z = a.z + b.z;
	Result.w = a.w + b.w;
	return(Result);
}
inline v4 operator-(v4 a, v4 b) {
	v4 Result;
	Result.x = a.x - b.x;
	Result.y = a.y - b.y;
	Result.z = a.z - b.z;
	Result.w = a.w - b.w;
	return(Result);
}

inline v4 operator*(v4 a, float s) { return Mul(a, s); }
inline v4 operator*(float s, v4 a) { return Mul(a, s); }
inline v4 operator/(v4 a, float s) { return Div(a, s); }

inline v4 operator*(v4 a, v4 b) { v4 r = { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w }; return(r); }
inline v4 operator/(v4 a, v4 b) { v4 r = { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }; return(r); }

inline v4 &operator+=(v4& a, v4 b) { return(a = a + b); }
inline v4 &operator-=(v4& a, v4 b) { return(a = a - b); }
inline v4 &operator*=(v4& a, float s) { return(a = a * s); }
inline v4 &operator/=(v4& a, float s) { return(a = a / s); }


/*Magnitude of the vector*/
inline float Magnitude(v2 v) { return(Sqrt(Dot(v, v))); }
inline float Magnitude(v3 v) { return(Sqrt(Dot(v, v))); }
inline float Magnitude(v4 v) { return(Sqrt(Dot(v, v))); }

/*Squared magnitude*/
inline float SqMagnitude(v2 v) { return(Dot(v, v)); }
inline float SqMagnitude(v3 v) { return(Dot(v, v)); }
inline float SqMagnitude(v4 v) { return(Dot(v, v)); }

inline v2 NOZ(v2 v) { float sqmag = Dot(v, v); return((sqmag) == 0.0f ? V2(0.0f) : v * RSqrt(sqmag)); }
inline v4 NOZ(v4 v) { float sqmag = Dot(v, v); return((sqmag) == 0.0f ? V4(0.0f) : v * RSqrt(sqmag)); }

inline v3 NOZ(v3 v) {
	v3 Result = {};
	float SqMag = v.x * v.x + v.y * v.y + v.z * v.z;
	if (SqMag > 0.000001f) {
		f32 RevSqrt = 1.0f / Sqrt(SqMag);

		Result.x = v.x * RevSqrt;
		Result.y = v.y * RevSqrt;
		Result.z = v.z * RevSqrt;
	}

	return(Result);
}

inline u32 RGBAPack4x8(v4 Unpacked) {
	u32 Result =
		((RoundF32ToU32(Unpacked.w) << 24) |
		(RoundF32ToU32(Unpacked.z) << 16) |
		(RoundF32ToU32(Unpacked.y) << 8) |
		(RoundF32ToU32(Unpacked.x)));

	return(Result);
}

inline u32 BGRAPack4x8(v4 Unpacked) {
	u32 Result =
		((RoundF32ToU32(Unpacked.w) << 24) |
		(RoundF32ToU32(Unpacked.x) << 16) |
		(RoundF32ToU32(Unpacked.y) << 8) |
		(RoundF32ToU32(Unpacked.z)));

	return(Result);
}

inline v3 Lerp(v3 A, v3 B, f32 t) {
	v3 Result;

	Result.x = (1.0f - t) * A.x + t * B.x;
	Result.y = (1.0f - t) * A.y + t * B.y;
	Result.z = (1.0f - t) * A.z + t * B.z;

	return(Result);
}

inline v4 Linear1ToSRGB255(v4 v) {
	v4 Result;

	Result.x = 255.0f * Sqrt(v.x);
	Result.y = 255.0f * Sqrt(v.y);
	Result.z = 255.0f * Sqrt(v.z);
	Result.w = 255.0f * v.w;

	return(Result);
}
#endif