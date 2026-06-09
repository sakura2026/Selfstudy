#include "DxLib.h"
#include "Input/InputManager.h"
#include "System/System.h"
#include "Utility/FPS.h"

// メイン関数
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	if (WakeUp() != TRUE)
	{
		// OutputDebugString→Log.txt出力する関数
		OutputDebugString("アプリケーションの初期化に失敗しました\n");
		return -1;
	}

	// アイコンの設定
	// SetWindowIconID(123);
	// ウィンドウタイトルを設定
	// SetMainWindowText("");
	// マウスカーソル非表示
	// SetMouseDispFlag(FALSE);

	// fps管理系
	const int Frame_Time = 1000 / 60;
	int start_time, delta_time;
	int currentFPS = 0;

	//while (!IsSceneManagerInitialized())
	//{
	//	ClearDrawScreen();
	//	// Lodingの描画を入れるならここに書く

	//	// ここで1つずつ初期化を進める（リソース系含む）
	//	SceneManagerInitialize();
	//	ScreenFlip();
	//}

	int fpstimer = GetNowCount();
	int frameCount = 0;

	// メインループ
	// ウィンドウが閉じられたorシーンマネージャー側で終了状態でループが終わる
	while (ProcessMessage() != -1 /*&& IsFinish() != TRUE*/)
	{
		// フレームクリア
		ClearDrawScreen();

		// フレーム開始時刻を記録
		start_time = GetNowCount();

		// 入力の更新
		InputManager::GetInstance().Update();

		// 画面の初期化
		// SceneManagerUpdate();

		// FPS
		UpdateFPS(frameCount, fpstimer, currentFPS);
		DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS: %d", currentFPS);

		// 裏画面の内容を表画面に反映
		ScreenFlip();

		// エスケープキーでループ終了
		if (InputManager::GetInstance().GetKeyState(KEY_INPUT_ESCAPE) == eInputState::eReleased)
		{
			break;
		}

		// フレームにかかった処理時間
		delta_time = GetNowCount() - start_time;

		if (delta_time < Frame_Time)
		{
			// 残り時間分だけ待機
			WaitTimer(Frame_Time - delta_time);
		}
	}
	// DxLib終了のお知らせ
	ShutDown();
	return 0;
}