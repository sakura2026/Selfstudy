#pragma once
#include"../SceneBase.h"

class InGame :public SceneBase
{
public:
	InGame();
	~InGame();

	// ‰Šú‰»
	void Init() override;

	// XVˆ—
	eSceneType Update() override;

	// •`‰æˆ—
	void Draw() const override;

	// I—¹ˆ—
	void Finalize() override;
};