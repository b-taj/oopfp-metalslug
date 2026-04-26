#pragma once
#include <SFML/Audio.hpp>


// SoundManager.h -- Central audio controller.
//
// Echo Chambers feature: when the player enters an aquatic biome,
// applyLowpassFilter() decomposes audio into discrete frequencies via FFT
// and removes the high-frequency components at runtime to simulate muffling.
// Pre-prepared muffled sounds are NOT used -- the filter runs live.

class SoundManager
{
public:
	void	play(const std::string& id);			// trigger one-shot SFX by id
	void	playMusic(const std::string& file);		// start looping background music
	void	setUnderwater(bool underwater);		// toggle low-pass filter mode

	// Apply runtime FFT low-pass filter: removes high-frequency components.
	void	applyLowpassFilter(sf::SoundBuffer& buffer);

	// Decompose a PCM buffer into its frequency components (FFT implementation).
	std::vector<float>	decomposeFrequencies(std::vector<float> buffer);

private:
	std::vector<sf::SoundBuffer>	buffers;
	std::vector<sf::Sound>		sounds;
	sf::Music			bgMusic;
	bool				isUnderwater;
};
