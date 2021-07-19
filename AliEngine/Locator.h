#pragma once
#include "AudioService.h"
#include "NullAudio.h"

class Locator final
{
public:
	static void Initialize();

	static void Provide(AudioService* service);
	static AudioService& GetAudio();
	static void FreeResources();

private:
	static AudioService* m_pService;
	static NullAudio m_NullService;
};
