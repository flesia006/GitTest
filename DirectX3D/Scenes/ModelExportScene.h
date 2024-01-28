#pragma once

class ModelExportScene : public Scene
{
public:
    ModelExportScene();
    ~ModelExportScene();

    void ExportAnimation(string fileName, string name);
        
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;
};