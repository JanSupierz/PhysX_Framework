#include "stdafx.h"
#include "PhysXTestScene.h"
#include "CubePosColorNorm.h"

void PhysXTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };
	const PxMaterial* pDefaultMaterial{ pPhysX->createMaterial(0.5f,0.5f,0.7f) };

	//Cube
	const XMFLOAT3 actorDimension{ 1.f,1.f,1.f };
	m_pCube = new CubePosColorNorm(actorDimension.x, actorDimension.y, actorDimension.z);
	AddGameObject(m_pCube);

	//Cube actor
	PxRigidDynamic* pCubeActor{ pPhysX->createRigidDynamic(PxTransform{PxIdentity}) };
	PxBoxGeometry boxGeometry{ 0.5f * actorDimension.x, 0.5f * actorDimension.y, 0.5f * actorDimension.z };

	//PxShape* pCubeShape{ pPhysX->createShape(boxGeometry,*pDefaultMaterial,true) };
	//pCubeActor->attachShape(*pCubeShape);
	//m_pPhysxScene->addActor(*pCubeActor);

	//OR

	PxRigidActorExt::createExclusiveShape(*pCubeActor, boxGeometry, *pDefaultMaterial);

	//Link Cube with Cube_Actor
	m_pCube->AttachRigidActor(pCubeActor);

	m_pCube->Translate(0.f, 10.f, 0.f);
	m_pCube->RotateDegrees(30.f, 20.f, 40.f);

	//Ground plane
	const auto pGroundActor{ pPhysX->createRigidStatic(PxTransform{PxQuat{PxPiDivTwo,PxVec3{0,0,1}}}) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	m_SceneContext.GetCamera()->SetPositionAndLookAt({ 0,10,-30 }, { 0,5,0 });
}

void PhysXTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pCube->Translate(0.f, 10.f, 0.f);
	}
}

void PhysXTestScene::Draw() const
{
}

void PhysXTestScene::OnSceneActivated()
{
}

void PhysXTestScene::OnSceneDeactivated()
{
}
