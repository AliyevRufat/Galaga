#include "pch.h"
#include "RenderComponent.h"
#include "Renderer.h"

RenderComponent::RenderComponent()
{
}

void RenderComponent::SetTexture2D(std::shared_ptr<dae::Texture2D> texture)
{
	m_spTexture2D = texture;
}

std::shared_ptr<dae::Texture2D> RenderComponent::GetTexture2D() const
{
	return m_spTexture2D;
}

void RenderComponent::RenderTexture(const glm::vec2& pos)
{
	dae::Renderer::GetInstance().RenderTexture(*m_spTexture2D, pos.x, pos.y);
}

void RenderComponent::RenderTexture(const float x, const float y, const float width, const float height,
	const float  xSrc, const float ySrc, const float widthSrc, const float heightSrc) const
{
	dae::Renderer::GetInstance().RenderTexture(*m_spTexture2D, x, y, width, height, xSrc, ySrc, widthSrc, heightSrc);
}

void RenderComponent::RenderTexture(const float x, const float y, const float width, const float height) const
{
	dae::Renderer::GetInstance().RenderTexture(*m_spTexture2D, x, y, width, height);
}