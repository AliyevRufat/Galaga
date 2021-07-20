#pragma once
#include "BaseComponent.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include <glm\vec2.hpp>
#include <SDL.h>

class TextComponent final : public BaseComponent
{
public:
	explicit TextComponent(const std::string& text, const std::shared_ptr<dae::Font>& spFont, const SDL_Color& color, bool isVisible = true);
	~TextComponent();

	void SetIsVisible(bool isVisible, float setVisibleFalseTimer = 0);
	void UpdateText(const std::string& text);
	void Update() override;
	void Render() override;
private:
	std::string m_Text;
	std::shared_ptr<dae::Font> m_SpFont;
	SDL_Color m_Color = {};
	//
	RenderComponent* m_pRender = nullptr;
	//
	float m_SetVisibleFalseTimer = {};
	bool m_IsVisible = false;
	bool m_SetTimer = false;
};