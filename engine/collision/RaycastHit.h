#pragma once
#include "BaseCollider.h"
#include <DirectXMath.h>

class Object3d;

//レイキャストによる情報を得るための構造体
struct RaycastHit
{
	//衝突相手のオブジェクト
	Object3d* object = nullptr;
	//衝突相手のコライダー
	BaseCollider* collider = nullptr;
	//衝突点
	Vector3 inter;
	//衝突点までの距離
	float distance = 0.0f;
};