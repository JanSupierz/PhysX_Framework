#include "stdafx.h"
#include "PhysiXMeshTestScene.h"

#include "CubePosColorNorm.h"
#include "TorusPosColorNorm.h"
#include "CylinderPosColorNorm.h"
#include "Logger.h"

#include "ContentManager.h"
#include "MeshObject.h"

enum InputIds : int
{
	Up, Down, Left, Right
};

void PhysiXMeshTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);

	//Ground plane
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo,{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Convex chair
	m_pChairConvex = new MeshObject(L"Resources/Meshes/ChairPhysiX.ovm");
	AddGameObject(m_pChairConvex);

	const auto pConvexMesh = ContentManager::GetInstance()->Load<PxConvexMesh>(L"Resources/Meshes/ChairPhysiX.ovpc");
	const auto pConvexActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pConvexActor, PxConvexMeshGeometry{ pConvexMesh }, *pDefaultMaterial);
	
	//PxVec3 massLocalPose{ 0.f,4.f,0.f };
	//PxRigidBodyExt::updateMassAndInertia(*pConvexActor, 10.f, &massLocalPose);

	m_pChairConvex->AttachRigidActor(pConvexActor);
	m_pChairConvex->Translate(0, 120.f, 0);

	//Triangle Chair
	m_pChairTriangle = new MeshObject(L"Resources/Meshes/ChairPhysiX.ovm");
	AddGameObject(m_pChairTriangle);

	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/ChairPhysiX.ovpt");
	const auto pTriangleActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	pTriangleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

	m_pChairTriangle->AttachRigidActor(pTriangleActor);
	m_pChairTriangle->Translate(10.f, 0.f, 0.f);
	m_KinematicPosition = { 10.f,0.f,0.f };
}

void PhysiXMeshTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pChairConvex->Translate(0.f, 12.f, 0.f);
		m_pChairConvex->Rotate(0.f, 0.f, 0.f);

		m_KinematicPosition = { 10.f,0.f,0.f };
		m_pChairTriangle->Translate(10.f, 0.f, 0.f);
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_KinematicPosition.x -= 5.f * m_SceneContext.GetGameTime()->GetElapsed();
		m_pChairTriangle->GetRigidActor()->is<PxRigidDynamic>()->setKinematicTarget(PxTransform{ m_KinematicPosition });
	}
}

void PhysiXMeshTestScene::Draw() const
{

}
