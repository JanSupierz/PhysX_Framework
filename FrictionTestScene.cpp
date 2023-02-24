#include "stdafx.h"
#include "FrictionTestScene.h"
#include "CubePosColorNorm.h"

void FrictionTestScene::Initialize()
{
	//Physics
	EnablePhysxDebugRendering(true);
	const auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };

	//Constants
	constexpr float actorSize{ 1.f };
	const XMFLOAT4 color{ Colors::Green };
	const PxBoxGeometry boxGeometry{ actorSize / 2.f,actorSize / 2.f,actorSize / 2.f };
	constexpr float restitution{ 0.5f };

	//No friction
	const PxMaterial* pNoFrictionMaterial{ pPhysX->createMaterial(0.f,0.f,restitution) };

	m_pNoFrictionBox = new CubePosColorNorm(actorSize, actorSize, actorSize);
	AddGameObject(m_pNoFrictionBox);

	PxRigidDynamic* pNoFrictionActor{ pPhysX->createRigidDynamic(PxTransform{PxIdentity}) };
	PxRigidActorExt::createExclusiveShape(*pNoFrictionActor, boxGeometry, *pNoFrictionMaterial);

	m_pNoFrictionBox->AttachRigidActor(pNoFrictionActor);

	//Half friction
	const PxMaterial* pHalfFrictionMaterial{ pPhysX->createMaterial(0.5f,0.5f,restitution) };

	m_pHalfFrictionBox = new CubePosColorNorm(actorSize, actorSize, actorSize);
	AddGameObject(m_pHalfFrictionBox);

	PxRigidDynamic* pHalfFrictionActor{ pPhysX->createRigidDynamic(PxTransform{PxIdentity}) };
	PxRigidActorExt::createExclusiveShape(*pHalfFrictionActor, boxGeometry, *pHalfFrictionMaterial);

	m_pHalfFrictionBox->AttachRigidActor(pHalfFrictionActor);

	//Full friction
	const PxMaterial* pFullFrictionMaterial{ pPhysX->createMaterial(1.f,1.f,restitution) };

	m_pFullFrictionBox = new CubePosColorNorm(actorSize, actorSize, actorSize);
	AddGameObject(m_pFullFrictionBox);

	PxRigidDynamic* pFullFrictionActor{ pPhysX->createRigidDynamic(PxTransform{PxIdentity}) };
	PxRigidActorExt::createExclusiveShape(*pFullFrictionActor, boxGeometry, *pFullFrictionMaterial);

	m_pFullFrictionBox->AttachRigidActor(pFullFrictionActor);

	//Translate
	ResetPositions();

	//Ground plane
	const PxMaterial* pDefaultMaterial{ pPhysX->createMaterial(0.3f,0.3f,0.2f) };

	const auto pGroundActor{ pPhysX->createRigidStatic(PxTransform{PxQuat{PxPiDivTwo,PxVec3{0,0,1}}}) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Ramps
	const PxMaterial* pRampMaterial{ pPhysX->createMaterial(0.f,0.f,0.2f) };

	const XMFLOAT3 rampSize{ 7.f,1.f,2.f };
	constexpr float rampAngle{ -30.f };
	constexpr float rampOffset{ 2.f };


	for (int index{ 0 }; index < 3; ++index)
	{
		GameObject* pRamp = new CubePosColorNorm(rampSize.x, rampSize.y, rampSize.z);
		AddGameObject(pRamp);

		const auto pRampActor{ pPhysX->createRigidStatic(PxTransform{PxIdentity}) };
		PxRigidActorExt::createExclusiveShape(*pRampActor, PxBoxGeometry{ 0.5f * rampSize.x, 0.5f * rampSize.y, 0.5f * rampSize.z }, *pRampMaterial);

		pRamp->AttachRigidActor(pRampActor);

		pRamp->Translate(-m_StartXOffset + m_StartXOffset * index + rampOffset, 0.5f * m_StartHeight, 0.f);
		pRamp->RotateDegrees(0.f, 0.f, rampAngle);
	}
}

void FrictionTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		ResetPositions();
	}
}

void FrictionTestScene::Draw() const
{
}

void FrictionTestScene::OnSceneActivated()
{
}

void FrictionTestScene::OnSceneDeactivated()
{
}

void FrictionTestScene::ResetPositions() const
{
	m_pNoFrictionBox->Translate(-m_StartXOffset, m_StartHeight, 0.f);
	m_pHalfFrictionBox->Translate(0.f, m_StartHeight, 0.f);
	m_pFullFrictionBox->Translate(m_StartXOffset, m_StartHeight, 0.f);

	m_pNoFrictionBox->RotateDegrees(0.f, 0.f, 0.f);
	m_pHalfFrictionBox->RotateDegrees(0.f, 0.f, 0.f);
	m_pFullFrictionBox->RotateDegrees(0.f, 0.f, 0.f);
}
