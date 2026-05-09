#pragma once
#include <SFML/Audio.hpp>

/**
 * SoundManager.h -- Manages audio playback and real-time DSP effects.
 * Includes a DFT-based lowpass filter for underwater muffling.
 */

class SoundManager
{
public:
	SoundManager();

	void	play(const char* id);
	void	playMusic(const char* file);
	void	setUnderwater(bool u);

	void	applyLowpassFilter(sf::SoundBuffer& buf);

private:
	void	dft(const sf::Int16* samples, int n, float* outReal, float* outImag);
	void	idft(float* real, float* imag, int n, sf::Int16* outSamples);

	sf::SoundBuffer buffers[32];
	sf::Sound		sounds[32];
	int				soundCount;
	
	sf::Music		bgMusic;
	bool			isUnderwater;

	// FFT working buffers (Using 2048 as requested for DSP window)
	float			fftReal[2048];
	float			fftImag[2048];
};
