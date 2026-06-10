#include "SceneManager.h"
#include "SceneType.h"
#include "SceneBase.h"
#include "DxLib.h"
#include "../Input/InputManager.h"

// 各シーンのインクルードをする
#include "../Scene/Title/Title.h"
#include "../Scene/InGame/InGame.h"
#include "../Scene/End/End.h"

// フェード系のインクルード
#include "../Utility/Fade.h"

// シーン管理変数
SceneBase* current_scene = nullptr;
static eSceneType current_scene_type = eTitle;
int           is_end_flag = 0;
static int    init_flag = false;

// 起動時フェードイン用
static Fade   startup_fade;           // ゲーム開始直後のフェードイン
static bool   startup_fading = true;  // 起動フェードが終わったか

// シーン切り替えトランジション用
//static TileTransitor transitor(50, 60);
//static bool is_transitioning = false;  // トランジション中かどうか

// 前方宣言
void ChangeScene(eSceneType new_scene_type);


/// <summary>
/// SceneManager の初期化処理（最初に一度だけ呼ぶ）
/// </summary>
void SceneManagerInitialize(void)
{
    if (init_flag)
    {
        return;
    }

    is_end_flag = 0;

    // TitleScene を最初のシーンとして生成・初期化する
    current_scene = new Title();
    current_scene->Init();
    current_scene_type = eTitle;

    // ★起動時フェードインを開始する（真っ黒→画面が見えてくる）
    startup_fade.StartFadeIn();
    startup_fading = true;

    //is_transitioning = false;

    init_flag = true;
}

/// <summary>
/// SceneManager が初期化済みかどうかを返す
/// </summary>
bool IsSceneManagerInitialized()
{
    return init_flag;
}

/// <summary>
/// SceneManager の毎フレーム更新処理
/// </summary>
void SceneManagerUpdate(void)
{
    // 入力のやつ
    InputManager& input = InputManager::GetInstance();

    // フェード中にボタンを押したかの判定
    bool forcePushed =
        input.GetKeyState(KEY_INPUT_SPACE) == eInputState::ePressed ||
        input.GetButtonState(XINPUT_BUTTON_A) == eInputState::ePressed;

    if (startup_fading)
    {
        // 通常通りシーンを更新・描画する
        current_scene->Update();  // 戻り値は無視（起動直後はシーン切り替えしない）
        current_scene->Draw();

        // ボタンを押したらフェードを強制終了
        if (forcePushed)
        {
            startup_fade.ForceEnd();
        }

        // フェードイン更新・描画（シーンの上に黒い幕を重ねる）
        startup_fade.Update();
        startup_fade.Draw();

        // フェードが終わったら通常モードへ移行する
        if (!startup_fade.IsFade())
        {
            startup_fading = false;
        }
        return;
    }

    // シーンを更新して、次のシーンタイプを受け取る
    eSceneType next_scene_type = current_scene->Update();

    // シーンを描画する
    current_scene->Draw();

    // シーン切り替えが必要か確認する
    if (next_scene_type != current_scene_type)
    {
        // ゲーム終了の場合
        if (next_scene_type == eEnd2)
        {
            is_end_flag = 1;
            return;
        }
    }
}

/// <summary>
/// ゲームを終了するかどうかのフラグを返す
/// </summary>
int IsFinish(void)
{
    return is_end_flag;
}

// 選択されたステージ番号（シーンをまたいで保持）
static int selected_stage = 1;

int GetSelectedStage()
{
    return selected_stage;
}

void SetSelectedStage(int stageNum)
{
    selected_stage = stageNum;
}

bool IsSceneInputLocked()
{
    return startup_fading;
}

/// <summary>
/// シーンを実際に切り替える内部処理
/// </summary>
void ChangeScene(eSceneType new_scene_type)
{
    // 現在のシーンを削除する（メモリ解放）
    if (current_scene != nullptr)
    {
        current_scene->Finalize();
        delete current_scene;
        current_scene = nullptr;
    }

    // 指定されたシーンタイプのシーンを生成する
    switch (new_scene_type)
    {
    case eTitle:
        current_scene = new Title();
        break;

    //case eInGame:
    //    current_scene = new InGame();
    //    break;

    //case eEnd:
    //    current_scene = new End();
    //    break;

    //case eEnd2:
    default:
        is_end_flag = 1;
        return;
    }

    // 新しいシーンを初期化する
    current_scene->Init();
}