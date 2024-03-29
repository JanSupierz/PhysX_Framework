#pragma once
#include "GameScene.h"
class FrictionTestScene final : public GameScene
{
public:
	FrictionTestScene() :GameScene(L"FrictionTestScene") {}
	~FrictionTestScene() override = default;
	FrictionTestScene(const FrictionTestScene& other) = delete;
	FrictionTestScene(FrictionTestScene&& other) noexcept = delete;
	FrictionTestScene& operator=(const FrictionTestScene& other) = delete;
	FrictionTestScene& operator=(FrictionTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	void ResetPositions() const;

	GameObject* m_pNoFrictionBox{ nullptr };
	GameObject* m_pHalfFrictionBox{ nullptr };
	GameObject* m_pFullFrictionBox{ nullptr };

	const float m_StartHeight{ 8.f };
	const float m_StartXOffset{ 10.f };
};