#pragma once

namespace devilution {

class Size;
using SizePtr = std::shared_ptr<Size>;

class Size : public std::enable_shared_from_this<Size> {
private:
	int width;
	int height;

public:
	bool operator==(const SizePtr other) const
	{
		return width == other->width && height == other->height;
	}

	bool operator!=(const SizePtr other) const
	{
		return !(width == other->width && height == other->height);
	}

	SizePtr operator=(const SizePtr other)
	{
		width = other->width;
		height = other->height;
		return shared_from_this();
	}
};

} // namespace devilution
