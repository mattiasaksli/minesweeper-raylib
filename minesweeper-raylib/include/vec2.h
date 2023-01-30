#pragma once

template <typename T>
class Vec2
{
public:
	T x;
	T y;

	Vec2() = default;

	constexpr Vec2(T x, T y)
		: x(x),
		  y(y)
	{
	}

	explicit constexpr Vec2(T xy)
		: x(xy),
		  y(xy)
	{
	}

	constexpr bool operator==(const Vec2& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	constexpr bool operator!=(const Vec2& rhs) const
	{
		return !(*this == rhs);
	}

	constexpr Vec2 operator+(const Vec2& rhs) const
	{
		return {x + rhs.x, y + rhs.y};
	}

	constexpr Vec2 operator+(const T rhs) const
	{
		return {x + rhs, y + rhs};
	}

	constexpr Vec2& operator+=(const Vec2& rhs)
	{
		return *this = *this + rhs;
	}

	constexpr Vec2 operator-(const Vec2& rhs) const
	{
		return {x - rhs.x, y - rhs.y};
	}

	constexpr Vec2 operator-(const T rhs) const
	{
		return {x - rhs, y - rhs};
	}

	constexpr Vec2& operator-=(const Vec2& rhs)
	{
		return *this = *this - rhs;
	}

	constexpr Vec2 operator*(const Vec2& rhs) const
	{
		return {x * rhs.x, y * rhs.y};
	}

	constexpr Vec2 operator*(const T rhs) const
	{
		return {x * rhs, y * rhs};
	}

	constexpr Vec2& operator*=(const Vec2& rhs)
	{
		return *this = *this * rhs;
	}
};
