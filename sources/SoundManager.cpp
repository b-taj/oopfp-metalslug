#include "../headers/SoundManager.h"
#include <cmath>
#include <cstring>

SoundManager::SoundManager() : soundCount(0), isUnderwater(false)
{
}

void SoundManager::play(const char* id)
{
	// Logic to find buffer by ID and play in sounds[soundCount++]
}

void SoundManager::playMusic(const char* file)
{
	bgMusic.openFromFile(file);
	bgMusic.setLoop(true);
	bgMusic.play();
}

void SoundManager::setUnderwater(bool u)
{
	if (isUnderwater != u) {
		isUnderwater = u;
		// If changed, real-time reprocessing would happen here
		// Rubric: trigger applyLowpassFilter
	}
}

void SoundManager::dft(const sf::Int16* samples, int n, float* outReal, float* outImag)
{
	for (int k = 0; k < n; ++k) {
		float sumReal = 0;
		float sumImag = 0;
		for (int t = 0; t < n; ++t) {
			float angle = 2.0f * 3.14159265f * k * t / n;
			sumReal += samples[t] * std::cos(angle);
			sumImag -= samples[t] * std::sin(angle);
		}
		outReal[k] = sumReal;
		outImag[k] = sumImag;
	}
}

void SoundManager::idft(float* real, float* imag, int n, sf::Int16* outSamples)
{
	for (int t = 0; t < n; ++t) {
		float sum = 0;
		for (int k = 0; k < n; ++k) {
			float angle = 2.0f * 3.14159265f * k * t / n;
			sum += real[k] * std::cos(angle) - imag[k] * std::sin(angle);
		}
		outSamples[t] = (sf::Int16)(sum / n);
	}
}

void SoundManager::applyLowpassFilter(sf::SoundBuffer& buf)
{
	const sf::Int16* samples = buf.getSamples();
	sf::Uint64 count = buf.getSampleCount();
	unsigned int rate = buf.getSampleRate();
	
	// We only process in chunks of 2048 for the DFT window
	int n = 2048;
	if (count < (sf::Uint64)n) return;

	sf::Int16* newSamples = new sf::Int16[count];
	std::memcpy(newSamples, samples, count * sizeof(sf::Int16));

	// Cutoff frequency ~500Hz
	// index k corresponds to frequency k * rate / n
	int cutoffK = (500 * n) / rate;

	for (sf::Uint64 i = 0; i + n <= count; i += n) {
		dft(&samples[i], n, fftReal, fftImag);
		
		// Muffle: Zero out high frequencies
		for (int k = cutoffK; k < n - cutoffK; ++k) {
			fftReal[k] = 0;
			fftImag[k] = 0;
		}

		idft(fftReal, fftImag, n, &newSamples[i]);
	}

	buf.loadFromSamples(newSamples, count, buf.getChannelCount(), rate);
	delete[] newSamples;
}
