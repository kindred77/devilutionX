#pragma once

#include "mir2/Common/Size.h"
#include "mir2/Common/Point.h"

namespace devilution {

class Rectangle;
using RectanglePtr = std::shared_ptr<Rectangle>;

class Rectangle : public std::enable_shared_from_this<Point> {
private:
	int left_;
	int right_;
	int top_;
	int bottom_;

public:
	Rectangle(int left, int right, int top, int bottom)
	{
		left_ = left;
		right_ = right;
		top_ = top;
		bottom_ = bottom;
	}
	bool operator==(const RectanglePtr other) const
	{
		return left_ == other->left_ && right_ == other->right_ && top_ == other->top_ && bottom_ == other->bottom_;
	}

	bool operator!=(const RectanglePtr other) const
	{
		return !(left_ == other->left_ && right_ == other->right_ && top_ == other->top_ && bottom_ == other->bottom_);
	}

	RectanglePtr operator=(const RectanglePtr other)
	{
		left_ = other->left_;
		right_ = other->right_;
		top_ = other->top_;
		bottom_ = other->bottom_;
		return shared_from_this();
	}

	bool Contains(PointPtr point)
	{
		return false;
	}
};

} // namespace devilution
