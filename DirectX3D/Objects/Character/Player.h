#pragma once
class Player : public ModelAnimator
{
private:
	enum State
	{
		L_001, L_002, L_003, L_004,	L_005,
		L_006, L_007, L_008, L_009,	L_010,
		L_011, L_012, L_013, L_014,	L_015,
		L_071, L_072, L_073, L_077, L_078,
		L_079, L_101, L_102, L_103, L_104,
		L_105, L_106, L_107, L_108, L_109, 
		L_110, L_111, L_112, L_113,	L_114, 
		L_115, L_116, L_117, L_118,	L_119, 
		L_120, L_121, L_122, L_123,
	};



public:
	Player();
	~Player();


	void Update();
	void Render();
	void GUIRender();
	void PostRender();


private:
	void Control();
	void Move();
	void ResetPlayTime();

	void Rotate();
	void Attack();
	void SetAnimation();

	void SetState(State state);
	void Throw();
	void EndThrow();

	void SetIdle() { SetState((State)1); }

	void MotionRotate(float degree);

private:
	void ReadClips();
	void RecordLastPos();
	void ReturnIdle()
	{
		Pos() = realPos->Pos();
		GetClip(L_001)->ResetPlayTime();
		SetState(L_001);
	}		

	
	void L001(); 


	void L002();
	void L003();
	void L004();
	void L005();
	void L006();
	void L007();
	void L008();
	void L009();
	void L010();

	void L101();
	void L102();
	void L103();
	void L104();
	void L105();

	void L106();
	void L107();
	void L108();
	void L109();
	void L110();

private:
	Transform* mainHand = nullptr;
	Transform* root = nullptr;
	Transform* realPos = nullptr;
	Transform* lastPos = nullptr;
	Transform* head = nullptr;

	SphereCollider* tmpCollider = nullptr;

	Model* longSword = nullptr;


	State curState = L_101;
	State preState = L_101;

	float moveSpeed = 500;
	float rotSpeed = 1.0f;
	float deceleration = 5;

	const float motionSpeed = 1.5f;

	Vector3 velocity = Vector3::Zero();
	Vector3 targetPos = Vector3::Zero();
	Vector3 prevMousePos = Vector3();

	bool isTarget = true;

	POINT clientCenterPos = { WIN_WIDTH / 2, WIN_HEIGHT >> 1 };

	Quad* crosshair = nullptr;

	int kunaiIndex = 0;


	int node = 84;

	float camRot;
	float rad;

	int loopApply = 334;

};

