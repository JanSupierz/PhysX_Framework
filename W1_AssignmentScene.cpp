#include "stdafx.h"
#include "W1_AssignmentScene.h"
#include "SpherePosColorNorm.h"
#include "CubePosColorNorm.h"

enum InputIds : int
{
	Up, Down, Left, Right
};

void W1_AssignmentScene::Initialize()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	//Physics
	EnablePhysxDebugRendering(true);
	const auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };

	//Sphere constants
	constexpr float sphereRadius{ 1.f };
	constexpr int sphereDivisions{ 15 };
	const XMFLOAT4 color{ Colors::Green };
	const PxSphereGeometry sphereGeometry{ sphereRadius };
	const PxMaterial* pSphereMaterial{ pPhysX->createMaterial(0.1f,0.2f,0.6f) };

	//Cube constants
	const PxBoxGeometry boxGeometry{ m_BoxSize / 2.f,m_BoxSize / 2.f,m_BoxSize / 2.f };
	const PxMaterial* pCubeMaterial{ pPhysX->createMaterial(0.8f,0.7f,0.2f) };

	//Sphere
	m_pSphere = new SpherePosColorNorm(sphereRadius, sphereDivisions, sphereDivisions, color);
	AddGameObject(m_pSphere);

	auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	pSphereActor->setMass(30.f);

	PxRigidActorExt::createExclusiveShape(*pSphereActor, sphereGeometry, *pSphereMaterial);

	m_pSphere->AttachRigidActor(pSphereActor);

	//Cubes
	for (int index{}; index < m_NrRows * m_NrColumns; ++index)
	{
		m_pCubes.push_back(new CubePosColorNorm(m_BoxSize, m_BoxSize, m_BoxSize));
		AddGameObject(m_pCubes[index]);

		PxRigidDynamic* pBoxActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
		PxRigidActorExt::createExclusiveShape(*pBoxActor, boxGeometry, *pCubeMaterial);

		m_pCubes[index]->AttachRigidActor(pBoxActor);
	}

	//Translate
	ResetPositions();

	//Ground plane
	const PxMaterial* pDefaultMaterial{ pPhysX->createMaterial(0.4f,1.f,0.2f) };

	const auto pGroundActor{ pPhysX->createRigidStatic(PxTransform{PxQuat{PxPiDivTwo,PxVec3{0,0,1}}}) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Input - Last two are optional (mouse, game pad)
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Down,InputTriggerState::down,VK_DOWN });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Up,InputTriggerState::down,VK_UP });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Right,InputTriggerState::down,VK_RIGHT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Left,InputTriggerState::down,VK_LEFT });
}

inline PxVec3 ToPxVec3(XMFLOAT3 v)
{
	return(PxVec3(v.x, v.y, v.z));
}

void W1_AssignmentScene::Update()
{
	//Reset
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		ResetPositions();
	}

	//Move ball
	constexpr float forceValue{ 80.f };

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Left))
	{
		const auto torque{ ToPxVec3(m_SceneContext.GetCamera()->GetForward()).getNormalized() * forceValue };
		static_cast<PxRigidBody*>(m_pSphere->GetRigidActor())->addTorque(torque, PxForceMode::eACCELERATION);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Right))
	{
		const auto torque{ ToPxVec3(m_SceneContext.GetCamera()->GetForward()).getNormalized() * -forceValue };
		static_cast<PxRigidBody*>(m_pSphere->GetRigidActor())->addTorque(torque, PxForceMode::eACCELERATION);
	}
	
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Up))
	{
		const auto torque{ ToPxVec3(m_SceneContext.GetCamera()->GetRight()).getNormalized() * forceValue };
		static_cast<PxRigidBody*>(m_pSphere->GetRigidActor())->addTorque(torque, PxForceMode::eACCELERATION);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Down))
	{
		const auto torque{ ToPxVec3(m_SceneContext.GetCamera()->GetRight()).getNormalized() * -forceValue };
		static_cast<PxRigidBody*>(m_pSphere->GetRigidActor())->addTorque(torque, PxForceMode::eACCELERATION);
	}
}

void W1_AssignmentScene::Draw() const
{
}

void W1_AssignmentScene::ResetPositions() const
{
	//Reset sphere
	m_pSphere->Translate(0.f, m_StartHeight, 0.f);
	m_pSphere->RotateDegrees(0.f, 0.f, 0.f);

	//Reset wall
	for (int indexY{}; indexY < m_NrRows; ++indexY)
	{
		for (int indexX{}; indexX < m_NrColumns; ++indexX)
		{
			const float height{ indexY * (m_BoxSize + m_BoxOffset) + m_BoxSize + m_StartHeight / 2.f };
			m_pCubes[indexY * m_NrColumns + indexX]->Translate(indexX * (m_BoxSize + m_BoxOffset), height, 10.f);

			const float randomRotationY{ static_cast<float>(rand() % 21 - 10) };
			m_pCubes[indexY * m_NrColumns + indexX]->RotateDegrees(0.f, randomRotationY, 0.f);
		}
	}
}
