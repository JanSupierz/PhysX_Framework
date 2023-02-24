#include "stdafx.h"
#include "BoxForceScene.h"
#include "CubePosColorNorm.h"

enum InputIds : int
{
	Up, Down, Left, Right
};

void BoxForceScene::Initialize()
{
	//Physics
	EnablePhysxDebugRendering(true);
	const auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };

	//Constants
	constexpr float actorSize{ 1.f };
	const XMFLOAT4 color{ Colors::Green };
	const PxBoxGeometry boxGeometry{ actorSize / 2.f,actorSize / 2.f,actorSize / 2.f };
	constexpr float restitution{ 0.5f };

	//Half friction
	const PxMaterial* pHalfFrictionMaterial{ pPhysX->createMaterial(0.5f,0.5f,restitution) };

	m_pHalfFrictionBox = new CubePosColorNorm(actorSize, actorSize, actorSize);
	AddGameObject(m_pHalfFrictionBox);

	m_pHalfFrictionActor = pPhysX->createRigidDynamic(PxTransform{PxIdentity});
	PxRigidActorExt::createExclusiveShape(*m_pHalfFrictionActor, boxGeometry, *pHalfFrictionMaterial);

	m_pHalfFrictionBox->AttachRigidActor(m_pHalfFrictionActor);

	//Translate
	ResetPosition();

	//Ground plane
	const PxMaterial* pDefaultMaterial{ pPhysX->createMaterial(0.3f,0.3f,0.2f) };

	const auto pGroundActor{ pPhysX->createRigidStatic(PxTransform{PxQuat{PxPiDivTwo,PxVec3{0,0,1}}}) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Input - Last two are optional (mouse, game pad)
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Down,InputTriggerState::down,VK_DOWN });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Up,InputTriggerState::down,VK_UP });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Right,InputTriggerState::down,VK_RIGHT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Left,InputTriggerState::down,VK_LEFT });
}

void BoxForceScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		ResetPosition();
	}

	constexpr float forceValue{ 1000.f };
	const float force{ forceValue * m_SceneContext.GetGameTime()->GetElapsed() };

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Up))
	{
		m_pHalfFrictionActor->addForce({ 0.f, 0.f, force });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Down))
	{
		m_pHalfFrictionActor->addForce({ 0.f, 0.f, -force });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Right))
	{
		m_pHalfFrictionActor->addForce({ force, 0.f, 0.f });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Left))
	{
		m_pHalfFrictionActor->addForce({ -force, 0.f, 0.f });
	}
}

void BoxForceScene::Draw() const
{
}

void BoxForceScene::OnSceneActivated()
{
}

void BoxForceScene::OnSceneDeactivated()
{
}

void BoxForceScene::ResetPosition() const
{
	m_pHalfFrictionBox->Translate(0.f, m_StartHeight, 0.f);
	m_pHalfFrictionBox->RotateDegrees(0.f, 0.f, 0.f);
}
