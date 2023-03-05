#pragma once
#include "GameScene.h"
class PhysiXMeshTestScene final : public GameScene
{
public:
	PhysiXMeshTestScene() :GameScene(L"PhysiXMeshTestScene") {}
	~PhysiXMeshTestScene() override = default;
	PhysiXMeshTestScene(const PhysiXMeshTestScene& other) = delete;
	PhysiXMeshTestScene(PhysiXMeshTestScene&& other) noexcept = delete;
	PhysiXMeshTestScene& operator=(const PhysiXMeshTestScene& other) = delete;
	PhysiXMeshTestScene& operator=(PhysiXMeshTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	GameObject* m_pChairConvex;
	GameObject* m_pChairTriangle;

	PxVec3 m_KinematicPosition{};
};

