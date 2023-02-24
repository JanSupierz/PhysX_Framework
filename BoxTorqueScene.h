#pragma once
#include "GameScene.h"
class BoxTorqueScene final : public GameScene
{
public:
	BoxTorqueScene() :GameScene(L"BoxTorqueScene") {}
	~BoxTorqueScene() override = default;
	BoxTorqueScene(const BoxTorqueScene& other) = delete;
	BoxTorqueScene(BoxTorqueScene&& other) noexcept = delete;
	BoxTorqueScene& operator=(const BoxTorqueScene& other) = delete;
	BoxTorqueScene& operator=(BoxTorqueScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	void ResetPosition() const;

	GameObject* m_pHalfFrictionBox{ nullptr };
	PxRigidDynamic* m_pHalfFrictionActor{ nullptr };

	const float m_StartHeight{ 8.f };
};