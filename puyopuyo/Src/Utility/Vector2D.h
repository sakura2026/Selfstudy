#pragma once
class Vector2D
{
public:

	float x;
	float y;

public:

	Vector2D();
	Vector2D(float scalar);
	Vector2D(float mx, float my);

	~Vector2D();

public:

	// ‘ã“ü‰‰Zq
	Vector2D& operator = (const Vector2D& location);


	// ‰ÁZ‰‰Zq
	const Vector2D operator + (const Vector2D& location) const;
	// ‰ÁZ‘ã“ü‰‰Zq
	Vector2D& operator += (const Vector2D location);


	// Œ¸Z‰‰Zq
	const Vector2D operator - (const Vector2D& location) const;
	// Œ¸Z‘ã“ü‰‰Zq
	Vector2D operator -= (const Vector2D& location);


	// æZ‰‰Zq(scalar)
	const Vector2D operator * (const float& scalar) const;
	// æZ‰‰Zq(location)
	const Vector2D operator * (const Vector2D& location) const;

	// æZ‘ã“ü‰‰Zq(scalar)
	Vector2D& operator *= (const float& scalar);
	// æZ‘ã“ü‰‰Zq(location)
	Vector2D& operator *= (const Vector2D& location);

	// œZ‰‰Zq(scalar)
	const Vector2D operator / (const float& scalar) const;
	// œZ‰‰Zq(location)
	const Vector2D operator / (const Vector2D& location) const;

	// œZ‘ã“ü‰‰Zq(scalar)
	Vector2D& operator /= (const float& scalar);
	// œZ‘ã“ü‰‰Zq(location)
	Vector2D& operator /= (const Vector2D& location);

public:

	// intŒ^‚É•ÏŠ·
	void ToInt(int* x, int* y) const;

};