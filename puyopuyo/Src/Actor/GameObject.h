#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include<vector>
#include "DxLib.h"
#include "../Utility/Vector2D.h"
//#include "../Utility/InputManager.h"

// ゲームオブジェクト基底クラス
class GameObject
{
protected:
	Vector2D location;			// 座標
	int image;					// 画像
	int image2;
	std::vector<int> allImage;	// すべての画像
	unsigned char z_layer;		// Zレイヤー

public:
	// コンストラクタ
	GameObject();
	// デストラクタ
	virtual ~GameObject() = default;


	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(float delta_time);
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const Vector2D& screen_offset) const;
	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// Zレイヤー取得
	/// </summary>
	/// <returns>Zレイヤー</returns>
	const unsigned char GetZLayer() const;

	/// <summary>
	/// 座標設定
	/// </summary>
	/// <param name="location">座標</param>
	void SetLocation(const Vector2D location);

	/// <summary>
	/// 座標取得
	/// </summary>
	/// <param name="location">座標</param>
	const Vector2D GetLocation() const;

};