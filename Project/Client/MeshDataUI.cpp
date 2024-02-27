#include "pch.h"
#include "MeshDataUI.h"
#include <Engine\CMeshData.h>
#include <Engine\CGameObject.h>

MeshDataUI::MeshDataUI()
    : ResUI(RES_TYPE::MESHDATA)
{
    SetName("MeshData");
}

MeshDataUI::~MeshDataUI()
{
}


int MeshDataUI::render_update()
{
    ResUI::render_update();

    Ptr<CMeshData> pMeshData = (CMeshData*)GetTargetRes().Get();

    ImGui::InputText("##Name", mName, sizeof(mName));
    wstring wstr(mName, mName + strlen(mName));
    
    if (ImGui::Button("Instantiate"))
    {
        CGameObject* obj = pMeshData->Instantiate();
        obj->SetName(wstr);
        SpawnGameObject(obj, Vec3(0.f, 0.f, 0.f), L"Default");
    }

    return 0;
}