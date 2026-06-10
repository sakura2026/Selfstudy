#include "InGame.h"
#include "../../Input/InputManager.h"
#include "DxLib.h"

// コンストラクタ
InGame::InGame()
	: m_puyo_Row(0)
	, m_lastTime(0)
	, m_isBottom(false)
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
	// 一番からスタート
	m_puyo_Row = 0;

	// 現在時間を記録する
	m_lastTime = GetNowCount();

	// まだ底についてない
	m_isBottom = false;
}

eSceneType InGame::Update()
{
	InputManager& input = InputManager::GetInstance();
	
	// 底についてなければ落下
	if (!m_isBottom)
	{
		// 現在の時間を取得する
		int nowTime = GetNowCount();
		
		// 1秒経過したか確認する
		if (nowTime - m_lastTime >= 1000)
		{
			// 1行下に落とす
			m_puyo_Row++;

			// 時間をリセットする
			m_lastTime = nowTime;

			// 一番下についたか確認する
			if (m_puyo_Row >= ROW - 1)
			{
				m_puyo_Row = ROW - 1;
				m_isBottom = true;
			}
		}
	}

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
	int White = GetColor(255, 255, 255);
	int red   = GetColor(255, 0, 0);
	DrawBox(BOX_LEFT_X, BOX_LEFT_Y, BOX_RIGHT_X, BOX_RIGHT_Y, White, FALSE);
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
			DrawBox(x1, y1, x2, y2, White, FALSE);
		}
	}
#endif

	// ぷよ描画
	for (int col = 2; col <= 3; col++)
	{
		// マスの左上座標を計算
		int x1 = BOX_LEFT_X + col * CELL_W;
		int y1 = BOX_LEFT_Y;

		// マスの右上座標を計算
		int x2 = x1 + CELL_W;
		int y2 = y1 + CELL_H;

		// ぷよを塗りつぶした円で描画
		int cx = (x1 + x2) / 2;  // マスの中心X
		int cy = (y1 + y2) / 2;  // マスの中心Y
		int r = CELL_W / 2 - 5;  // 円の半径

		// 左のぷよは右に、右のぷよは左にずらして近づける
		if (col == 2)
		{
			cx += 10;  // 右にずらす（数字を大きくするほど近づく）
		}
		if (col == 3)
		{
			cx -= 10;  // 左にずらす（数字を大きくするほど近づく）
		}

		DrawCircle(cx, cy, r, red, TRUE);
	}

	DrawString(100, 300, "InGame", GetColor(255, 255, 255));
}

// 終了処理
void InGame::Finalize()
{
}