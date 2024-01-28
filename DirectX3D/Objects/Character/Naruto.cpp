#include "Framework.h"

Naruto::Naruto() : ModelAnimator("naruto")
{	

	mainHand = new Transform();
	tmpCollider = new SphereCollider();
	tmpCollider->Scale() *= 10.1f;
	tmpCollider->SetParent(mainHand);

	crosshair = new Quad(L"Textures/UI/cursor.png");
	crosshair->Pos() = { CENTER_X,CENTER_Y, 0 };
	crosshair->Scale() *= 0.3f;
	crosshair->UpdateWorld();

	ReadClip("IDLE");
	ReadClip("Run");
	ReadClip("RunBack");
	ReadClip("RunLeft");
	ReadClip("RunRight");
	ReadClip("THROW");
	ReadClip("Punching");

	FOR(10)
	{
		Model* kunai = new ModelInstancing("Kunai");
		kunai->SetParent(mainHand);
		kunais.push_back(kunai);
	}
	

	GetClip(THROW)->SetEvent(bind(&Naruto::Throw, this), 0.7f);
	GetClip(THROW)->SetEvent(bind(&Naruto::EndThrow, this), 0.9f);

	GetClip(PUNCH)->SetEvent(bind(&Naruto::EndThrow, this), 0.9f);
}

Naruto::~Naruto()
{
	delete crosshair;
	delete mainHand;
	delete tmpCollider;
}

void Naruto::Update()
{
	Control();
	SetAnimation();
	mainHand->SetWorld(GetTransformByNode(38));

	ModelAnimator::Update();
	tmpCollider->UpdateWorld();


	for (auto& k : kunais)
	{
		if (k->Active())
			k->Pos() += Forward() * moveSpeed * DELTA;		
		k->UpdateWorld();
	}

	// TODO :  쿠나이가 공격 중에는 앞으로 나아가게_ 아닐 때는 사라지게

	
}

void Naruto::Render()
{
	ModelAnimator::Render();
	tmpCollider->Render();
	for (auto& k : kunais)
	{
		if(k->Active())
			k->Render();
	}

}

void Naruto::GUIRender()
{
	ModelAnimator::GUIRender();

	string temp = "CAM";
	ImGui::DragFloat3(temp.c_str(), (float*)&CAM->Rot(), 0.1f);

}

void Naruto::PostRender()
{
	crosshair->Render();
}

void Naruto::Control()
{
	Rotate();
	Move();
	Attack();
}

void Naruto::Move()
{
	if (curState == THROW || curState == PUNCH) return; // 던지기 중이면 조작 받지 않음

	bool isMoveZ = false; // 전후 이동 중 아님
	bool isMoveX = false; // 좌우 이동 중 아님

	if (KEY_PRESS('W'))
	{
		velocity.z += DELTA; // 속도(범용변수)에 델타만큼 전후값 주기
		isMoveZ = true; //전후 이동 수행 중
	}

	if (KEY_PRESS('S'))
	{
		velocity.z -= DELTA;
		isMoveZ = true; //전후 이동 수행 중
	}

	if (KEY_PRESS('A'))
	{
		velocity.x -= DELTA;
		isMoveX = true; //좌우 이동 수행 중
	}

	if (KEY_PRESS('D'))
	{
		velocity.x += DELTA;
		isMoveX = true; //좌우 이동 수행 중
	}

	if (velocity.Length() > 1) //속도의 전체 가치가 1을 넘으면 (선으로 표현한 벡터의 길이가 1 초과)
		velocity.Normalize(); //정규화
	// 생각할 거리 : 이 코드가 만약 없으면....?

	if (!isMoveZ) // 전후 이동 중이 아니면
		velocity.z = Lerp(velocity.z, 0, deceleration * DELTA); //보간에 의해 감속

	if (!isMoveX) // 좌우이동에 적용
		velocity.x = Lerp(velocity.x, 0, deceleration * DELTA);

	//좌우회전과 행렬계산에 의한 면법선 내기 ( = 정면 구하기)
	Matrix rotY = XMMatrixRotationY(Rot().y);
	Vector3 direction = XMVector3TransformCoord(velocity, rotY); // 현재의 공간이 가지는 "정면"의 실제 벡터 방향

	Pos() += direction * -1 * moveSpeed * DELTA; //"정면" 방향대로 이동


}

void Naruto::Rotate()
{

	Vector3 forword = CAM->Back();

	if(curState != IDLE)
		Rot().y = atan2(forword.x, forword.z) ;
	
	//CAM->Rot().x -= delta.y * rotSpeed * DELTA;

}

void Naruto::Attack()
{
	if (curState == THROW)
		return;

	if (KEY_DOWN(VK_LBUTTON))
	{
		SetState(THROW);
	}
	if (KEY_DOWN(VK_RBUTTON))
	{
		SetState(PUNCH);
	}

	if (curState == THROW || curState == PUNCH)
	{
		vector<Robot*> robots = RobotManager::Get()->GetRobots();
		for (auto& r : robots)
		{
			if (tmpCollider->IsCollision(r->GetCollider()))
			{
				r->Hit();
			}
		}
	}
}

void Naruto::SetAnimation()
{
	if (curState == THROW || curState == PUNCH)
		return;

	if (velocity.z > 0.1f) // 전진 속도가 조금이라도 있으면
		SetState(RUN_F);
	else if (velocity.z < -0.1f) // 전후진 상태는 좌우이동 상태보다 우선한다는 코드
		SetState(RUN_B);
	else if (velocity.x < -0.1f) // 좌우 중에선 왼쪽 우선
		SetState(RUN_L);
	else if (velocity.x > 0.1f) // 마지막 오른쪽
		SetState(RUN_R);
	else // 4축 방향 모두 움직임이 (거의) 없음
		SetState(IDLE);

}

void Naruto::SetState(State state)
{
	if (curState == state)
		return;

	curState = state;
	PlayClip(state);

}

void Naruto::Throw()
{
	KunaiManager::Get()->Throw(tmpCollider->GlobalPos(), Back());
}

void Naruto::EndThrow()
{
	SetState(IDLE);
}
