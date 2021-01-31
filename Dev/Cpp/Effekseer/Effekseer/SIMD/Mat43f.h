
#ifndef __EFFEKSEER_SIMD_MAT43F_H__
#define __EFFEKSEER_SIMD_MAT43F_H__

#include "Float4.h"
#include "Vec3f.h"
#include "Vec4f.h"
#include <assert.h>

namespace Effekseer
{

struct Matrix43;

namespace SIMD
{

struct Mat43f
{
	Float4 X;
	Float4 Y;
	Float4 Z;

	Mat43f() = default;
	Mat43f(const Mat43f& rhs) = default;
	Mat43f(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33, float m41, float m42, float m43);
	Mat43f(const Matrix43& mat);

	bool IsValid() const;

	Vec3f GetScale() const;

	Mat43f GetRotation() const;

	Vec3f GetTranslation() const;

	void GetSRT(Vec3f& s, Mat43f& r, Vec3f& t) const;

	void SetTranslation(const Vec3f& t);

	Mat43f& operator*=(const Mat43f& rhs);

	Mat43f& operator*=(float rhs);

	static const Mat43f Identity;

	static bool Equal(const Mat43f& lhs, const Mat43f& rhs, float epsilon = DefaultEpsilon);

	static Mat43f SRT(const Vec3f& s, const Mat43f& r, const Vec3f& t);

	static Mat43f Scaling(float x, float y, float z);

	static Mat43f Scaling(const Vec3f& scale);

	static Mat43f RotationX(float angle);

	static Mat43f RotationY(float angle);

	static Mat43f RotationZ(float angle);

	static Mat43f RotationXYZ(float rx, float ry, float rz);

	static Mat43f RotationZXY(float rz, float rx, float ry);

	static Mat43f RotationAxis(const Vec3f& axis, float angle);

	static Mat43f RotationAxis(const Vec3f& axis, float s, float c);

	static Mat43f Translation(float x, float y, float z);

	static Mat43f Translation(const Vec3f& pos);
};

inline Mat43f::Mat43f(
	float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33, float m41, float m42, float m43)
	: X(m11, m21, m31, m41)
	, Y(m12, m22, m32, m42)
	, Z(m13, m23, m33, m43)
{
}

inline bool operator==(const Mat43f& lhs, const Mat43f& rhs)
{
	return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
}

inline bool operator!=(const Mat43f& lhs, const Mat43f& rhs)
{
	return lhs.X != rhs.X && lhs.Y != rhs.Y && lhs.Z != rhs.Z;
}

inline Mat43f operator*(const Mat43f& lhs, const Mat43f& rhs)
{
	const Float4 mask = Float4::SetUInt(0, 0, 0, 0xffffffff);

	Mat43f res;
	res.X = mask & rhs.X;
	res.X = Float4::MulAddLane<0>(res.X, lhs.X, rhs.X);
	res.X = Float4::MulAddLane<1>(res.X, lhs.Y, rhs.X);
	res.X = Float4::MulAddLane<2>(res.X, lhs.Z, rhs.X);

	res.Y = mask & rhs.Y;
	res.Y = Float4::MulAddLane<0>(res.Y, lhs.X, rhs.Y);
	res.Y = Float4::MulAddLane<1>(res.Y, lhs.Y, rhs.Y);
	res.Y = Float4::MulAddLane<2>(res.Y, lhs.Z, rhs.Y);

	res.Z = mask & rhs.Z;
	res.Z = Float4::MulAddLane<0>(res.Z, lhs.X, rhs.Z);
	res.Z = Float4::MulAddLane<1>(res.Z, lhs.Y, rhs.Z);
	res.Z = Float4::MulAddLane<2>(res.Z, lhs.Z, rhs.Z);
	return res;
}

inline Vec3f Vec3f::Transform(const Vec3f& lhs, const Mat43f& rhs)
{
	Float4 s0 = rhs.X;
	Float4 s1 = rhs.Y;
	Float4 s2 = rhs.Z;
	Float4 s3 = Float4::SetZero();
	Float4::Transpose(s0, s1, s2, s3);

	Float4 res = Float4::MulAddLane<0>(s3, s0, lhs.s);
	res = Float4::MulAddLane<1>(res, s1, lhs.s);
	res = Float4::MulAddLane<2>(res, s2, lhs.s);
	return Vec3f{res};
}

inline Vec4f Vec4f::Transform(const Vec4f& lhs, const Mat43f& rhs)
{
	Float4 s0 = rhs.X;
	Float4 s1 = rhs.Y;
	Float4 s2 = rhs.Z;
	Float4 s3 = Float4(0.0f, 0.0f, 0.0f, 1.0f);
	Float4::Transpose(s0, s1, s2, s3);

	Float4 res = Float4::MulLane<0>(s0, lhs.s);
	res = Float4::MulAddLane<1>(res, s1, lhs.s);
	res = Float4::MulAddLane<2>(res, s2, lhs.s);
	res = Float4::MulAddLane<3>(res, s3, lhs.s);
	return res;
}

inline Mat43f& Mat43f::operator*=(const Mat43f& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline Mat43f& Mat43f::operator*=(float rhs)
{
	X *= rhs;
	Y *= rhs;
	Z *= rhs;
	return *this;
}

struct SRTMat43f
{
	Mat43f mat;
	float s;
	bool isSeparated;

	static const SRTMat43f Identity;

	bool IsValid() const
	{
		return mat.IsValid();
	}

	Vec3f GetTranslation() const
	{
		return mat.GetTranslation();
	}

	Vec3f GetScale() const
	{
		if (isSeparated)
		{
			return {s, s, s};
		}
		else
		{
			return mat.GetScale();
		}
	}

	SRTMat43f GetRotation() const
	{
		return {
			mat.GetRotation(),
			1.0f,
			true};
	}

	void GetSRT(Vec3f& scale, SRTMat43f& r, Vec3f& t) const
	{
		if (isSeparated)
		{
			r.mat.X = mat.X;
			r.mat.Y = mat.Y;
			r.mat.Z = mat.Z;
			t = Vec3f{mat.X.GetW(), mat.Y.GetW(), mat.Z.GetW()};
		}
		else
		{
			mat.GetSRT(scale, r.mat, t);
			r.isSeparated = true;
			r.s = 1.0f;
		}
	}

	void SetTranslation(const Vec3f& value)
	{
		return mat.SetTranslation(value);
	}

	SRTMat43f& operator*=(const SRTMat43f& rhs);

	Mat43f ToMat() const
	{
		if (isSeparated)
		{
			Mat43f ret = mat;
			Float4 scaling{s, s, s, 1.0f};
			ret.X *= scaling;
			ret.Y *= scaling;
			ret.Z *= scaling;
			return ret;
		}
		else
		{
			return mat;
		}
	}

	static SRTMat43f FromMat(const Mat43f& src)
	{
		SRTMat43f dst;

		Vec3f s;
		Mat43f r;
		Vec3f t;

		src.GetSRT(s, r, t);

		if (s.GetX() == s.GetY() && s.GetY() == s.GetZ())
		{
			dst.mat = r * Mat43f::Translation(t);
			dst.s = s.GetX();
			dst.isSeparated = true;
		}
		else
		{
			dst.mat = src;
			dst.isSeparated = false;
		}

		return dst;
	}

	static SRTMat43f SRT(const Vec3f& s, const SRTMat43f& r, const Vec3f& t)
	{
		assert(r.isSeparated);

		SRTMat43f dst;

		if (s.GetX() == s.GetY() && s.GetY() == s.GetZ())
		{
			dst.mat = r.mat * Mat43f::Translation(t);
			dst.s = s.GetX();
			dst.isSeparated = true;
		}
		else
		{
			dst.mat = Mat43f::SRT(s, r.mat, t);
			dst.isSeparated = false;
		}

		return dst;
	}

	static SRTMat43f RotationX(float angle)
	{
		return SRTMat43f{Mat43f::RotationX(angle), 1.0f, true};
	}

	static SRTMat43f RotationY(float angle)
	{
		return SRTMat43f{Mat43f::RotationY(angle), 1.0f, true};
	}

	static SRTMat43f RotationZ(float angle)
	{
		return SRTMat43f{Mat43f::RotationZ(angle), 1.0f, true};
	}

	static SRTMat43f RotationXYZ(float rx, float ry, float rz)
	{
		return SRTMat43f{Mat43f::RotationXYZ(rx, ry, rz), 1.0f, true};
	}

	static SRTMat43f RotationZXY(float rz, float rx, float ry)
	{
		return SRTMat43f{Mat43f::RotationZXY(rz, rx, ry), 1.0f, true};
	}

	static SRTMat43f RotationAxis(const Vec3f& axis, float angle)
	{
		return SRTMat43f{Mat43f::RotationAxis(axis, angle), 1.0f, true};
	}

	static SRTMat43f RotationAxis(const Vec3f& axis, float s, float c)
	{
		return SRTMat43f{Mat43f::RotationAxis(axis, s, c), 1.0f, true};
	}

	static SRTMat43f Translation(float x, float y, float z)
	{
		return SRTMat43f{Mat43f::Translation(x, y, z), 1.0f, true};
	}

	static SRTMat43f Translation(const Vec3f& pos)
	{
		return SRTMat43f{Mat43f::Translation(pos), 1.0f, true};
	}
};

inline Vec3f Vec3f::Transform(const Vec3f& lhs, const SRTMat43f& rhs)
{
	if (rhs.isSeparated)
	{
		return Vec3f::Transform(lhs * rhs.s, rhs.mat);
	}
	else
	{
		return Vec3f::Transform(lhs, rhs.mat);
	}
}

inline SRTMat43f operator*(const SRTMat43f& lhs, const SRTMat43f& rhs)
{
	if (lhs.isSeparated && rhs.isSeparated)
	{
		auto rm = rhs.mat;
		rm.X.SetW(rm.X.GetW() * lhs.s);
		rm.Y.SetW(rm.Y.GetW() * lhs.s);
		rm.Z.SetW(rm.Z.GetW() * lhs.s);
		rm = rm * lhs.mat;
		auto s = lhs.s * rhs.s;
		return SRTMat43f{rm, s, true};
	}
	else
	{
		auto mat = lhs.ToMat() * rhs.ToMat();
		return SRTMat43f{mat, 1.0f, false};
	}
}

inline SRTMat43f& SRTMat43f::operator*=(const SRTMat43f& rhs)
{
	*this = *this * rhs;
	return *this;
}

} // namespace SIMD

} // namespace Effekseer

#endif // __EFFEKSEER_SIMD_MAT43F_H__