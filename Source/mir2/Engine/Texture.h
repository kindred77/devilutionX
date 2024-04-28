#pragma once

namespace devilution {

class Texture;
using TexturePtr = std::shared_ptr<Texture>;

class Texture : public std::enable_shared_from_this<Texture> {
private:
	bool isDisposed_ = false;

public:
	Texture()
	{
	}

	void Dispose()
	{
		isDisposed_ = true;
	}

	bool IsDisposed() const
	{
		return isDisposed_;
	}
};

} // namespace devilution
