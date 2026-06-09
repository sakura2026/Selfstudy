#include "InputManager.h"
#include <cmath>

void InputManager::Update()
{
	// キーボード入力値の更新
	memcpy(old_key, now_key, sizeof(char) * 256);
	GetHitKeyStateAll(now_key);

	//コントローラー入力値の更新
	XINPUT_STATE input = {};
	GetJoypadXInputState(DX_INPUT_PAD1, &input);
	for (int i = 0; i < D_BUTTON_MAX; i++)
	{
		old_button[i] = now_button[i];
		now_button[i] = static_cast<bool>(input.Buttons[i]);
	}
	// ZL/ZRの押した瞬間を取るため、前回値を保存してから現在値を更新
	old_trigger[0] = trigger[0];
	old_trigger[1] = trigger[1];
	trigger[0] = TriggerNormalize(input.LeftTrigger);
	trigger[1] = TriggerNormalize(input.RightTrigger);
	Left_stick = Left_StickNormalize(input.ThumbLX, input.ThumbLY);		// 左スティックの更新処理
	Right_stick = Right_StickNormalize(input.ThumbRX, input.ThumbRY);	// 右スティックの更新処理
}

// ボタンの現在状態取得
eInputState InputManager::GetButtonState(int button) const
{
	//ボタンが押されているか
	if (CheckButtonRange(button) && (now_button[button] && old_button[button]))
	{
		return eInputState::eHeld;
	}

	//ボタンが押された瞬間か
	if (CheckButtonRange(button) && (now_button[button] && !old_button[button]))
	{
		return eInputState::ePressed;
	}

	//ボタンが離された瞬間か
	if (CheckButtonRange(button) && (!now_button[button] && old_button[button]))
	{
		return eInputState::eReleased;
	}

	return eInputState::eNone;
}

// 左トリガーの現在状態取得
eInputState InputManager::GetLeftTriggerState() const
{
	return GetTriggerState(trigger[0], old_trigger[0]);
}

// 右トリガーの現在状態取得
eInputState InputManager::GetRightTriggerState() const
{
	return GetTriggerState(trigger[1], old_trigger[1]);
}

// デッドゾーン処理
Vector2D InputManager::StickNormalize(float x, float y) const
{
	x /= 32767.0f;
	y /= 32767.0f;

	// 大きさの取得
	float length = sqrtf(x * x + y * y);

	// 取得した大きさがDEAD_ZONEより小さい場合、全て０にする
	if (length < DEAD_ZONE)
	{
		return Vector2D(0.0f, 0.0f);
	}

	// 現在スティックの数値計算
	float normalized = (length - DEAD_ZONE) / (1.0f - DEAD_ZONE);
	if (normalized > 1.0f) normalized = 1.0f;

	// スティックの数値を渡す
	return Vector2D((x / length) * normalized,
		(y / length) * normalized);
}
// ZL/ZRトリガーをボタンと同じ ePressed / eHeld / eReleased で扱う
eInputState InputManager::GetTriggerState(float now, float old) const
{
	const bool isNowPressed = now >= TRIGGER_DEAD_ZONE;
	const bool wasPressed = old >= TRIGGER_DEAD_ZONE;

	if (isNowPressed && wasPressed)
	{
		return eInputState::eHeld;
	}

	if (isNowPressed && !wasPressed)
	{
		return eInputState::ePressed;
	}

	if (!isNowPressed && wasPressed)
	{
		return eInputState::eReleased;
	}

	return eInputState::eNone;
}

float InputManager::TriggerNormalize(unsigned char value) const
{
	return static_cast<float>(value) / 255.0f;
}

// 左トリガー取得
float InputManager::GetLeftTrigger() const
{
	return trigger[0];
}

// 右トリガー取得
float InputManager::GetRightTrigger() const
{
	return trigger[1];
}

// 左スティック情報の取得
const Vector2D& InputManager::GetLeftStick() const
{
	return Left_stick;
}

// 右スティック情報の取得
const Vector2D& InputManager::GetRightStick() const
{
	return Right_stick;
}

// ボタンの範囲走査
bool InputManager::CheckButtonRange(int button) const
{
	return (0 <= button && button < D_BUTTON_MAX);
}

// 左スティックの範囲をVector2Dに変換
Vector2D InputManager::Left_StickNormalize(float x, float y) const
{
	return StickNormalize(x, y);
}

// 右スティックの範囲をVector2Dに変換
Vector2D InputManager::Right_StickNormalize(float x, float y) const
{
	return StickNormalize(x, y);
}

eInputState InputManager::GetKeyState(int key) const
{
	if (CheckKeycodeRange(key))
	{
		if (old_key[key] == TRUE)
		{
			if (now_key[key] == TRUE)
			{
				return eInputState::eHeld;
			}
			else
			{
				return eInputState::eReleased;
			}
		}
		else
		{
			if (now_key[key] == TRUE)
			{
				return eInputState::ePressed;
			}
		}
	}
	return eInputState::eNone;
}

bool InputManager::CheckKeycodeRange(int key) const
{
	return (0 <= key && key < 256);
}