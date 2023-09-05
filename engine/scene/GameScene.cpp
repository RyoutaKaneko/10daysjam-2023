#include "GameScene.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <map>

GameScene::GameScene() {}

GameScene::~GameScene() {
}

///-----�ϐ��̏�����-----///
void GameScene::Initialize(SpriteCommon& spriteCommon) {
	//���
	dxCommon = DirectXCommon::GetInstance();
	winApp = WinApp::GetInstance();
	input = Input::GetInstance();
	//�����蔻��
	collisionManager = CollisionManager::GetInstance();

	//���x���G�f�B�^�ǂݍ���
	LoadObjFromLevelEditor("scene");

	viewProjection = new ViewProjection;

	xmViewProjection = new XMViewProjection;

	//FbxObject�̐ÓI������
	//�J�������Z�b�g
	FbxObject3d::SetCamera(viewProjection);
	//�O���t�B�b�N�X�p�C�v���C����������
	FbxObject3d::CreateGraphicsPipeline();

	// �X�v���C�g�̏�����
	// �X�v���C�g
	sprite = new Sprite();
	spriteCommon_ = sprite->SpriteCommonCreate(dxCommon->GetDevice());

	//�p�[�e�B�N��������
	particle = Particle::LoadParticleTexture("blue.png");
	pm_ = ParticleManager::Create();
	particle_ = Particle::LoadParticleTexture("crosshair.png");
	pm = ParticleManager::Create();
	//�I�u�W�F�N�g�Ƀ��f����R�t����
	pm->SetParticleModel(particle);
	pm_->SetParticleModel(particle_);
	//�J�������Z�b�g
	pm->SetXMViewProjection(xmViewProjection);
	pm_->SetXMViewProjection(xmViewProjection);

	////���f�������w�肵�ēǂݍ���
	//obj = new FbxObject3d;
	//obj->Initialize();
	//model = FbxLoader::GetInstance()->LoadModelFlomFile("cube");
	//obj->SetModel(model);
	//obj->SetPosition(Vector3(1, 0, -10));
	//obj->SetScale(Vector3((float)0.01, (float)0.01, (float)0.01));
}

///-----�X�V����-----///
void GameScene::Update() {
	input->Update();
	
}

void GameScene::Draw() {
#pragma region 3D�I�u�W�F�N�g�`��

	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(dxCommon->GetCommandList());

	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();

#pragma endregion

#pragma region FBX3D�I�u�W�F�N�g�`��

	// 3D�I�u�W�F�N�g�`��O����
	FbxObject3d::PreDraw(dxCommon->GetCommandList());

	// 3D�I�u�W�F�N�g�`��㏈��
	FbxObject3d::PostDraw();

#pragma endregion

#pragma region �p�[�e�B�N���`��

	// �p�[�e�B�N���`��O����
	ParticleManager::PreDraw(dxCommon->GetCommandList());

	///==== �p�[�e�B�N���`�� ====///
	//�p�[�e�B�N��
	/*pm->Draw();
	pm_->Draw();*/

	// �p�[�e�B�N���`��㏈��
	ParticleManager::PostDraw();

#pragma endregion

#pragma region �X�v���C�g�`��

	// �X�v���C�g�`��O����
	Sprite::PreDraw(dxCommon->GetCommandList(), spriteCommon_);

	
	// �X�v���C�g�`��㏈��
	Sprite::PostDraw();
}

void GameScene::Reset() {
	
}

Vector3 GameScene::GetScreenToWorldPos(Sprite& sprite_, RailCamera* rail)
{
	if (rail == nullptr) {
		return Vector3(0, 0, 1);
	}

	//�r���[�|�[�g�s�񐶐�
	Matrix4 viewPort = viewPort.ViewPortMat(WinApp::window_width, WinApp::window_height, Vector2(0.0f, 0.0f));

	//�r���[�v���W�F�N�V�����r���[�|�[�g�����s��
	Matrix4 invViewPort = viewPort;
	invViewPort.MakeInverse();
	//�v���W�F�N�V�����s��//
	float fovAngleY = 45.0f * (3.141592f / 180.0f);;
	float aspectRatio = (float)WinApp::window_width / WinApp::window_height;
	//�v���W�F�N�V�����s�񐶐�
	Matrix4 projection = projection.ProjectionMat(fovAngleY, aspectRatio, 0.1f, 200.0f);
	Matrix4 invProjection = projection;
	invProjection.MakeInverse();
	//�r���[�s��//
	Matrix4 view = viewProjection->GetMatView();
	Matrix4 invView = view;
	invView.MakeInverse();
	////�����s��̋t�s����v�Z����
	Matrix4 matInverseVPV = invViewPort * invProjection * invView;
	//�X�N���[�����W�n���烏�[���h���W�n
	Matrix4 mat1, mat2;
	//w���Z
	Vector3 posNear = Vector3(sprite_.GetPosition().x, sprite_.GetPosition().y, 0);
	Vector3 posFar = Vector3(sprite_.GetPosition().x, sprite_.GetPosition().y, 1);
	posNear = mat1.transform(posNear, matInverseVPV);
	posFar = mat2.transform(posFar, matInverseVPV);

	//�}�E�X���C�̕���
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = mouseDirection.normalize();
	//�J��������Ə��I�u�W�F�N�g�̋���
	const float kDistanceTestObject = 0.05f;

	/*Vector3 pos = player->GetWorldPos();*/
	Vector3 translate = (posFar/* - pos*/) * kDistanceTestObject;

	return translate;
}

Vector3 GameScene::GetWorldToScreenPos(Vector3 pos_, RailCamera* rail)
{
	if (rail == nullptr) {
		return Vector3(0, 0, 0);
	}

	//�r���[�s��//
	Matrix4 view = viewProjection->GetMatView();
	//�v���W�F�N�V�����s��//
	float fovAngleY = 45.0f * (3.141592f / 180.0f);;
	float aspectRatio = (float)WinApp::window_width / WinApp::window_height;
	//�v���W�F�N�V�����s�񐶐�
	Matrix4 projection = projection.ProjectionMat(fovAngleY, aspectRatio, 0.1f, 200.0f);
	//�r���[�|�[�g�s�񐶐�
	Matrix4 viewPort = viewPort.ViewPortMat(WinApp::window_width, WinApp::window_height, Vector2(0.0f, 0.0f));

	Matrix4 matVPV = view * projection * viewPort;

	Matrix4 mat;
	Vector3 posScreen = pos_;
	posScreen = mat.transform(posScreen, matVPV);
	posScreen.z = 0;

	return posScreen;
}

Vector2 GameScene::GetWorldToScreenScale(Object3d* obj, RailCamera* rail)
{
	if (rail == nullptr) {
		return Vector2(0, 0);
	}

	Vector3 v = obj->GetPosition() - rail->GetView()->GetEye();
	v.normalize();
	float len = v.length();

	float x = 64;
	x *= obj->GetScale().x;
	float y = 64;
	y *= obj->GetScale().y;


	return Vector2(x / len, y / len);
}

void GameScene::LoadObjFromLevelEditor(const std::string& fileName) {
	JsonLoader* file = nullptr;
	LevelData* levelData = file->LoadFile(fileName);

	//�I�u�W�F�N�g�z�u
	for (auto& objectData : levelData->objects) {
		//�t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) { model = it->second; }
		//���f�����w�肵��3DObject�𐶐�
		Object3d* newObject = Object3d::Create();
		newObject->Initialize();
		newObject->SetModel(model);
		//���W
		Vector3 pos;
		pos = Vector3(objectData.translation.x, objectData.translation.y, objectData.translation.z);
		newObject->SetPosition(pos);
		//��]�p
		Vector3 rot;
		rot = Vector3(objectData.rotation.x, objectData.rotation.y, objectData.rotation.z);
		newObject->SetRotation(rot);
		//�X�P�[��
		Vector3 scale;
		scale = Vector3(objectData.scaling.x, objectData.scaling.y, objectData.scaling.z);
		newObject->SetScale(scale);
		//�z��ɓo�^
		objects.push_back(newObject);
	}
}