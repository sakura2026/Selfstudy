#pragma once

#include <vector>
#include "GameObject.h"
#include "../Utility/Singleton.h"

class ObjectManager : public Singleton<ObjectManager>
{
	friend class Singleton<ObjectManager>;

private:
	std::vector<GameObject*> create_object;
	std::vector<GameObject*> destroy_object;
	std::vector<GameObject*> game_object_list;

public:
	//生成したオブジェクトをリストに入れる
	void CheckCreateObject()
	{
		if (!create_object.empty())
		{
			for (GameObject* obj : create_object)
			{
				//オブジェクトのZレイヤー情報を入れる
				int z_layer = obj->GetZLayer();
				std::vector<GameObject*>::iterator itr = game_object_list.begin();

				//オブジェクトのZレイヤーの値を比較し、挿入する場所を決める。
				for (; itr != game_object_list.end(); itr++)
				{
					if (z_layer < (*itr)->GetZLayer())
					{
						break;
					}
				}
				//オブジェクトリストに挿入する
				game_object_list.insert(itr, obj);
			}
			//オブジェクト生成リストを解放する
			create_object.clear();

		}
	}

	//削除リストにオブジェクトがあるか確認し、削除する
	void CheckDstroyObject()
	{
		// 破棄リスト内が空でない場合、リスト内のオブジェクトを破棄する
		while (!destroy_object.empty())
		{
			std::vector<GameObject*>::iterator itr = game_object_list.begin();	// オブジェクトリストの先頭
			// リストの末尾になるまで走査する
			for (; itr != game_object_list.end(); itr++)
			{
				// リスト内にあれば、削除する
				if ((*itr) == destroy_object.back())
				{
					game_object_list.erase(itr);
					destroy_object.back()->Finalize();
					delete destroy_object.back();
					break;
				}
			}

			destroy_object.pop_back();		//一番後ろの要素を抜き出す
		}
	}
	const std::vector<GameObject*>& GetObjectsList() const
	{
		return game_object_list;
	}

	//オブジェクト生成テンプレート
	template <class OBJECT>
	OBJECT* CreateGameObject(const Vector2D& generate_location)
	{
		OBJECT* new_instance = new OBJECT();

		GameObject* new_object = dynamic_cast<GameObject*>(new_instance);

		if (new_object == nullptr)
		{
			delete new_instance;

			throw ("ゲームオブジェクトが生成できませんでした\n");
			return nullptr;
		}

		new_object->SetLocation(generate_location);

		new_object->Initialize();

		create_object.push_back(new_object);

		CheckCreateObject();

		return new_instance;
	}

	//削除するオブジェクトをリストに追加する
	void DestroyGameObject(GameObject* target)
	{
		if (target == nullptr)
		{
			return;
		}

		for (GameObject* obj : destroy_object)
		{
			if (obj == target)
			{
				return;
			}
		}

		destroy_object.push_back(target);
	}

	//全てのオブジェクトを削除する
	void AllDestroyGameObject()
	{
		for (GameObject* obj : game_object_list)
		{
			obj->Finalize();
			delete obj;
		}

		//メモリの解放
		game_object_list.clear();
	}
};