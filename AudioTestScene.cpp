#include "stdafx.h"
#include "AudioTestScene.h"

#include "SpherePosColorNorm.h"
#include "TorusPosColorNorm.h"
#include "CylinderPosColorNorm.h"
#include "Logger.h"
#include "SoundManager.h"

enum InputIds : int
{
	Play2D, Play3D, IncreaseVolume, DecreaseVolume, Left, Right
};

void AudioTestScene::Initialize()
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

	//Input actions
	const auto pInput = m_SceneContext.GetInput();
	pInput->AddInputAction({ Play2D, InputTriggerState::pressed, VK_SPACE });
	pInput->AddInputAction({ Play3D, InputTriggerState::pressed,'B' });
	pInput->AddInputAction({ IncreaseVolume, InputTriggerState::pressed, VK_UP });
	pInput->AddInputAction({ DecreaseVolume, InputTriggerState::pressed, VK_DOWN });

	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Right,InputTriggerState::down, VK_RIGHT,-1,XINPUT_GAMEPAD_DPAD_RIGHT});
	m_SceneContext.GetInput()->AddInputAction(InputAction{ InputIds::Left,InputTriggerState::down,VK_LEFT,-1,XINPUT_GAMEPAD_DPAD_LEFT });

	const auto pFmod = SoundManager::GetInstance()->GetSystem();

	//Sound 2D
	//Sound effect - rechtstreeks ingeladen - pFmod->createSound()
	//Background music - in een buffer op hardeschijf - pFmod->createStream()

	FMOD::Sound* pSound2D{ nullptr };
	auto result = pFmod->createStream("Resources/Sounds/wave.mp3", FMOD_2D | FMOD_LOOP_NORMAL, nullptr, &pSound2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	//Assignen aan een channel (ipv nullptr kan je een channel group doorgeven)
	result = pFmod->playSound(pSound2D, nullptr, true, &m_pChannel2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	//Sound 3D

	FMOD::Sound* pSound3D{ nullptr };
	result = pFmod->createStream("Resources/Sounds/Thrones.mp3", FMOD_3D | FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF, nullptr, &pSound3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound3D, nullptr, true, &m_pChannel3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	m_pChannel3D->set3DMinMaxDistance(0.f, 200.f);
}

inline FMOD_VECTOR ToFmod(XMFLOAT3 v) //DirectX
{
	return FMOD_VECTOR(v.x, v.y, v.z); //FMod
}

inline FMOD_VECTOR ToFmod(PxVec3 v) //PhysX
{
	return FMOD_VECTOR(v.x, v.y, v.z); //FMod
}

void AudioTestScene::Update()
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

	//Sound 
	if (m_SceneContext.GetInput()->IsActionTriggered(Play2D))
	{
		bool bPaused{ false };
		m_pChannel2D->getPaused(&bPaused);
		m_pChannel2D->setPaused(!bPaused);
	}

	//Sound 
	if (m_SceneContext.GetInput()->IsActionTriggered(Play3D))
	{
		bool bPaused{ false };
		m_pChannel3D->getPaused(&bPaused);
		m_pChannel3D->setPaused(!bPaused);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(IncreaseVolume))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);
	
		volume += 0.1f;
		Clamp(volume, 1.f, 0.f);
	
		m_pChannel2D->setVolume(volume);
		m_pChannel3D->setVolume(volume);

		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume changed >> %f", volume);
	}
	
	if (m_SceneContext.GetInput()->IsActionTriggered(DecreaseVolume))
	{
		float volume{};
		m_pChannel2D->getVolume(&volume);
	
		volume -= 0.1f;
		Clamp(volume, 1.f, 0.f);
	
		m_pChannel2D->setVolume(volume);
		m_pChannel3D->setVolume(volume);

		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume changed >> %f", volume);
	}

	//3D Sound Attributes
	auto pCam = m_SceneContext.GetCamera();
	auto pos = ToFmod(pCam->GetPosition());
	auto forward = ToFmod(pCam->GetForward());
	auto up = ToFmod(pCam->GetUp());
	
	FMOD_VECTOR vel{};
	const float deltaT = m_SceneContext.GetGameTime()->GetElapsed();
	vel.x = (pos.x - m_prevCamPos.x) / deltaT;
	vel.y = (pos.y - m_prevCamPos.y) / deltaT;
	vel.z = (pos.z - m_prevCamPos.z) / deltaT;
	m_prevCamPos = pos;

	SoundManager::GetInstance()->GetSystem()->set3DListenerAttributes(0, &pos, &vel, &forward, &up);

	auto spherePos = ToFmod(m_pSphere->GetPosition());
	auto sphereVel = ToFmod(m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->getLinearVelocity());
}

void AudioTestScene::Draw() const
{

}

