#pragma once
#include "GameScene.h"
class BouncingBallScene final : public GameScene
{
public:
	BouncingBallScene() :GameScene(L"BouncingBallScene") {}
	~BouncingBallScene() override = default;
	BouncingBallScene(const BouncingBallScene& other) = delete;
	BouncingBallScene(BouncingBallScene&& other) noexcept = delete;
	BouncingBallScene& operator=(const BouncingBallScene& other) = delete;
	BouncingBallScene& operator=(BouncingBallScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	void ResetPositions() const;

	GameObject* m_pBouncySphere{ nullptr };
	GameObject* m_pDefaultSphere{ nullptr };
	GameObject* m_pNotBouncySphere{ nullptr };
	
	const float m_StartHeight{ 10.f };
	const float m_StartXOffset{ 8.f };
};