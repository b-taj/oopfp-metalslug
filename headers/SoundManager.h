#pragma once
#include <SFML/Audio.hpp>

/**
 * SoundManager.h -- Manages audio playback and real-time DSP effects.
 * Includes a lookup table for named sound effects and a DFT-based lowpass filter.
 */

class SoundManager
{
public:
	struct SoundEntry
	{
		char id[32];
		sf::SoundBuffer buffer;
		sf::Sound sound;
	};

	SoundManager();

	bool	load(const char* id, const char* filePath);
	void	play(const char* id);
	void	playMusic(const char* file);
	void	setUnderwater(bool u);

	void	applyLowpassFilter(sf::SoundBuffer& buf);

private:
	void	dft(const sf::Int16* samples, int n, float* outReal, float* outImag);
	void	idft(float* real, float* imag, int n, sf::Int16* outSamples);

	SoundEntry		entries[32];
	int				entryCount;
	
	sf::Music		bgMusic;
	bool			isUnderwater;

	float			fftReal[2048];
	float			fftImag[2048];
};
