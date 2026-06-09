#pragma once

#include "Template.h"

#include "../Utility/Vector2D.h"

//マクロ定義
#define D_BUTTON_MAX	(16)

enum eInputState
{
	eNone,
	ePressed,
	eReleased,
	eHeld
};

class InputManager : public Singleton<InputManager>
{
private:
	//コントローラー入力情報
	bool now_button[D_BUTTON_MAX] = {};
	bool old_button[D_BUTTON_MAX] = {};
	float trigger[2] = {};		// ZL/ZRトリガーの現在入力値
	float old_trigger[2] = {};	// ZL/ZRトリガーの前回入力値
	Vector2D Left_stick;
	Vector2D Right_stick;
	static constexpr float STICK_MAX = 1.0f;	// スティックの最大入力範囲
	static constexpr float DEAD_ZONE = 0.5f;	// この値以下の数値を全て0にする
	static constexpr float TRIGGER_DEAD_ZONE = 0.5f;	// この値以上でトリガー入力扱いにする

	// キーボードの入力情報
	static InputManager* instance;

	char now_key[256] = { NULL };
	char old_key[256] = { NULL };

public:
	void Update();
	eInputState GetButtonState(int button) const;
	eInputState GetLeftTriggerState() const;	// ZLトリガーの押下状態取得
	eInputState GetRightTriggerState() const;	// ZRトリガーの押下状態取得
	float GetLeftTrigger() const;
	float GetRightTrigger() const;
	const Vector2D& GetLeftStick() const;
	const Vector2D& GetRightStick() const;

	// キーボードの状態取得
	eInputState GetKeyState(int key) const;

private:
	// 入力範囲取得
	bool CheckButtonRange(int button) const;
	bool CheckKeycodeRange(int key) const;
	eInputState GetTriggerState(float now, float old) const;	// トリガーの現在値と前回値から押下状態を判定
	float TriggerNormalize(unsigned char value) const;	// トリガー入力値を0.0f～1.0fに変換
	Vector2D Left_StickNormalize(float x, float y) const;
	Vector2D Right_StickNormalize(float x, float y) const;
	Vector2D StickNormalize(float x, float y) const;

};

