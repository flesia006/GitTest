#pragma once
class Robot 
{
private:

	enum State
	{
		STAND_UP, 
		WALK,
		RUN,
		HIT,
		DYING
	};

public:
//	Robot(); // 기본 형태
	Robot(Transform* transform, ModelAnimatorInstancing* instancing, UINT index);
	~Robot();

	void Update();
	void Render();
	void PostRender();

	// 액션 
	void Hit();
	void Spawn(Vector3 pos);
	void SetTarget(Transform* target);

	Transform* GetTransform() { return transform; }
	CapsuleCollider* GetCollider() { return collider; }

private:
	void SetEvent(int clip, Event event, float timeRatio);
	void ExecuteEvent();

	void EndStandUp();
	void EndHit();
	void EndDying();

	void SetState(State state);

	void Move();
	void UpdateUI(); //


private:
	Transform* transform = nullptr;
	CapsuleCollider* collider = nullptr;

	// 주말에
	Transform* root = nullptr;
	Transform* target = nullptr;

	//
	ModelAnimatorInstancing* instancing = nullptr;
	ModelAnimatorInstancing::Motion* motion;

	// 개별 모션 저장용
	float speed = 50;
	float maxHp = 100;
	float curHP = 100;

	Vector3 velocity = { 0, 0, 0 };
	// 업데이트당 벡터 움직임

	UINT index = 0;
	State curState = STAND_UP;

	// 스테이트 혹은 움직임에 따른 이벤트 지정
	vector<map<float, Event>> totalEvent;
	vector<map<float, Event>::iterator> eventIters;


};

