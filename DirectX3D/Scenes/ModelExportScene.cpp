#include "Framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
    string name = "player";
    string file = "Models/FBX/" + name + ".fbx";
    ModelExporter* exporter;
    string clipName;

    exporter = new ModelExporter(name, file);
    //exporter->ExportMaterial();
    //exporter->ExportMesh();
    //delete exporter;    
    
    ExportAnimation("L_004", name);
//    ExportAnimation("L_127", name);
//    ExportAnimation("L_128", name);
//    ExportAnimation("L_130", name);
//    ExportAnimation("L_131", name);
//    ExportAnimation("L_132", name);
//    ExportAnimation("L_133", name);
//    ExportAnimation("L_134", name);
//    ExportAnimation("L_135", name);
//    ExportAnimation("L_136", name);
//    ExportAnimation("L_137", name);
//    ExportAnimation("L_138", name);
//    ExportAnimation("L_139", name);
//    ExportAnimation("L_140", name);
//    ExportAnimation("L_147", name);
//
//    ExportAnimation("L_151", name);
//    ExportAnimation("L_152", name);
//    ExportAnimation("L_153", name);
//    ExportAnimation("L_154", name);
//    ExportAnimation("L_155", name);
//    ExportAnimation("L_156", name);

}

ModelExportScene::~ModelExportScene()
{
}

void ModelExportScene::ExportAnimation(string fileName, string name)
{
    string clipName = fileName;
    string file = "Models/Animations/" + name + "/" + clipName + ".fbx";
    ModelExporter* exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
}

void ModelExportScene::Update()
{
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
}

void ModelExportScene::PostRender()
{
}

void ModelExportScene::GUIRender()
{
}
