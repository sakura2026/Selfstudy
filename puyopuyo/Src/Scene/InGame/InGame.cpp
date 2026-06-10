#include "InGame.h"
#include "../../Input/InputManager.h"
#include "DxLib.h"

// コンストラクタ
InGame::InGame()
{
}

// デストラクタ
InGame::~InGame()
{
	InGame::Finalize();
}

// 初期化処理
void InGame::Init()
{
	// いまはない
}

eSceneType InGame::Update()
{
	InputManager& input = InputManager::GetInstance();
	
	// スペースキーを押したらタイトルに戻る
	if (input.GetKeyState(KEY_INPUT_SPACE) == eInputState::ePressed)
	{
		return eTitle;
	}

	return eInGame;
}

// 描画処理
void InGame::Draw() const
{
	int color = GetColor(255, 255, 255);
	DrawBox(BOX_LEFT_X, BOX_LEFT_Y, BOX_RIGHT_X, BOX_RIGHT_Y, color, FALSE);
#ifdef _DEBUG

	// デバッグ用マス目
	for (int row = 0; row < ROW; row++)
	{
		for (int col = 0; col < COL; col++)
		{
			// マスの左上座標を計算
			int x1 = BOX_LEFT_X + col * CELL_W;
			int y1 = BOX_LEFT_Y + row * CELL_H;

			// マスの右上座標を計算
			int x2 = x1 + CELL_W;
			int y2 = y1 + CELL_H;

			// マスの枠を描画する
			DrawBox(x1, y1, x2, y2, color, FALSE);
		}
	}
#endif
	DrawString(100, 300, "InGame", GetColor(255, 255, 255));
}

// 終了処理
void InGame::Finalize()
{
}