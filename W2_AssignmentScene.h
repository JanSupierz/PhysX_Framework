#pragma once
#include "GameScene.h"
#include "Logger.h"

class W2_AssignmentScene final : public GameScene
{
public:
	W2_AssignmentScene() :GameScene(L"W2_AssignmentScene") {}
	~W2_AssignmentScene() override = default;
	W2_AssignmentScene(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene(W2_AssignmentScene&& other) noexcept = delete;
	W2_AssignmentScene& operator=(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene& operator=(W2_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;

private:
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
	void Reset();

	GameObject* m_pSphere{ nullptr };
	GameObject* m_pSphereSmall0{ nullptr };
	GameObject* m_pSphereSmall1{ nullptr };
	GameObject* m_pLevelConvex{ nullptr };

	GameObject* m_pRedCube{ nullptr };
	GameObject* m_pBlueCube{ nullptr };

	PxRigidStatic* m_pTriggerRed{ nullptr };
	PxRigidStatic* m_pTriggerBlue{ nullptr };

	FMOD::Channel* m_pChannel2D{ nullptr };
	FMOD::Sound* m_pSound{ nullptr };

	bool m_IsTriggeredRed{};
	bool m_IsTriggeredBlue{};

	float m_HatchRedAngle{};
	float m_HatchBlueAngle{};

	GameObject* m_pHatchRed{ nullptr };
	GameObject* m_pHatchBlue{ nullptr };
};
