#pragma once
#include "GameScene.h"
class W1_AssignmentScene final : public GameScene
{
public:
	W1_AssignmentScene() :GameScene(L"W1_AssignmentScene") {}
	~W1_AssignmentScene() override = default;
	W1_AssignmentScene(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene(W1_AssignmentScene&& other) noexcept = delete;
	W1_AssignmentScene& operator=(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene& operator=(W1_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;

private:
	void ResetPositions() const;

	GameObject* m_pSphere{ nullptr };


	const float m_StartHeight{ 0.1f };
	const float m_BoxSize{ 1.f };
	const float m_BoxOffset{ 0.2f };
	const int m_NrRows{ 8 }, m_NrColumns{ 10 };

	std::vector<GameObject*> m_pCubes{};
};