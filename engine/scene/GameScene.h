#pragma once
#include "Sprite.h"
#include "Object3d.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include "Object3d.h"
#include "Input.h"
#include "time.h"
#include <random>
#include "Model.h"
#include "ViewProjection.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "FbxObject3d.h"
#include "FbxModel.h"
#include "Player.h"
#include "RailCamera.h"
#include "Enemy.h"
#include "CollisionManager.h"
#include "Boss.h"
#include "JsonLoader.h"	
#include "energy.h"


class GameScene {
public:
	//�����o�֐�
	GameScene();

	~GameScene();

	void Initialize(SpriteCommon& spriteCommon);
	void LoadObjFromLevelEditor(const std::string& fileName);

	void Update();

	void Draw();
	void Reset();

	Vector3 GetScreenToWorldPos(Sprite& sprite_, RailCamera* rail);
	Vector3 GetWorldToScreenPos(Vector3 pos_, RailCamera* rail);
	Vector2 GetWorldToScreenScale(Object3d* obj, RailCamera* rail);

private:
	//base
	DirectXCommon* dxCommon = nullptr;
	WinApp* winApp = nullptr;
	//���͂̃|�C���^
	Input* input = nullptr;
	//�X�v���C�g�̃|�C���^
	Sprite* sprite = new Sprite;
	SpriteCommon spriteCommon_;
	//�J����
	ViewProjection* viewProjection = nullptr;
	XMViewProjection* xmViewProjection = nullptr;
	//�p�[�e�B�N��
	Particle* particle = nullptr;
	ParticleManager* pm = nullptr;
	Particle* particle_ = nullptr;
	ParticleManager* pm_ = nullptr;
	//�����蔻��
	CollisionManager* collisionManager = nullptr;
	//FBX
	FbxModel* model = nullptr;
	FbxObject3d* obj = nullptr;
	//leveleditor
	std::map<std::string, Model*> models;
	std::vector<Object3d*> objects;
};