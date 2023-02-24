#pragma once
#include "GameScene.h"
class BoxForceScene final : public GameScene
{
public:
	BoxForceScene() :GameScene(L"BoxForceScene") {}
	~BoxForceScene() override = default;
	BoxForceScene(const BoxForceScene& other) = delete;
	BoxForceScene(BoxForceScene&& other) noexcept = delete;
	BoxForceScene& operator=(const BoxForceScene& other) = delete;
	BoxForceScene& operator=(BoxForceScene&& other) noexcept = delete;

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