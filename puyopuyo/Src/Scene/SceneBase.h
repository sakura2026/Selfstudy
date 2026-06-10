#pragma once
#include "../Utility/Vector2D.h"
#include "../Actor/GameObject.h"
#include "SceneType.h"
#include <vector>
#include <string>

/// <summary>
/// シーン基底クラス
/// 全シーン(タイトル・インゲーム・エンド)の親クラス
/// </summary>
class SceneBase
{
protected:
    // ---- オブジェクト管理用リスト ----
    std::vector<GameObject*> create_list;   // 生成待ちのオブジェクトリスト
    std::vector<GameObject*> object_list;   // 更新・描画を行うオブジェクトリスト
    std::vector<GameObject*> destroy_list;  // 破棄待ちのオブジェクトリスト
    Vector2D                 screen_offset; // スクリーンオフセット

public:
	// 現在のシーンの種類
	eSceneType scene_type;

	/// <summary>
	/// コンストラクタ：メンバ変数の初期化
	/// </summary>
	SceneBase():
        create_list(),
        object_list(),
        destroy_list(),
        screen_offset(0.0f),
        scene_type(eSceneType::eTitle) // 初期シーンはタイトルに設定
	{
	}

	/// <summary>
	/// デストラクタ：virtualをつけて派生クラスのデストラクタも正しく呼ぶ
	/// </summary>
	virtual ~SceneBase()
	{
		// メモリの解放忘れを防ぐために終了処理を呼ぶ
        Finalize();
	}
public:
    /// <summary>
    /// 初期化処理（派生クラスで必ず実装する）
    /// </summary>
    virtual void Init() = 0;

    /// <summary>
    /// 毎フレームの更新処理（派生クラスで必ず実装する）
    /// 派生クラスの Update() の中で UpdateObjectList() を呼ぶこと
    /// </summary>
    /// <returns>現在のシーン種類</returns>
    virtual eSceneType Update() = 0;

    /// <summary>
    /// 毎フレームの描画処理（派生クラスで必ず実装する）
    /// 派生クラスの Draw() の中で DrawObjectList() を呼ぶこと
    /// </summary>
    //virtual void Draw() = 0;

protected:
    /// <summary>
    /// オブジェクトリストの更新処理
    /// 派生クラスの Update() の中で必ず呼ぶこと
    /// </summary>
    /// <param name="delta_second">1フレームあたりの経過時間（秒）</param>
    void UpdateObjectList(const float& delta_second)
    {
        // ---- 生成リストのオブジェクトをオブジェクトリストに追加 ----
        if (!create_list.empty())
        {
            for (GameObject* obj : create_list)
            {
                // Zレイヤー（描画順）を取得する
                int z_layer = obj->GetZLayer();

                // オブジェクトリストの先頭から順番に見ていく
                std::vector<GameObject*>::iterator itr = object_list.begin();
                for (; itr != object_list.end(); itr++)
                {
                    // Zレイヤーが大きい要素の手前に挿入する（描画順を保つ）
                    if (z_layer < (*itr)->GetZLayer())
                    {
                        break;
                    }
                }
                // 適切な位置にオブジェクトを挿入する
                object_list.insert(itr, obj);
            }
            // 生成リストを空にする
            create_list.clear();
        }

        // ---- オブジェクトリスト内の全オブジェクトを更新 ----
        for (GameObject* obj : object_list)
        {
            obj->Update(delta_second);
        }

        // ---- 破棄リストのオブジェクトをオブジェクトリストから削除 ----
        if (!destroy_list.empty())
        {
            for (GameObject* obj : destroy_list)
            {
                // オブジェクトリストの先頭から順番に見ていく
                std::vector<GameObject*>::iterator itr = object_list.begin();
                for (; itr != object_list.end(); itr++)
                {
                    // 破棄対象のオブジェクトを見つけたら削除する
                    if ((*itr) == obj)
                    {
                        object_list.erase(itr); // リストから取り除く
                        obj->Finalize();        // オブジェクトの終了処理
                        delete obj;             // メモリを解放する
                        break;
                    }
                }
            }
            // 破棄リストを空にする
            destroy_list.clear();
        }
    }

public:
    /// <summary>
    /// ロールバック処理
    /// </summary>
    /// <param name="RollValue">float型のロールバックする値</param>
    void RollScreen(float RollValue)
    {
        for (GameObject* obj : object_list)
        {
            // 現在のX座標からスクロール量を引いて位置を更新する
            obj->SetLocation(Vector2D(obj->GetLocation().x - RollValue, obj->GetLocation().y));
        }
    }

    /// <summary>
    /// プレイヤーより後ろのオブジェクトを削除する処理
    /// </summary>
    /// <param name="PlayerLoc">プレイヤーのX座標</param>
    /// <returns>オブジェクトを削除した場合はtrue</returns>
    bool DeleteCheck(float PlayerLoc)
    {
        for (GameObject* obj : object_list)
        {
            // オブジェクトがプレイヤーより左にある場合は削除する
            if (obj->GetLocation().x - PlayerLoc < 0)
            {
                DestroyObject(obj);
                return true;
            }
            return false;
        }
        return false;
    }

    /// <summary>
    /// オブジェクトリストの描画処理
    /// 派生クラスの Draw() の中で必ず呼ぶこと
    /// </summary>
    virtual void Draw() const
    {
        for (GameObject* obj : object_list)
        {
            obj->Draw(screen_offset);
        }
    }

    /// <summary>
    /// 終了時処理：全オブジェクトを破棄してリストを空にする
    /// </summary>
    virtual void Finalize()
    {
        // オブジェクトリスト内の全オブジェクトを破棄する
        DestroyAllObject();

        // 生成リストが空でなければ解放する
        if (!create_list.empty())
        {
            create_list.clear();
        }

        // 破棄リストが空でなければ解放する
        if (!destroy_list.empty())
        {
            destroy_list.clear();
        }
    }

    /// <summary>
    /// 当たり判定処理（必要な派生クラスで上書きして使う）
    /// </summary>
    /// <param name="target">1つ目のゲームオブジェクト</param>
    /// <param name="partner">2つ目のゲームオブジェクト</param>
    virtual void CheckCollision(GameObject* target, GameObject* partner)
    {
        // 派生クラスで内容を記述する
    }

    /// <summary>
    /// ゲームオブジェクト生成処理（テンプレート関数）
    /// 使い方例：CreateObject＜Enemy＞(Vector2D(100.0f, 200.0f));
    /// </summary>
    /// <typeparam name="OBJECT">生成するクラスの型</typeparam>
    /// <param name="generate_location">生成する座標</param>
    /// <returns>生成したオブジェクトのポインタ</returns>
    template<class OBJECT>
    OBJECT* CreateObject(const Vector2D& generate_location)
    {
        // 指定された型のオブジェクトをメモリに確保する
        OBJECT* new_instance = new OBJECT();

        // GameObjectを継承しているかチェックする
        GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

        // キャストに失敗した場合（GameObjectを継承していない型）
        if (new_object == nullptr)
        {
            // 確保したメモリを解放する
            delete new_instance;
            // エラーメッセージを投げる
            throw std::string("ゲームオブジェクトが生成できませんでした");
            return nullptr;
        }

        // オブジェクトの初期化処理を呼ぶ
        new_object->Initialize();

        // 生成位置を設定する
        new_object->SetLocation(generate_location);

        // 生成リストの末尾に追加する（次のUpdateで正式にリストへ移動する）
        create_list.push_back(new_object);

        // 生成したオブジェクトのポインタを返す
        return new_instance;
    }

    /// <summary>
    /// オブジェクト破棄予約処理：破棄リストに追加する
    /// </summary>
    /// <param name="target">破棄したいオブジェクトのポインタ</param>
    void DestroyObject(GameObject* target)
    {
        // ヌルポインタチェック
        if (target == nullptr)
        {
            return;
        }

        // すでに破棄リストに入っていれば追加しない（二重削除防止）
        for (GameObject* obj : destroy_list)
        {
            if (obj == target)
            {
                return;
            }
        }

        // 破棄リストに追加する（次のUpdateで削除される）
        destroy_list.push_back(target);
    }

    /// <summary>
    /// シーン終了フラグ取得処理（必要な派生クラスで上書きして使う）
    /// </summary>
    /// <returns>シーンを終了するかどうか</returns>
    virtual bool GetEndFlag()
    {
        return false;
    }

    /// <summary>
    /// スクリーンオフセット場取得処理
    /// </summary>
    /// <returns>スクリーンオフセット値</returns>
    const Vector2D GetScreenOffset() const
    {
        return screen_offset;
    }

private:
    /// <summary>
    /// オブジェクトリスト内の全オブジェクトを破棄する処理（外から呼ばせない）
    /// </summary>
    void DestroyAllObject()
    {
        // オブジェクトリストが空なら何もしない
        if (object_list.empty())
        {
            return;
        }

        // 全オブジェクトの終了処理とメモリ解放を行う
        for (GameObject* obj : object_list)
        {
            obj->Finalize(); // オブジェクトの終了処理
            delete obj;      // メモリを解放する
        }

        // リストを空にする
        object_list.clear();
    }
};