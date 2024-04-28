#pragma once

namespace devilution {

class SoundManager;
using SoundManagerPtr = std::shared_ptr<SoundManager>;

class SoundManager
{
public:
	static void PlaySound(int index, bool loop = false);
};

void SoundManager::PlaySound(int index, bool loop = false) {
	
};

}
