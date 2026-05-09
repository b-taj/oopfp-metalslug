#pragma once
#include <SFML/Audio.hpp>

// SoundManager.h -- Central audio controller using allowed SFML objects.

class SoundManager
{
public:
	SoundManager() {}

	void	playMusic(const char* file) { bgMusic.openFromFile(file); bgMusic.setVolume(50.0f); bgMusic.setLoop(true); }
	
private:
	// Since std::vector is forbidden, we will handle a few fixed buffers if needed
	sf::Music			bgMusic;
};
