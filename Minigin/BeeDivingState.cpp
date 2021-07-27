#include "MiniginPCH.h"
#include "BeeDivingState.h"
#include "BeeTurnAroundState.h"
#include "TransformComponent.h"

BeeDivingState::BeeDivingState()
{
	m_SwitchState = { false };
}

void BeeDivingState::Update(BeeStateManager& bee)
{
	//dive down in one line
	const int speed = 500;
	auto beeTransformComp = bee.GetGameObject()->GetComponent<TransformComponent>();
	beeTransformComp->SetPosition(glm::vec2{ beeTransformComp->GetTransform().GetPosition().x ,beeTransformComp->GetTransform().GetPosition().y + speed * EngineTime::GetInstance().GetDeltaTime() });
	//check if reached the bottom
	if (beeTransformComp->GetTransform().GetPosition().y >= 850)//TODO : change this later to the actual window height
	{
		m_SwitchState = true;
	}
}

BeeState* BeeDivingState::StateSwitch()
{
	return new BeeTurnAroundState();
}

void BeeDivingState::Enter(BeeStateManager&)
{
}

void BeeDivingState::CreatePaths()
{
}