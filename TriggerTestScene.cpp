#include "stdafx.h"
#include "TriggerTestScene.h"

#include "SpherePosColorNorm.h"
#include "TorusPosColorNorm.h"
#include "CylinderPosColorNorm.h"
#include "Logger.h"

enum InputIds : int
{
	Up, Down, Left, Right
};

void TriggerTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = pPhysX->createMaterial(0.8f, 0.8f, 0.2f);

	//Ground plane
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ {PxPiDivTwo,{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//Sphere
	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(0.f, 2.f, 0.f);

	//Trigger left
	m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{ {-8.f,0.5f,0.f} });
	const auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{ 0.5f,0.5f,0.5f }, *pDefaultMaterial);
	
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	
	m_pPhysxScene->addActor(*m_pTriggerLeft);
	
	//Trigger right
	m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ {8.f,0.5f,0.f} });
	const auto pShapeR = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ 0.5f,0.5f,0.5f }, *pDefaultMaterial);

	pShapeR->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShapeR->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerRight);

	//Input - Last two are optional (mouse, game pad)
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Down,InputTriggerState::down,VK_DOWN,-1,XINPUT_GAMEPAD_DPAD_DOWN });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Up,InputTriggerState::down,VK_UP,-1,XINPUT_GAMEPAD_DPAD_UP });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Right,InputTriggerState::down,VK_RIGHT,-1,XINPUT_GAMEPAD_DPAD_RIGHT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Left,InputTriggerState::down,VK_LEFT,-1,XINPUT_GAMEPAD_DPAD_LEFT });
}

void TriggerTestScene::Update()
{
	constexpr float force{ 5.f };

	if (m_SceneContext.GetInput()->IsActionTriggered(Left))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,force });
	}
	else if (m_SceneContext.GetInput()->IsActionTriggered(Right))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0,0,-force });
	}

	if (m_IsTriggeredLeft)
	{
		m_IsTriggeredLeft = false;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0.f,10.f,0.f }, PxForceMode::eIMPULSE);
	}

	if (m_IsTriggeredRight)
	{
		m_IsTriggeredRight = false;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0.f,10.f,0.f }, PxForceMode::eIMPULSE);
	}
}

void TriggerTestScene::Draw() const
{

}

void TriggerTestScene::OnSceneActivated()
{

}

void TriggerTestScene::OnSceneDeactivated()
{
}

void TriggerTestScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{

		for (auto i{ 0 }; i < count; ++i)
		{
			//ignore deleted shapes
			if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				continue;

			const PxTriggerPair& pair = pairs[i];

			if (pair.triggerActor == m_pTriggerLeft)
			{
				if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //on enter
				{
					Logger::GetInstance()->LogInfo(L"Left on enter");
					m_IsTriggeredLeft = true;
				}
				else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //end
				{
					Logger::GetInstance()->LogInfo(L"Left on end");
				}
			}
			else if (pair.triggerActor == m_pTriggerRight)
			{
				if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) //on enter
				{
					Logger::GetInstance()->LogInfo(L"Right on enter");
					m_IsTriggeredRight = true;
				}
				else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) //end
				{
					Logger::GetInstance()->LogInfo(L"Right on end");
				}
			}
		}
	
}
