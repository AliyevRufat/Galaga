#pragma once
#include "BaseComponent.h"
#include <SDL.h>
#include "glm\vec2.hpp"
#include <memory>
#include "Texture2D.h"

class RenderComponent final : public BaseComponent
{
public:
	explicit RenderComponent();

	void SetTexture2D(std::shared_ptr<dae::Texture2D> texture);
	std::shared_ptr<dae::Texture2D> GetTexture2D() const;
	//
	void RenderTexture(const glm::vec2& pos = { 0,0 });
	void RenderTexture(const float x, const float y, const float width, const float height) const;
	void RenderTexture(const float x, const float y, const float width, const float height,
		const float  xSrc, const float ySrc, const float widthSrc, const float heightSrc) const;
	//
	void Render() override {};
private:
	std::shared_ptr<dae::Texture2D> m_spTexture2D{};
};
