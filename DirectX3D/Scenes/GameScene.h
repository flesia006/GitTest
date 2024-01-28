#pragma once
class GameScene : public Scene
{
public:

    GameScene();
    ~GameScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:

private:
    Naruto* naruto = nullptr;  
    Player* player = nullptr;
    ModelAnimator* boss = nullptr;

    Model* ground = nullptr;
    SkyBox* sky = nullptr;




};

