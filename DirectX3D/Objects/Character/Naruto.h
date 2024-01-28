#pragma once



class Naruto : public ModelAnimator
{
private:
	enum State
	{
		IDLE, 		
		RUN_F,
		RUN_B,
		RUN_L,
		RUN_R,
		THROW,
		PUNCH,
	};



public:
	Naruto();
	~Naruto();


	void Update();
	void Render();
	void GUIRender();
	void PostRender();


private:
	void Control();
	void Move();
	void Rotate();
	void Attack();
	void SetAnimation();
	
	void SetState(State state);
	void Throw();
	void EndThrow();


private:
	Transform* mainHand = nullptr;
	SphereCollider* tmpCollider = nullptr;

	vector<Model*> kunais;


	State curState = IDLE;
	float moveSpeed = 500;
	float rotSpeed = 1.0f;
	float deceleration = 5;

	Vector3 velocity = Vector3::Zero();
	Vector3 targetPos = Vector3::Zero();
	Vector3 prevMousePos = Vector3();

	bool isTarget = true;

	POINT clientCenterPos = { WIN_WIDTH / 2, WIN_HEIGHT >> 1 };

	Quad* crosshair = nullptr;

	int kunaiIndex = 0;
};

