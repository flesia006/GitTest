#include "Framework.h"
#include "RobotManager.h"

RobotManager::RobotManager()
{
    //인스턴싱 (의 공통 모델) 생성
    robotInstancing = new ModelAnimatorInstancing("Robot");

    robotInstancing->ReadClip("StandUp");
    robotInstancing->ReadClip("Walk");
    robotInstancing->ReadClip("Run");
    robotInstancing->ReadClip("Hit");
    robotInstancing->ReadClip("Dying");

    //로봇 인스턴스 만들어서 인스턴싱과 연결
    FOR(SIZE)
    {
        Transform* transform = robotInstancing->Add(); // 인스턴싱에서 트랜스폼을 추가한 후 매니저에서도 할당
        transform->SetActive(false); // 아직 게임 내에서 생성을 거친 게 아니라 활성화는 안 함
        Robot* robot = new Robot(transform, robotInstancing, i); //내부 인덱스 i번째, 모델은 인스턴싱을 통해 공유하는 로봇 생성
        robots.push_back(robot); //벡터에 윗줄에서 만든 로봇 추가
        delete transform;
    }

}

RobotManager::~RobotManager()
{
    delete robotInstancing;

    for (Robot* r : robots)
        delete r;
}

void RobotManager::Update()
{
    Collision();

    //시간 경과를 체크
    time += DELTA;

    if (time >= SPAWN_TIME) //시간이 생성 간격에 도달하면
    {
        time -= SPAWN_TIME; // time이 오차 발생시 오차를 무시하고 재생성 간격을 우선
        // -> 장 : 대체로 시간 간격을 유지 가능 / 단 : 오차가 간헐적일 경우 불규칙한 재생성(일찍 생성)
// time = 0;        // time이 오차 발생시 간격을 무시하고 오차 교정을 우선
                    // -> 장 : 규칙적인 생성 환경을 유지 가능 / 단점 : 시간 간격이 벌어질 위험이 있다
        Spawn(); //재생성
    }

    //모델 자체를 업데이트
    robotInstancing->Update();

    //로봇의 객체들도 업데이트 (객체 : 설계도로서의 클래스 + 결과물로서의 인스턴스의 총칭.)
    for (Robot* r : robots)
        r->Update();
}

void RobotManager::Render()
{
    robotInstancing->Render();

    //로봇의 객체들도 업데이트 (객체 : 설계도로서의 클래스 + 결과물로서의 인스턴스의 총칭.)
    for (Robot* r : robots)
        r->Render();
}

void RobotManager::PostRender()
{
    for (Robot* r : robots)
        r->PostRender();
}

void RobotManager::GUIRender()
{
}

void RobotManager::SetTarget(Transform* target)
{
    for (Robot* r : robots)
        r->SetTarget(target);
}

void RobotManager::Collision()
{

}

void RobotManager::Spawn()
{
    Vector3 dir;
    dir.x = Random(-1.0f, 1.0f);
    dir.y = 0;
    dir.z = Random(-1.0f, 1.0f);

    // 매니저가 추적 중인 표적의 현재 위치에서 어느 정도 떨어진, 방향은 랜덤이 장소를 선택
    // 거리를 조절하거나, 거리까지 랜덤으로 하고 싶다면 아래 코드의 2000.0f 계수를 수정
    Vector3 randomPos = (dir.GetNormalized() * 1000.0f);

    //정해진 랜덤 장소에서 로봇 생성
    for (Robot* r : robots)
    {
        if (!r->GetTransform()->Active()) // 아직 로봇이 안 나와 있다면
        {
            r->Spawn(randomPos);
            break; //생성 종료
        }
    }
}
