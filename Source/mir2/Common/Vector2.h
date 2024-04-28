#pragma once

namespace devilution {

class Vector2;
using Vector2Ptr = std::shared_ptr<Vector2>;

class Vector2 : public std::enable_shared_from_this<Vector2> {
private:
	int x_;
	int y_;

public:
	Vector2(int x, int y)
	{
		x_ = x;
		y_ = y;
	}
	bool operator==(const Vector2Ptr other) const
	{
		return x_ == other->x_ && y_ == other->y_;
	}

	Vector2Ptr operator=(const Vector2Ptr other)
	{
		x_ = other->x_;
		y_ = other->y_;
		return shared_from_this();
	}
};

} // namespace devilution
