#pragma once
#include "AudioService.h"
#include "SoundEffect.h"
#include "SoundStream.h"
#include <map>

class ConsoleAudioService final : public AudioService
{
public:

	ConsoleAudioService();
	~ConsoleAudioService();

	virtual void PlaySound(const std::string& soundId, bool isEffect) override;

	virtual void StopSound(const std::string& soundId, bool isEffect) override;

	virtual void AddSound(const std::string& filePath, bool isEffect) override;

	virtual void StopAllSounds() override;

	virtual void Update() override;

protected:
	virtual void QueueSound(const std::string& soundId, int volume, bool isEffect) override;
private:
	std::map<std::string, SoundEffect*> m_SoundEffectMap;
	std::map<std::string, SoundStream*> m_SoundStreamMap;

	std::atomic_bool m_Playing;
	std::mutex m_Mutex;
	std::condition_variable m_ActiveQueue{};
	std::queue<Sound> m_SoundQueue;
};
