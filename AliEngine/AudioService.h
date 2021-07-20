#pragma once
#include <thread>
#include <mutex>
#include <queue>

class AudioService
{
public:

	struct Sound
	{
		std::string soundId;
		bool isEffect;
		int volume;
	};

	AudioService() = default;
	virtual ~AudioService() = default;
	AudioService(const AudioService&) = delete;
	AudioService& operator=(const AudioService&) = delete;
	AudioService(AudioService&&) = delete;
	AudioService& operator= (AudioService&&) = delete;

	virtual void PlaySound(const std::string& soundId, bool isEffect) = 0;
	virtual void StopSound(const std::string& soundId, bool isEffect) = 0;
	virtual void AddSound(const std::string& filePath, bool isEffect) = 0;
	virtual void StopAllSounds() = 0;
	virtual void Update() = 0;

protected:
	virtual void QueueSound(const std::string& soundId, int volume, bool isEffect) = 0;
};
