#include "../headers/SoundManager.h"
#include <cmath>
#include <cstring>

SoundManager::SoundManager() : entryCount(0), isUnderwater(false)
{
}

bool SoundManager::load(const char* id, const char* filePath)
{
	if (entryCount >= 32) return false;
	
	SoundEntry& e = entries[entryCount];
	
	int j = 0;
	for (; j < 31 && id[j] != '\0'; j++) {
		e.id[j] = id[j];
	}
	e.id[j] = '\0';

	if (!e.buffer.loadFromFile(filePath)) return false;
	
	e.sound.setBuffer(e.buffer);
	entryCount++;
	return true;
}

void SoundManager::play(const char* id)
{
	for (int i = 0; i < entryCount; i++) {
		if (std::strcmp(entries[i].id, id) == 0) {
			entries[i].sound.play();
			return;
		}
	}
}

void SoundManager::playMusic(const char* file)
{
	bgMusic.openFromFile(file);
	bgMusic.setLoop(true);
    bgMusic.setVolume(30.0f);
	bgMusic.play();
}

void SoundManager::pauseMusic() { bgMusic.pause(); }
void SoundManager::resumeMusic() { bgMusic.play(); }

void SoundManager::setUnderwater(bool u)
{
	if (isUnderwater != u) {
		isUnderwater = u;
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
	
	int n = 2048;
	if (count < (sf::Uint64)n) return;

	sf::Int16* newSamples = new sf::Int16[count];
	std::memcpy(newSamples, samples, count * sizeof(sf::Int16));

	int cutoffK = (500 * n) / rate;

	for (sf::Uint64 i = 0; i + n <= count; i += n) {
		dft(&samples[i], n, fftReal, fftImag);
		for (int k = cutoffK; k < n - cutoffK; ++k) {
			fftReal[k] = 0;
			fftImag[k] = 0;
		}
		idft(fftReal, fftImag, n, &newSamples[i]);
	}

	buf.loadFromSamples(newSamples, count, buf.getChannelCount(), rate);
	delete[] newSamples;
}
