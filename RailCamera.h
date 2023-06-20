﻿#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include"Matrix4x4.h"
#include "ImGuiManager.h"
#include<imgui.h>


class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& radian);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }	

private:
	//ワールド変換データ
    WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;
};

