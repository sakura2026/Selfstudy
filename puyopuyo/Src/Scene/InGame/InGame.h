#pragma once
#include"../SceneBase.h"

class InGame :public SceneBase
{
public:
	InGame();
	~InGame();

	// 初期化
	void Init() override;

	// 更新処理
	eSceneType Update() override;

	// 描画処理
	void Draw() const override;

	// 終了処理
	void Finalize() override;

private:
	// ボックスの情報
	static const int BOX_LEFT_X  = 450;  // ボックスの左上端X
	static const int BOX_LEFT_Y  = 30;   // ボックスの左上端Y
	static const int BOX_RIGHT_X = 830;  // ボックスの右下端X
	static const int BOX_RIGHT_Y = 690;  // ボックスの右下端Y

	// デバッグ用変数：マスの数
	static const int COL =  6;  // 横6列
	static const int ROW = 12;  // 縦12行

	// デバッグ用変数：マス1つのサイズを計算する
	static const int CELL_W = (BOX_RIGHT_X - BOX_LEFT_X) / COL;  // 横幅
	static const int CELL_H = (BOX_RIGHT_Y - BOX_LEFT_Y) / ROW;  // 縦幅
};