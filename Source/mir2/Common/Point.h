#pragma once

namespace devilution {

class Point;
using PointPtr = std::shared_ptr<Point>;

class Point : public std::enable_shared_from_this<Point> {
private:
	int x_;
	int y_;

public:
	static PointPtr Empty;
	Point()
	{
		x_ = 0;
		y_ = 0;
	}

	Point(int x, int y)
	{
		x_ = x;
		y_ = y;
	}
	bool operator==(const PointPtr other) const
	{
		return x_ == other->x_ && y_ == other->y_;
	}

	PointPtr Add(const PointPtr other) const
	{
		return std::make_shared<Point>(x_ + other->x_, y_ + other->y_);
	}

	PointPtr Add(int x, int y) const
	{
		return std::make_shared<Point>(x_ + x, y_ + y);
	}

	PointPtr Subtract(const PointPtr other) const
	{
		return std::make_shared<Point>(x_ - other->x_, y_ - other->y_);
	}

	PointPtr Subtract(int x, int y) const
	{
		return std::make_shared<Point>(x_ - x, y_ - y);
	}

	int X() const
	{
		return x_;
	}

	int Y() const
	{
		return y_;
	}
};

PointPtr Point::Empty = std::make_shared<Point>();

}
