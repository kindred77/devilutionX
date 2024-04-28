#pragma once

namespace devilution {

class Color;
using ColorPtr = std::shared_ptr<Color>;

class Color : public std::enable_shared_from_this<Point> {
private:
	int red_;
	int green_;
	int blue_;

public:
	static ColorPtr WHITE, BLACK;
	Color(int red, int green, int blue)
	{
		red_ = red;
		green_ = green;
		blue_ = blue;
	}
	bool operator==(const ColorPtr other) const
	{
		return red_ == other->red_ && green_ == other->green_ && blue_ == other->blue_;
	}

	ColorPtr operator=(const ColorPtr other)
	{
		red_ = other->red_;
		green_ = other->green_;
		blue_ = other->blue_;
		return shared_from_this();
	}
};

ColorPtr Color::WHITE = std::make_shared<Color>(255, 255, 255);
ColorPtr Color::BLACK = std::make_shared<Color>(0, 0, 0);

}
