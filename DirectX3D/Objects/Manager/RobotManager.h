#pragma once

class RobotManager : public Singleton<RobotManager>
{
private:
    UINT SIZE = 5; // 최대 몇 개체까지 관리할 것인가 (범위 크기)
    float SPAWN_TIME = 15.0f; // 생성 혹은 재생성에 필요한 시간 (간격)

public:
    //외부호출용

    RobotManager();
    ~RobotManager();

    void Update();
    void Render();
    void PostRender();
    void GUIRender();

    //로봇 혹은 로봇들이 누구를 쫓아갈 것인지 매니저에서 지정 가능하도록
    void SetTarget(Transform* target);

    vector<Robot*> GetRobots() { return robots; }

private:
    //내부멤버함수
    void Collision(); // 충돌 판정 진행
    void Spawn(); // (재)생성

private:
    //내부멤버변수

    ModelAnimatorInstancing* robotInstancing;
    vector<Robot*> robots;

    Transform* target;

    float time = 0; // 경과 시간

};

