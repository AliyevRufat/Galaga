#include "MiniginPCH.h"
#include "AnimationComponent.h"
#include "Texture2DComponent.h"

AnimationComponent::AnimationComponent(float frameSwitchTimer, int nrOfColumns, bool isLoop)
	:m_NrOfColumns{ nrOfColumns }
	, m_AnimIndex{ 0 }
	, m_IsLoop{ isLoop }
	, m_NextFrameTimer{ frameSwitchTimer }
	, m_NextFrameTime{ 0.0f }
{
}

void AnimationComponent::Render()
{
	Animate();
}

void AnimationComponent::Animate()
{
	if (!m_IsInitialized)
	{
		m_IsInitialized = true;
		m_spTexture2D = m_pGameObject->GetComponent<Texture2DComponent>()->GetTexture2D();
	}
	//get the correct frame
	SDL_Rect srcRect{};
	int textureWidth, textureHeight;
	SDL_QueryTexture(m_spTexture2D.get()->GetSDLTexture(), nullptr, nullptr, &textureWidth, &textureHeight);
	srcRect.h = textureHeight;
	srcRect.w = textureWidth / m_NrOfColumns;
	srcRect.y = 0;
	srcRect.x = srcRect.w * m_AnimIndex;
	//loop
	if (m_AnimIndex >= m_NrOfColumns)
	{
		if (m_IsLoop)
		{
			m_AnimIndex = 0;
		}
		else if (!m_IsLoop)
		{
			m_pGameObject->SetMarkForDelete(true);
		}
	}
	//next frame
	m_NextFrameTime += EngineTime::GetInstance().GetDeltaTime();
	if (m_NextFrameTime >= m_NextFrameTimer)
	{
		m_NextFrameTime -= m_NextFrameTime;
		++m_AnimIndex;
	}
	//
	m_pGameObject->GetComponent<Texture2DComponent>()->SetSrcRect(srcRect);
}

int AnimationComponent::GetNrOfColumns() const
{
	return m_NrOfColumns;
}