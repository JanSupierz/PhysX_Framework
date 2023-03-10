#include "stdafx.h"
#include "W2_AssignmentScene.h"

#include "SpherePosColorNorm.h"
#include "CubePosColorNorm.h"

#include "Logger.h"
#include "SoundManager.h"

#include "ContentManager.h"
#include "MeshObject.h"

enum InputIds : int
{
	Left, Right, Jump
};

void W2_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = pPhysX->createMaterial(0.8f, 0.8f, 0.2f);

	//Ground plane
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo,{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Level
	m_pLevelConvex = new MeshObject(L"Resources/Meshes/Level.ovm");
	AddGameObject(m_pLevelConvex);

	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Level.ovpt");
	const auto pTriangleActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });

	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);
	m_pLevelConvex->AttachRigidActor(pTriangleActor);

	//Sphere
	constexpr float sphereMass{ 20.f };
	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphere);

	auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	PxRigidBodyExt::setMassAndUpdateInertia(*pSphereActor, sphereMass);

	//Lock sphere in xy-plane
	auto d6Joint = PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pSphereActor, PxTransform{ PxIdentity });

	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	m_pSphere->AttachRigidActor(pSphereActor);

	//Small sphere0
	constexpr float smallSphereSize{ 0.5f };
	m_pSphereSmall0 = new SpherePosColorNorm(smallSphereSize, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphereSmall0);

	pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ smallSphereSize }, *pDefaultMaterial);
	PxRigidBodyExt::setMassAndUpdateInertia(*pSphereActor, sphereMass);

	//Lock sphere0 in xy-plane
	d6Joint = PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pSphereActor, PxTransform{ PxIdentity });

	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	m_pSphereSmall0->AttachRigidActor(pSphereActor);

	//Small sphere1
	m_pSphereSmall1 = new SpherePosColorNorm(smallSphereSize, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphereSmall1);

	pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ smallSphereSize }, *pDefaultMaterial);
	PxRigidBodyExt::setMassAndUpdateInertia(*pSphereActor, sphereMass);

	//Lock sphere1 in xy-plane
	d6Joint = PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pSphereActor, PxTransform{ PxIdentity });

	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	m_pSphereSmall1->AttachRigidActor(pSphereActor);

	//Boxes
	constexpr float boxSize{ 1.5f };
	constexpr float boxMass{ 0.1f };
	const PxBoxGeometry boxGeometry{ boxSize / 2.f,boxSize / 2.f,boxSize / 2.f };

	//Red cube
	m_pRedCube = new CubePosColorNorm(boxSize, boxSize, boxSize, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pRedCube);

	PxRigidDynamic* pBoxActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pBoxActor, boxGeometry, *pDefaultMaterial);
	PxRigidBodyExt::setMassAndUpdateInertia(*pBoxActor, boxMass);
	
	//Lock redCube in xy-plane
	d6Joint = PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pBoxActor, PxTransform{ PxIdentity });

	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	m_pRedCube->AttachRigidActor(pBoxActor);
	
	//Blue cube
	m_pBlueCube = new CubePosColorNorm(boxSize, boxSize, boxSize, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pBlueCube);

	pBoxActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pBoxActor, boxGeometry, *pDefaultMaterial);
	PxRigidBodyExt::setMassAndUpdateInertia(*pBoxActor, boxMass);

	//Lock blueCube in xy-plane
	d6Joint = PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pBoxActor, PxTransform{ PxIdentity });

	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	m_pBlueCube->AttachRigidActor(pBoxActor);

	//Trigger red
	const auto triggerGeometry{ PxBoxGeometry{ 1.f,0.5f,3.f } };

	auto pTriggerBox = new CubePosColorNorm
	(
		triggerGeometry.halfExtents.x * 2.f,
		triggerGeometry.halfExtents.y * 2.f,
		triggerGeometry.halfExtents.z * 2.f,
		XMFLOAT4{ Colors::DarkRed }
	);

	m_pTriggerRed = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRed, triggerGeometry, *pDefaultMaterial);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	AddGameObject(pTriggerBox);
	pTriggerBox->Translate(-7.5f, 3.f, 0.f);
	pTriggerBox->AttachRigidActor(m_pTriggerRed);

	//Trigger blue

	pTriggerBox = new CubePosColorNorm
	(
		triggerGeometry.halfExtents.x * 2.f,
		triggerGeometry.halfExtents.y * 2.f,
		triggerGeometry.halfExtents.z * 2.f,
		XMFLOAT4{ Colors::DarkBlue }
	);

	m_pTriggerBlue = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerBlue, triggerGeometry, *pDefaultMaterial);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	AddGameObject(pTriggerBox);
	pTriggerBox->Translate(6.5f, 3.f, 0.f);
	pTriggerBox->AttachRigidActor(m_pTriggerBlue);

	//Red hatch
	const auto hatchDimensions = XMFLOAT3{ 2.f,0.2f,5.f };
	constexpr float hatchPosX{ 9.f }, hatchPosY{ 16.f };
	const auto hatchGeometry = PxBoxGeometry{ hatchDimensions.x / 2.f,hatchDimensions.y / 2.f,hatchDimensions.z / 2.f };

	m_pHatchRed = new CubePosColorNorm(hatchDimensions.x, hatchDimensions.y, hatchDimensions.z, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pHatchRed);

	auto pHatch = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pHatch, hatchGeometry, *pDefaultMaterial);
	pHatch->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	pHatch->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	m_pHatchRed->AttachRigidActor(pHatch);
	m_pHatchRed->Translate(-hatchPosX, hatchPosY, 0.f);

	//Blue hatch
	m_pHatchBlue = new CubePosColorNorm(hatchDimensions.x, hatchDimensions.y, hatchDimensions.z, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pHatchBlue);

	pHatch = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pHatch, hatchGeometry, *pDefaultMaterial);
	pHatch->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	pHatch->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	m_pHatchBlue->AttachRigidActor(pHatch);
	m_pHatchBlue->Translate(hatchPosX, hatchPosY, 0.f);

	//Set start positions
	Reset();

	//Input actions
	const auto pInput = m_SceneContext.GetInput();
	pInput->AddInputAction(InputAction{ InputIds::Right,InputTriggerState::down, VK_RIGHT,-1,XINPUT_GAMEPAD_DPAD_RIGHT });
	pInput->AddInputAction(InputAction{ InputIds::Left,InputTriggerState::down,VK_LEFT,-1,XINPUT_GAMEPAD_DPAD_LEFT });
	pInput->AddInputAction(InputAction{ InputIds::Jump,InputTriggerState::pressed,VK_SPACE,-1,XINPUT_GAMEPAD_A });

	const auto pFmod = SoundManager::GetInstance()->GetSystem();

	//Sound 2D
	auto result = pFmod->createStream("Resources/Sounds/bell.mp3", FMOD_2D | FMOD_LOOP_OFF, nullptr, &m_pSound);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(m_pSound, nullptr, true, &m_pChannel2D);
	SoundManager::GetInstance()->ErrorCheck(result);
}

void W2_AssignmentScene::Update()
{
	//Reset scene
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, 'R'))
	{
		Reset();
	}

	//Move ball
	constexpr float force{ 15.f };
	if (m_SceneContext.GetInput()->IsActionTriggered(Left))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,force }, PxForceMode::eACCELERATION);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(Right))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,-force }, PxForceMode::eACCELERATION);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(Jump))
	{
		constexpr float jumpForce{ 200.f };
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0,jumpForce,0 }, PxForceMode::eIMPULSE);
	}

	//Move hatches
	constexpr float speed{ 60.f };
	constexpr float maxAngle{ 90.f };

	if (m_IsTriggeredRed && m_HatchRedAngle > -maxAngle)
	{
		m_HatchRedAngle -= m_SceneContext.GetGameTime()->GetElapsed() * speed;
		m_pHatchRed->RotateDegrees(0.f, 0.f, m_HatchRedAngle);
	}

	if (m_IsTriggeredBlue && m_HatchBlueAngle < maxAngle)
	{
		m_HatchBlueAngle += m_SceneContext.GetGameTime()->GetElapsed() * speed;
		m_pHatchBlue->RotateDegrees(0.f, 0.f, m_HatchBlueAngle);
	}
}

void W2_AssignmentScene::Reset()
{
	//Reset spheres
	constexpr float sphereY{ 8.f };

	m_pSphere->Translate(0.f, sphereY, 0.f);
	m_pSphere->RotateDegrees(0.f, 0.f, 0.f);

	constexpr float smallSphereX{ 9.f }, smallSphereY{ 18.f };

	m_pSphereSmall0->Translate(smallSphereX, smallSphereY, 0.f);
	m_pSphereSmall0->RotateDegrees(0.f, 0.f, 0.f);
			
	m_pSphereSmall1->Translate(-smallSphereX, smallSphereY, 0.f);
	m_pSphereSmall1->RotateDegrees(0.f, 0.f, 0.f);

	//Reset Boxes
	constexpr float boxX{ 2.f }, boxY{ 7.f };
	m_pRedCube->Translate(-boxX, boxY, 0.f);
	m_pRedCube->RotateDegrees(0.f, 0.f, 0.f);

	m_pBlueCube->Translate(boxX, boxY, 0.f);
	m_pBlueCube->RotateDegrees(0.f, 0.f, 0.f);

	//Triggers
	m_IsTriggeredBlue = false;
	m_IsTriggeredRed = false;

	//Hatches
	m_HatchBlueAngle = 0.f;
	m_HatchRedAngle = 0.f;

	m_pHatchRed->RotateDegrees(0.f, 0.f, m_HatchRedAngle);
	m_pHatchBlue->RotateDegrees(0.f, 0.f, m_HatchBlueAngle);
}

void W2_AssignmentScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (PxU32 index{}; index < count; ++index)
	{
		//ignore deleted shapes
		if (pairs[index].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		const PxTriggerPair& pair{ pairs[index] };

		if (pair.triggerActor == m_pTriggerRed && pair.otherActor == m_pRedCube->GetRigidActor())
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				m_IsTriggeredRed = true;
				SoundManager::GetInstance()->GetSystem()->playSound(m_pSound, nullptr, false, &m_pChannel2D);
			}
		}

		if (pair.triggerActor == m_pTriggerBlue && pair.otherActor == m_pBlueCube->GetRigidActor())
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				m_IsTriggeredBlue = true;
				SoundManager::GetInstance()->GetSystem()->playSound(m_pSound, nullptr, false, &m_pChannel2D);
			}
		}
	}
}

void W2_AssignmentScene::Draw() const
{

}