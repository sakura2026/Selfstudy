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
	// とりあえず文字だけ表示
	DrawString(100, 300, "InGame", GetColor(255, 255, 255));
}

// 終了処理
void InGame::Finalize()
{
}