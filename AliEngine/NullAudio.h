#pragma once
#include "AudioService.h"

class NullAudio final : public AudioService
{
public:
	NullAudio() = default;
	~NullAudio() override = default;

	virtual void PlaySound(const SoundIds&, bool) override {};
	virtual void StopSound(const SoundIds&, bool) override {};
	virtual void AddSound(const SoundIds&, const std::string&, bool)override {};
	virtual void StopAllSounds() override {};
	virtual void Update() override {};

protected:
	virtual void QueueSound(const SoundIds&, int, bool) override {};
};