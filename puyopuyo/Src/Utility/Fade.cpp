#include "Fade.h"
#include "../System/ProjectConfig.h"
#include <DxLib.h>

// 画面の横幅・縦幅
static const int SCREEN_W = D_WINDOW_SIZE_Y;
static const int SCREEN_H = D_WINDOW_SIZE_X;

// 1フレームあたり alpha が変化する量（大きいほど速くフェードする）
static const int FADE_SPEED = 5;

/// <summary>
/// コンストラクタ
/// </summary>
Fade::Fade()
{
    alpha = 0;     // 最初は透明（真っ黒にはなっていない）
    isFade = false;
    isFadeOut = false;
    isEnd = false;
}

/// <summary>
/// フェードインを開始する（黒 → 透明）
/// </summary>
void Fade::StartFadeIn()
{
    alpha = 255;   // 真っ黒な状態からスタート
    isFade = true;
    isFadeOut = false;
    isEnd = false;
}

/// <summary>
/// フェードアウトを開始する（透明 → 黒）
/// </summary>
void Fade::StartFadeOut()
{
    alpha = 0;     // 透明な状態からスタート
    isFade = true;
    isFadeOut = true;
    isEnd = false;
}

/// <summary>
/// Start() は StartFadeIn() の別名
/// 以前のコードとの互換性のために残してある
/// </summary>
void Fade::Start()
{
    StartFadeIn();
}

/// <summary>
/// 毎フレームの更新処理
/// </summary>
void Fade::Update()
{
    // フェード中でなければ何もしない
    if (!isFade)
    {
        return;
    }

    if (isFadeOut)
    {
        // フェードアウト：alpha を増やして黒くしていく
        alpha += FADE_SPEED;

        if (alpha >= 255)
        {
            alpha = 255;   // 255 を超えないようにクランプする
            isFade = false;
            isEnd = true;  // フェードアウト完了フラグを立てる
        }
    }
    else
    {
        // フェードイン：alpha を減らして透明にしていく
        alpha -= FADE_SPEED;

        if (alpha <= 0)
        {
            alpha = 0;        // 0 を下回らないようにクランプする
            isFade = false;
        }
    }
}

/// <summary>
/// 毎フレームの描画処理
/// </summary>
void Fade::Draw() const
{
    // アルファブレンドモードに切り替えて黒い幕を描く
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    DrawBox(0, 0, SCREEN_W, SCREEN_H,
        GetColor(0, 0, 0), TRUE);  // 黒い四角を画面全体に塗りつぶす

    // ブレンドモードを元に戻す（他の描画に影響させない）
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// フェード中かどうかを返す
/// </summary>
bool Fade::IsFade() const
{
    return isFade;
}

/// <summary>
/// フェードアウトが完了したかどうかを返す
/// </summary>
bool Fade::IsFadeOutEnd() const
{
    return isEnd;
}

void Fade::ForceEnd()
{
    // フェード中でなければ何もしない
    if (!isFade)
    {
        return;
    }

    if (isFadeOut)
    {
        // フェードアウト中　→　即座に真っ黒にして「完了」にする
        alpha = 255;
        isFade = false;
        isEnd = true;
    }
    else
    {
        // フェードイン中　→　即座に透明にして「完了」にする
        alpha = 0;
        isFade = false;
        isEnd = false;
    }
}