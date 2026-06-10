#pragma once


/// <summary>
/// フェードイン・フェードアウトを管理するクラス
/// SceneManager から使うことで、全シーン切り替えに自動でフェードをかけられる
/// </summary>
class Fade
{
private:
    int alpha;       // 黒い幕の透明度 
    bool isFade;     // フェード中かどうか
    bool isFadeOut;  // true=フェードアウト中 / false=フェードイン中
    bool isEnd;      // フェードアウトが完了したかどうか

public:
    /// <summary>
    /// コンストラクタ：全変数を初期化する
    /// </summary>
    Fade();

    /// <summary>
    /// フェードインを開始する（黒→透明）
    /// SceneManager で新しいシーンに切り替えた直後に呼ぶ
    /// </summary>
    void StartFadeIn();

    /// <summary>
    /// フェードアウトを開始する（透明→黒）
    /// SceneManager でシーン切り替えを検知したときに呼ぶ
    /// </summary>
    void StartFadeOut();

    /// <summary>
    /// Start() は StartFadeIn() の別名
    /// 以前のコードとの互換性のために残してある
    /// </summary>
    void Start();

    /// <summary>
    /// 毎フレーム呼ぶ更新処理：alpha 値を変化させる
    /// </summary>
    void Update();

    /// <summary>
    /// 毎フレーム呼ぶ描画処理：黒い幕を画面全体に描く
    /// </summary>
    void Draw() const;

    /// <summary>
    /// フェード中かどうかを返す
    /// true の間はシーン切り替えを待機させる
    /// </summary>
    bool IsFade() const;

    /// <summary>
    /// フェードアウトが完了したかどうかを返す
    /// true になったらシーンを切り替えてよいサイン
    /// </summary>
    bool IsFadeOutEnd() const;

    /// <summary>
    /// フェードを強制終了する
    /// フェードアウト中なら即座に、alpha=255にして完了扱いにする
    /// フェードイン中なら即座に、alpha=0にして透明に戻す
    /// </summary>
    void ForceEnd();
};