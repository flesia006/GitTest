#include "Framework.h"
#include "Robot.h"


Robot::Robot(Transform* transform, ModelAnimatorInstancing* instancing, UINT index)
	:	transform(transform),
		instancing(instancing), 
		index(index)
{

	// 루트
	root = new Transform();

	// 충돌체
	collider = new CapsuleCollider(30, 100);
	collider->SetParent(root);
	collider->Rot().z = XM_PIDIV2 - 0.2f;
	collider->Pos() = { -15, 10, 0 };
	collider->SetActive(true);

	// 모델을 밖에서 만들지, 여기서 만들지
	motion = instancing->GetMotion(index);
	totalEvent.resize(instancing->GetClipSize());
	eventIters.resize(instancing->GetClipSize());


	SetEvent(STAND_UP, bind(&Robot::EndStandUp, this), 0.7f);
	SetEvent(HIT, bind(&Robot::EndHit, this), 0.9f);
	SetEvent(DYING, bind(&Robot::EndDying, this), 0.9f);


	FOR(totalEvent.size())
	{
		eventIters[i] = totalEvent[i].begin(); 
	}


}

Robot::~Robot()
{
	delete collider;
	delete root;

}

void Robot::Update()
{
	// 활성화 시에만 업뎃
	if (!transform->Active())
		return;

	velocity = target->GlobalPos() - transform->GlobalPos();

	ExecuteEvent(); // 이벤트가 터져야하면 수행
	Move();

	UpdateUI();

	root->SetWorld(instancing->GetTransformByNode(index, 3));
	collider->UpdateWorld();
}

void Robot::Render()
{
	collider->Render();
}

void Robot::PostRender()
{

}

void Robot::Hit()
{
	// 얻어맞기
	curHP -= 20;
	
	if (curHP <= 0)
	{
		SetState(DYING);
		return;
	}

	curState = HIT;
	instancing->PlayClip(index, HIT);
	eventIters[HIT] = totalEvent[HIT].begin();
}

void Robot::Spawn(Vector3 pos)
{
	transform->SetActive(true);
	collider->SetActive(true);

	SetState(WALK);
	curHP = maxHp;

	transform->Pos() = pos;

}

void Robot::SetTarget(Transform* target)
{
	this->target = target;
}

void Robot::SetEvent(int clip, Event event, float timeRatio)
{
	if (totalEvent[clip].count(timeRatio) > 0) // 선행 예약 이벤트가 있다면 종료
	{
		return;
	}

	totalEvent[clip][timeRatio] = event;
}

void Robot::ExecuteEvent()
{
	int index = curState;
	
	if (totalEvent[index].empty())
		return;


	if (eventIters[index] == totalEvent[index].end())
		return;

	float ratio = motion->runningTime / motion->duration; // 진행된 시간 나누기 전체 진행 시간
	
	if (eventIters[index]->first > ratio)  // ratio : 전체 동작 중, 현재 동작에 대한 시간 비율
		return;

	eventIters[index]->second();
	eventIters[index]++;
}

// 전환 이벤트들
void Robot::EndStandUp()
{
	collider->SetActive(true);
	SetState(WALK);

}

void Robot::EndHit()
{
	SetState(WALK);
	
}

void Robot::EndDying()
{
	transform->SetActive(false);
	collider->SetActive(false);
}

void Robot::SetState(State state)
{
	if (state == curState)
		return;

	curState = state;
	instancing->PlayClip(index, (int)state);
	eventIters[state] = totalEvent[state].begin();
}

void Robot::Move()
{
	if (curState == STAND_UP || curState == HIT || curState == DYING)
		return;

	if (velocity.Length() < 10) // 표적과 너무 가까움
	{
		return;
	}

	if (velocity.Length() < 1000)
	{
		speed = 150;
		SetState(RUN);
	}
	else
	{
		speed = 50;
		SetState(WALK);
	}

	transform->Pos() += velocity.GetNormalized() * speed * DELTA;
	transform->Rot().y = atan2(velocity.x, velocity.z) + XM_PI;
}

void Robot::UpdateUI()
{
}
