#include "Title.h"
#include "DxLib.h"
#include "../SceneManager.h"
#include "../../Input/InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Title::Title()
    : Font(0)
    , BigFont(0)
    , SmallFont(0)
    , Alpha(0)
    , FadeIn(true)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Title::~Title()
{
    Title::Finalize();
}

void Title::Init()
{
    //// ResourceManagerの呼び出し
    //ResourceManager& RM = ResourceManager::GetInstance();

    // フォントファイルをプログラムに登録
    // このプログラムだけで使う設定
    Font = AddFontResourceEx("Resource/Fonts/Cyberphont 2.0.ttf", FR_PRIVATE, NULL);
    // 登録に失敗したら警告
    if (Font == 0)
    {
        MessageBox(NULL, "フォントの読み込みに失敗", "ERROR", MB_OK);
    }
    // 大きい文字用
    BigFont = CreateFontToHandle("Cyberphont 2.0", 125, -1, DX_FONTTYPE_NORMAL);
    //　小さな文字用
    SmallFont = CreateFontToHandle("Cyberphont 2.0", 60, -1, DX_FONTTYPE_EDGE, -1, 4);
    //==============================ここまでフォント登録の処理=========================//

    // アルファ値の初期化
    Alpha = 0;
    // 最初はフェードイン(明るくなる)
    FadeIn = true;
}

eSceneType Title::Update()
{
    // 入力情報の取得
    InputManager& input = InputManager::GetInstance();

    // フェードインのとき
    if (FadeIn)
    {
        // アルファ値を増やす
        Alpha += 3;
        if (Alpha >= 255)
        {
            Alpha = 255;
            FadeIn = false;
        }
    }

    // フェードアウトのとき
    else
    {
        // アルファ値を減らす
        Alpha -= 3;

        // 最小値に達したらフェードインに切り替え
        if (Alpha <= 0)
        {
            Alpha = 0;
            FadeIn = true;
        }
    }

    if (!IsSceneInputLocked())
    {
        if (input.GetKeyState(KEY_INPUT_SPACE) == eInputState::ePressed ||
            input.GetButtonState(XINPUT_BUTTON_A) == eInputState::ePressed)
        {
            return eInGame;
        }
    }

    return eTitle;
}

void Title::Draw() const
{
    // アルファブレンドモードにして透明度をAlphaの値にする
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, Alpha);
    // 小さな文字の位置と色
    DrawStringToHandle(20, 10, "press   a  ", GetColor(125, 255, 70), SmallFont, GetColor(0, 0, 0));
    // ブレンドモードを元（通常描画）に戻す ← これを忘れると他の描画もおかしくなる
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Title::TitleCursorUpdate(void)
{
    InputManager& input = InputManager::GetInstance();
}

void Title::Finalize()
{
    // フォントハンドルを削除
    DeleteFontToHandle(BigFont);
    DeleteFontToHandle(SmallFont);

    // 登録したフォントをプログラムから解放
    RemoveFontResourceEx("Resource/Fonts/Cyberphont 2.0.ttf", FR_PRIVATE, NULL);
}