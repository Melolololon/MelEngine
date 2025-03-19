#include "Player.h"
#include"TestObject.h"
#include<Input.h>
#include<LibMath.h>


void Player::Move()
{
	// �ړ��x�N�g��
	MelLib::Vector3 moveVector;
	// �ړ����x
	static const float MOVE_SPEED = 0.2f;
	
	if (MelLib::Input::KeyState(DIK_LEFT))
	{
		moveVector.x -= MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_RIGHT))
	{
		moveVector.x += MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_UP))
	{
		moveVector.z += MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_DOWN))
	{
		moveVector.z -= MOVE_SPEED;
	}

	if (MelLib::Input::KeyTrigger(DIK_SPACE)) {
		moveVector.x += MOVE_SPEED;
	}
	float angle = MelLib::Input::LeftStickAngle(30);
	if (angle != -1.0f) 
	{
		MelLib::Vector2 v = MelLib::LibMath::AngleToVector2(angle, true);
		moveVector.x = v.x * MOVE_SPEED;
		moveVector.z = v.y * MOVE_SPEED;
	}

	// ���Z
	// AddPosition�ASetPosition�͓����蔻����ꏏ�ɓ���
	AddPosition(moveVector);
}

void Player::LoadRes()
{
	bool res = MelLib::ModelData::Load("Resources/Player/Player.fbx", false,"test");

}

// ���O���w�肷��Ƃ���objectName��n���Ȃ��Ɣԍ������f����Ȃ�
// ���Ɩ��O�ύX�������GUI�̕ϐ����������Ȃ��Ƃ����Ȃ�
// �ǉ��֐�����Ȃ���GameObject�̃R���X�g���N�^�Ŕԍ��t���Ȃ��Ƃ����Ȃ�
// ObjectManager�ɑ��݂��邩�m�F���Ĕԍ��t����ƒǉ�������Enemy5�������S�������ɂȂ����Ⴄ
// GameObject��static��umap�������Ĕԍ��Ǘ������ق����悳����
Player::Player()
	:GameObject("Player")
{
	// �ǂݍ��񂾃p�����[�^�[�������I�Ƀ��f���ɂ��K��������
	// modelObject�ⓖ���蔻���GUI�ł������悤�ɂ���

	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX),GetObjectName());

	// ���ǂݍ��񂾒l�����f���I�u�W�F�N�g�ɔ��f�ł��ĂȂ�����
	// �����ʒu��0,0,5��
	//SetPosition(GetPosition());

	modelObjects["main"].SetPosition(GetPosition());

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition({2.5f,0,0});
	sphereDatas["main"][0].SetRadius(1.0f);
	//modelObjects["main"].SetPosition(GetPosition());
	modelObjects["main"].SetScale(1);

	sphereFrameHitCheckNum = 1;


	

	hp.SetData(10, GetObjectName(), "HP", 0, 100);
	power.SetData(10, GetObjectName(), "Power", 1, 100);
}

void Player::Update()
{
	Move();


}

void Player::Draw()
{
	// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
	AllDraw();
}

void Player::Hit
(
	const GameObject& object, 
	const MelLib::ShapeType3D shapeType, 
	const std::string& shapeName, 
	const MelLib::ShapeType3D hitObjShapeType,
	const std::string& hitShapeName
)
{

	int z = 0;

}

std::shared_ptr<MelLib::GameObject> Player::GetNewPtr()
{
	return std::make_shared<Player>();
}
