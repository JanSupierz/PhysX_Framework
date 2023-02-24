#include "stdafx.h"
#include "BouncingBallScene.h"
#include "SpherePosColorNorm.h"

void BouncingBallScene::Initialize()
{
	//Physics
	EnablePhysxDebugRendering(true);
	const auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };

	//Sphere constants
	constexpr float actorRadius{ 1.f };
	constexpr int nrDivisions{ 15 };
	const XMFLOAT4 color{ Colors::Green};
	const PxSphereGeometry sphereGeometry{ actorRadius };

	//Not bouncy
	const PxMaterial* pNotBouncyMaterial{ pPhysX->createMaterial(0.5f,0.5f,0.f) };

	m_pNotBouncySphere = new SpherePosColorNorm(actorRadius, nrDivisions, nrDivisions, color);
	AddGameObject(m_pNotBouncySphere);

	PxRigidDynamic* pNotBouncySphereActor{ pPhysX->createRigidDynamic(PxTransform{PxIdentity}) };
	PxRigidActorExt::createExclusiveShape(*pNotBouncySphereActor, sphereGeometry, *pNotBouncyMaterial);

	m_pNotBouncySphere->AttachRigidActor(pNotBouncySphereActor);

	//Default
	const PxMaterial* pDefaultMaterial{ pPhysX->createMaterial(0.5f,0.5f,0.5f) };

	m_pDefaultSphere = new SpherePosColorNorm(actorRadius, nrDivisions, nrDivisions, color);
	AddGameObject(m_pDefaultSphere);

	PxRigidDynamic* pDefaultSphereActor{ pPhysX->createRigidDynamic(PxTransform{PxIdentity}) };
	PxRigidActorExt::createExclusiveShape(*pDefaultSphereActor, sphereGeometry, *pDefaultMaterial);

	m_pDefaultSphere->AttachRigidActor(pDefaultSphereActor);

	//Bouncy
	const PxMaterial* pBouncyMaterial{ pPhysX->createMaterial(0.5f,0.5f,1.f) };

	m_pBouncySphere = new SpherePosColorNorm(actorRadius, nrDivisions, nrDivisions, color);
	AddGameObject(m_pBouncySphere);

	PxRigidDynamic* pBouncySphereActor{ pPhysX->createRigidDynamic(PxTransform{PxIdentity}) };
	PxRigidActorExt::createExclusiveShape(*pBouncySphereActor, sphereGeometry, *pBouncyMaterial);

	m_pBouncySphere->AttachRigidActor(pBouncySphereActor);
	
	//Translate
	ResetPositions();

	//Ground plane
	const auto pGroundActor{ pPhysX->createRigidStatic(PxTransform{PxQuat{PxPiDivTwo,PxVec3{0,0,1}}}) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);
}

void BouncingBallScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		ResetPositions();
	}
}

void BouncingBallScene::Draw() const
{
}

void BouncingBallScene::OnSceneActivated()
{
}

void BouncingBallScene::OnSceneDeactivated()
{
}

void BouncingBallScene::ResetPositions() const
{
	m_pNotBouncySphere->Translate(-m_StartXOffset, m_StartHeight, 0.f);
	m_pDefaultSphere->Translate(0.f, m_StartHeight, 0.f);
	m_pBouncySphere->Translate(m_StartXOffset, m_StartHeight, 0.f);
}
