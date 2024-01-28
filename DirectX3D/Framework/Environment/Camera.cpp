#include "Framework.h"

Camera::Camera()
{
    tag = "Camera";

    viewBuffer = new ViewBuffer();
    viewBuffer->SetVS(1);

    Load();

    camSphere = new SphereCollider(distance);
    camSphere->SetParent(this);
    camSphere->UpdateWorld();

    ground = new BoxCollider({ FLT_MAX, 0.1, FLT_MAX });
    ground->UpdateWorld();

    prevMousePos = mousePos;
    prevPos = Pos();

    sight.dir = Vector3::Forward();
    sightRot = new Transform();
}

Camera::~Camera()
{
    delete viewBuffer;

    Save();
}

void Camera::Update()
{
    projection = Environment::Get()->GetProjection();
    Frustum();

    if (target)
        //ThirdPersonMode();
        ThirdPresonViewMode();
    else
        FreeMode();

    UpdateWorld();
    camSphere->UpdateWorld();

    view = XMMatrixInverse(nullptr, world);
    viewBuffer->Set(view, world);
}

void Camera::GUIRender()
{
    if (ImGui::TreeNode("CameraOption"))
    {
        ImGui::DragFloat("MoveSpeed", &moveSpeed);
        ImGui::DragFloat("RotSpeed", &rotSpeed);

        if (target && ImGui::TreeNode("TargetOption"))
        {
            ImGui::DragFloat("Distance", &distance, 0.1f);
            ImGui::DragFloat("Height", &height, 0.1f);
            ImGui::DragFloat3("FocusOffset", (float*)&focusOffset, 0.1f);

            float degree = XMConvertToDegrees(rotY);
            ImGui::DragFloat("RotY", &degree, 0.1f);
            rotY = XMConvertToRadians(degree);

            ImGui::DragFloat("MoveDamping", &moveDamping, 0.1f);
            ImGui::DragFloat("RotDamping", &rotDamping, 0.1f);

            ImGui::Checkbox("LookAtTargetX", &isLookAtTargetX);
            ImGui::Checkbox("LookAtTargetY", &isLookAtTargetY);
            
            ImGui::InputText("File", file, 128);

            if(ImGui::Button("Save"))
                TargetOptionSave(file);
            ImGui::SameLine();
            if (ImGui::Button("Load"))
                TargetOptionLoad(file);

            ImGui::TreePop();
        }

        Transform::GUIRender();

        ImGui::TreePop();
    }
}

void Camera::SetView()
{  
    viewBuffer->SetVS(1);
    viewBuffer->SetPS(1);
}

Vector3 Camera::ScreenToWorld(Vector3 screenPos)
{
    return XMVector3TransformCoord(screenPos, world);
}

Vector3 Camera::WorldToScreen(Vector3 worldPos)
{
    Vector3 screenPos;

    screenPos = XMVector3TransformCoord(worldPos, view);
    screenPos = XMVector3TransformCoord(screenPos, projection);
    //NDC : -1 ~ 1

    screenPos = (screenPos + Vector3::One()) * 0.5f;//0~1

    screenPos.x *= WIN_WIDTH;
    screenPos.y *= WIN_HEIGHT;

    return screenPos;
}

Ray Camera::ScreenPointToRay(Vector3 screenPoint)
{
    Vector3 screenSize(WIN_WIDTH, WIN_HEIGHT, 1.0f);

    Vector2 point;
    point.x = (screenPoint.x / screenSize.x) * 2.0f - 1.0f;
    point.y = (screenPoint.y / screenSize.y) * 2.0f - 1.0f;    

    Float4x4 temp;
    XMStoreFloat4x4(&temp, projection);

    screenPoint.x = point.x / temp._11;
    screenPoint.y = point.y / temp._22;
    screenPoint.z = 1.0f;

    screenPoint = XMVector3TransformNormal(screenPoint, world);

    Ray ray;
    ray.pos = Pos();
    ray.dir = screenPoint.GetNormalized();

    return ray;
}

void Camera::LookAtTarget()
{    
    rotMatrix = XMMatrixRotationY(target->Rot().y + rotY);

    Vector3 forward = XMVector3TransformNormal(Vector3::Forward(), rotMatrix);

    Pos() = target->GlobalPos() + forward * -distance;
    Pos().y += height;    

    Vector3 offset = XMVector3TransformCoord(focusOffset, rotMatrix);
    Vector3 targetPos = target->GlobalPos() + offset;

    Vector3 dir = (targetPos - Pos()).GetNormalized();
    forward = Vector3(dir.x, 0.0f, dir.z).GetNormalized();

    Rot().x = acos(Dot(forward, dir));
    Rot().y = atan2(dir.x, dir.z);
}

void Camera::FreeMode()
{
    Vector3 delta = mousePos - prevMousePos;
    prevMousePos = mousePos;

    if (KEY_PRESS(VK_RBUTTON))
    {
        if (KEY_PRESS('W'))
            Pos() += Forward() * moveSpeed * DELTA;
        if (KEY_PRESS('S'))
            Pos() += Back() * moveSpeed * DELTA;
        if (KEY_PRESS('A'))
            Pos() += Left() * moveSpeed * DELTA;
        if (KEY_PRESS('D'))
            Pos() += Right() * moveSpeed * DELTA;
        if (KEY_PRESS('E'))
            Pos() += Down() * moveSpeed * DELTA;
        if (KEY_PRESS('Q'))
            Pos() += Up() * moveSpeed * DELTA;

        Rot().x -= delta.y * rotSpeed * DELTA;
        Rot().y += delta.x * rotSpeed * DELTA;
    }
}

void Camera::FollowMode()
{   
    destRot = Lerp(destRot, target->Rot().y, rotDamping * DELTA);    
    rotMatrix = XMMatrixRotationY(destRot + rotY);

    Vector3 forward = XMVector3TransformNormal(Vector3::Forward(), rotMatrix);

    destPos = target->GlobalPos() + forward * -distance;
    destPos.y += height;

    Pos() = Lerp(Pos(), destPos, moveDamping * DELTA);    

    Vector3 offset = XMVector3TransformCoord(focusOffset, rotMatrix);
    Vector3 targetPos = target->GlobalPos() + offset;

    Vector3 dir = (targetPos - Pos()).GetNormalized();
    forward = Vector3(dir.x, 0.0f, dir.z).GetNormalized();

    if (isLookAtTargetX)
    {
        Rot().x = acos(Dot(forward, dir));        
    }    
    if (isLookAtTargetY)
    {
        Rot().y = atan2(dir.x, dir.z);
    }
}

void Camera::ThirdPersonMode()
{
    Vector3 delta = mousePos - prevMousePos;
    prevMousePos = mousePos;
    Vector3 posDelta = Pos() - prevPos;
    prevPos = Pos();


    destRot = Lerp(destRot, target->Rot().y / 600, rotDamping * DELTA);
    rotMatrix = XMMatrixRotationY(destRot + rotY);

    Vector3 forward = XMVector3TransformNormal(Forward(), rotMatrix);

    destPos = target->GlobalPos();
    //destPos.y += height / 3;

    Pos() = destPos;
    //Pos() = Lerp(Pos(), destPos, moveDamping * DELTA);


//    Vector3 offset = XMVector3TransformCoord(focusOffset, rotMatrix);
//    Vector3 targetPos = target->GlobalPos() + offset;
//
//    Vector3 dir = (targetPos - Pos()).GetNormalized();
//    forward = Vector3(dir.x, 0.0f, dir.z).GetNormalized();

    CAM->Rot().x -= delta.y * rotSpeed * DELTA;
    CAM->Rot().x = Clamp(-XM_PIDIV2 + 0.01f, XM_PIDIV2 - 0.01f, CAM->Rot().x);
    CAM->Rot().y += delta.x * rotSpeed * DELTA;


    Pos().z -= distance * 2 * cos(-Rot().x) * cos(Rot().y);
    Pos().x -= distance * 2 * cos(-Rot().x) * sin(Rot().y);
    
    Pos().y -= distance * 2 * sin(-Rot().x) - height / 2;

}

void Camera::ThirdPresonViewMode()
// 시선 반대 방향으로 광선을 쏘고 맞은 지점을 카메라 위치로 지정하는 방식
// 이후에 terrain 이나 월드의 여러 오브젝트에도 광선을 쏘아 카메라 위치를 정하면
// 지하로 들어가거나 물건을 통과하는 등의 상황을 방지할 수 있음
{
    Vector3 delta = mousePos - prevMousePos;
    prevMousePos = mousePos;    
    
    sightRot->Rot().x -= delta.y * rotSpeed * DELTA;
    sightRot->Rot().x = Clamp(-XM_PIDIV2 + 0.01f, XM_PIDIV2 - 0.01f, sightRot->Rot().x);
    sightRot->Rot().y += delta.x * rotSpeed * DELTA;
    sightRot->UpdateWorld();

    CAM->Rot() = sightRot->Rot();
    CAM->Pos() = target->GlobalPos() + sightRot->Back() * distance * 1.6;

    // 만약 카메라가 지면을 파고든다? (TODO : Terrain 만들면 그에 맞게 수정)

    // 1. 광선(뒤통수의 시선) 만들기
    sight.dir = sightRot->Back();
    sight.pos = target->GlobalPos();

    // 2. 땅과 contact 받아오기
    Contact contact;
    bool hitGround = ground->IsRayCollision(sight, &contact);

    // 3. 광선이 지면에 닿지 않았거나 camSphere 의 반지름보다 먼 거리에서 hit 되었다면 그냥 리턴
    if ((target->GlobalPos() - contact.hitPoint).Length() > distance || !hitGround)
        return;
    
    // 4. Cam 위치를 광선과 땅이 만난 지점으로, 
    //    대신 카메라가 너무 땅에 딱붙어 있으면 어색하니까 살짝 보정 

    CAM->Pos() = contact.hitPoint - sight.dir.Back() * 5;
}

void Camera::Frustum()
{
    Float4x4 VP;
    XMStoreFloat4x4(&VP, view * projection);

    //Left
    a = VP._14 + VP._11;
    b = VP._24 + VP._21;
    c = VP._34 + VP._31;
    d = VP._44 + VP._41;
    planes[0] = XMVectorSet(a, b, c, d);

    //Right
    a = VP._14 - VP._11;
    b = VP._24 - VP._21;
    c = VP._34 - VP._31;
    d = VP._44 - VP._41;
    planes[1] = XMVectorSet(a, b, c, d);

    //Bottom
    a = VP._14 + VP._12;
    b = VP._24 + VP._22;
    c = VP._34 + VP._32;
    d = VP._44 + VP._42;
    planes[2] = XMVectorSet(a, b, c, d);

    //Top
    a = VP._14 - VP._12;
    b = VP._24 - VP._22;
    c = VP._34 - VP._32;
    d = VP._44 - VP._42;
    planes[3] = XMVectorSet(a, b, c, d);

    //Near
    a = VP._14 + VP._13;
    b = VP._24 + VP._23;
    c = VP._34 + VP._33;
    d = VP._44 + VP._43;
    planes[4] = XMVectorSet(a, b, c, d);

    //Far
    a = VP._14 - VP._13;
    b = VP._24 - VP._23;
    c = VP._34 - VP._33;
    d = VP._44 - VP._43;
    planes[5] = XMVectorSet(a, b, c, d);

    FOR(6)
        planes[i] = XMPlaneNormalize(planes[i]);
}

void Camera::TargetOptionSave(string file)
{
    string path = "TextData/Camera/" + file + ".cam";

    BinaryWriter* writer = new BinaryWriter(path);

    writer->Float(distance);
    writer->Float(height);
    writer->Float(moveDamping);
    writer->Float(rotDamping);
    writer->Float(rotY);
    writer->Vector(focusOffset);
    writer->Bool(isLookAtTargetX);
    writer->Bool(isLookAtTargetY);

    delete writer;
}

void Camera::TargetOptionLoad(string file)
{
    string path = "TextData/Camera/" + file + ".cam";

    BinaryReader* reader = new BinaryReader(path);

    distance = reader->Float();
    height = reader->Float();
    moveDamping = reader->Float();
    rotDamping = reader->Float();
    rotY = reader->Float();
    focusOffset = reader->Vector();
    isLookAtTargetX = reader->Bool();
    isLookAtTargetY = reader->Bool();

    delete reader;
}

bool Camera::ContainPoint(Vector3 point)
{
    FOR(6)
    {
        Vector3 dot = XMPlaneDotCoord(planes[i], point);

        if (dot.x < 0.0f)
            return false;
    }

    return true;
}
