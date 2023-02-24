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
	srand(std::time(nullptr));

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

	m_pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	m_pSphereActor->setMass(10.f);
	PxRigidActorExt::createExclusiveShape(*m_pSphereActor, sphereGeometry, *pSphereMaterial);

	m_pSphere->AttachRigidActor(m_pSphereActor);

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

void W1_AssignmentScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		ResetPositions();
	}

	constexpr float forceValue{ 10000.f };
	const float torque{ forceValue * m_SceneContext.GetGameTime()->GetElapsed() };

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Left))
	{
		auto direction{ m_SceneContext.GetCamera()->GetRight() };
		const auto vector{ -torque * XMLoadFloat3(&direction) };
		XMStoreFloat3(&direction, vector);

		m_pSphereActor->addTorque({ direction.z,direction.y,-direction.x });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Right))
	{
		auto direction{ m_SceneContext.GetCamera()->GetRight() };
		const auto vector{ torque * XMLoadFloat3(&direction) };
		XMStoreFloat3(&direction, vector);

		m_pSphereActor->addTorque({ direction.z,direction.y,-direction.x });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Up))
	{
		auto direction{ m_SceneContext.GetCamera()->GetForward() };
		const auto vector{ torque * XMLoadFloat3(&direction) };
		XMStoreFloat3(&direction, vector);

		m_pSphereActor->addTorque({ direction.z,direction.y,-direction.x });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Down))
	{
		auto direction{ m_SceneContext.GetCamera()->GetForward() };
		const auto vector{ -torque * XMLoadFloat3(&direction) };
		XMStoreFloat3(&direction, vector);

		m_pSphereActor->addTorque({ direction.z,direction.y,-direction.x });
	}
}

void W1_AssignmentScene::Draw() const
{
}

void W1_AssignmentScene::OnSceneActivated()
{
}

void W1_AssignmentScene::OnSceneDeactivated()
{
}

void W1_AssignmentScene::ResetPositions() const
{
	m_pSphere->Translate(0.f, m_StartHeight, 0.f);
	m_pSphere->RotateDegrees(0.f, 0.f, 0.f);

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
