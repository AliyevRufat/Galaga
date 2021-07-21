#include "pch.h"
#include <string>
#include <memory>
#include <iostream>
#include <SDL.h>
#include "Renderer.h"
#include "glm\vec2.hpp"
#include "Font.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include <SDL_ttf.h>
#include "EngineTime.h"
#include "GameObject.h"

TextComponent::TextComponent(const std::string& text, const std::shared_ptr<dae::Font>& spFont, const SDL_Color& color, bool isVisible)
	:m_Text{ text }
	, m_SpFont{ spFont }
	, m_Color{ color }
	, m_IsVisible{ isVisible }
{
	const auto surf = TTF_RenderText_Blended(m_SpFont->GetFont(), m_Text.c_str(), m_Color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_pRender = new RenderComponent();
	m_pRender->SetTexture2D(std::make_shared<dae::Texture2D>(texture));
}

TextComponent::~TextComponent()
{
	delete m_pRender;
}

void TextComponent::SetIsVisible(bool isVisible, float setVisibleFalseTimer)
{
	m_IsVisible = isVisible;
	//set timer
	if (setVisibleFalseTimer != 0)
	{
		m_SetTimer = true;
		m_SetVisibleFalseTimer = setVisibleFalseTimer;
	}
}

void TextComponent::UpdateText(const std::string& text)
{
	m_Text = text;
}

void TextComponent::Update()
{
	if (m_SetTimer)
	{
		m_SetVisibleFalseTimer -= EngineTime::GetInstance().GetDeltaTime();

		if (m_SetVisibleFalseTimer <= 0.0f)
		{
			m_SetTimer = false;
			m_SetVisibleFalseTimer = 0.0f;
			m_IsVisible = false;
		}
	}
}

void TextComponent::Render()
{
	glm::vec2 position = {};
	if (m_pGameObject && m_pGameObject->GetComponent<TransformComponent>())
	{
		position = m_pGameObject->GetComponent<TransformComponent>()->GetTransform().GetPosition();
	}
	//
	if (m_pRender->GetTexture2D() && m_IsVisible)
	{
		const auto surf = TTF_RenderText_Blended(m_SpFont->GetFont(), m_Text.c_str(), m_Color);
		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		SDL_FreeSurface(surf);
		m_pRender->SetTexture2D(std::make_shared<dae::Texture2D>(texture));
		m_pRender->RenderTexture(glm::vec2{ position.x, position.y });
	}
};