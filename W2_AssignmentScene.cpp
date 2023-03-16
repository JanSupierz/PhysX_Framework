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
	pTriggerBox->Translate(6.5f, 3.f, 0.f);
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
	pTriggerBox->Translate(-7.5f, 3.f, 0.f);
	pTriggerBox->AttachRigidActor(m_pTriggerBlue);

	//Hatches
	constexpr float upperLimit{ 0.f };
	const float lowerLimit{ -PxPi / 2.f };

	const XMFLOAT3 hatchSize{ 2.2f,0.3f,5.f };
	PxBoxGeometry hatchGeo{ PxBoxGeometry{ hatchSize.x / 2.f, hatchSize.y / 2.f, hatchSize.z / 2.f } };

	//Blue Hatch
	GameObject* pHatch{ new CubePosColorNorm(hatchSize.x, hatchSize.y, hatchSize.z, XMFLOAT4{ Colors::Blue }) };
	AddGameObject(pHatch);

	auto pHatchActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidActorExt::createExclusiveShape(*pHatchActor, hatchGeo, *pDefaultMaterial);
	pHatch->AttachRigidActor(pHatchActor);
	pHatch->Translate(-9.f, 17.f, 0.f);

	//Blue anchor
	auto pAnchor = pPhysX->createRigidDynamic(PxTransform{ -9.f,17.f,0.f });
	pAnchor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	m_pPhysxScene->addActor(*pAnchor);

	//Blue joint
	m_pBlueJoint = PxRevoluteJointCreate(*pPhysX, pAnchor, PxTransform{ 0.f,0.f,0.f }, pHatchActor, PxTransform{ 0.f,0.f,0.f });
	m_pBlueJoint->setLimit(PxJointAngularLimitPair(lowerLimit, upperLimit, 0.01f));

	m_pBlueJoint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	m_pBlueJoint->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
	m_pBlueJoint->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);

	//Local pose
	PxTransform anchorLocalPose{ PxVec3{-1.f,0.f,0.f}, PxQuat(PxHalfPi, PxVec3(0, -1, 0)) };
	PxTransform hatchLocalPose{ PxVec3{-1.f,0.f,0.f}, PxQuat(PxHalfPi, PxVec3(0, -1, 0)) };
	m_pBlueJoint->setLocalPose(PxJointActorIndex::eACTOR0, anchorLocalPose);
	m_pBlueJoint->setLocalPose(PxJointActorIndex::eACTOR1, hatchLocalPose);

	//Red Hatch
	pHatch = new CubePosColorNorm(hatchSize.x, hatchSize.y, hatchSize.z, XMFLOAT4{ Colors::Red });
	AddGameObject(pHatch);

	pHatchActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pHatchActor, hatchGeo, *pDefaultMaterial);
	pHatch->AttachRigidActor(pHatchActor);
	pHatch->Translate(9.f, 17.f, 0.f);

	//Red anchor
	pAnchor = pPhysX->createRigidDynamic(PxTransform{ 9.f,17.f,0.f });
	pAnchor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	m_pPhysxScene->addActor(*pAnchor);

	//Red joint
	m_pRedJoint = PxRevoluteJointCreate(*pPhysX, pAnchor, PxTransform{ 0.f,0.f,0.f }, pHatchActor, PxTransform{ 0.f,0.f,0.f });
	m_pRedJoint->setLimit(PxJointAngularLimitPair(lowerLimit, upperLimit, 0.01f));

	m_pRedJoint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	m_pRedJoint->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
	m_pRedJoint->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);

	//Local pose
	anchorLocalPose = PxTransform{ PxVec3{1.f,0.f,0.f}, PxQuat(PxHalfPi, PxVec3(0, 1, 0)) };
	hatchLocalPose = PxTransform{ PxVec3{1.f,0.f,0.f}, PxQuat(PxHalfPi, PxVec3(0, 1, 0)) };
	m_pRedJoint->setLocalPose(PxJointActorIndex::eACTOR0, anchorLocalPose);
	m_pRedJoint->setLocalPose(PxJointActorIndex::eACTOR1, hatchLocalPose);

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
	constexpr float speed{ -2.f };

	if (m_ShouldOpenBlue)
	{
		m_pBlueJoint->setDriveVelocity(speed);
		m_ShouldOpenBlue = false;
	}

	if (m_ShouldOpenRed)
	{
		m_pRedJoint->setDriveVelocity(speed);
		m_ShouldOpenRed = false;
	}
}

void W2_AssignmentScene::Reset()
{
	//Reset spheres
	constexpr float sphereY{ 8.f };

	m_pSphere->Translate(0.f, sphereY, 0.f);
	m_pSphere->RotateDegrees(0.f, 0.f, 0.f);

	constexpr float smallSphereX{ 9.f }, smallSphereY{ 20.f };

	m_pSphereSmall0->Translate(smallSphereX, smallSphereY, 0.f);
	m_pSphereSmall0->RotateDegrees(0.f, 0.f, 0.f);
			
	m_pSphereSmall1->Translate(-smallSphereX, smallSphereY, 0.f);
	m_pSphereSmall1->RotateDegrees(0.f, 0.f, 0.f);

	//Reset Boxes
	constexpr float boxX{ 2.f }, boxY{ 7.f };
	m_pRedCube->Translate(boxX, boxY, 0.f);
	m_pRedCube->RotateDegrees(0.f, 0.f, 0.f);

	m_pBlueCube->Translate(-boxX, boxY, 0.f);
	m_pBlueCube->RotateDegrees(0.f, 0.f, 0.f);

	//Triggers
	m_IsTriggeredBlue = false;
	m_IsTriggeredRed = false;
	m_ShouldOpenBlue = false;
	m_ShouldOpenRed = false;

	//Joints
	m_pBlueJoint->setDriveVelocity(8);
	m_pRedJoint->setDriveVelocity(8);
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
				m_ShouldOpenRed = true;
				m_IsTriggeredRed = true;
				SoundManager::GetInstance()->GetSystem()->playSound(m_pSound, nullptr, false, &m_pChannel2D);
			}
		}

		if (pair.triggerActor == m_pTriggerBlue && pair.otherActor == m_pBlueCube->GetRigidActor())
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				m_ShouldOpenBlue = true;
				m_IsTriggeredBlue = true;
				SoundManager::GetInstance()->GetSystem()->playSound(m_pSound, nullptr, false, &m_pChannel2D);
			}
		}
	}
}

void W2_AssignmentScene::Draw() const
{

}