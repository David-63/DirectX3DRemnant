#include "pch.h"
#include "PrefabUI.h"
#include <Engine\CPrefab.h>
#include <Engine\CResMgr.h>
#include <Engine\CGameObject.h>
#include <Engine\CResMgr.h>

PrefabUI::PrefabUI()
    :ResUI(RES_TYPE::PREFAB)
    , obj(nullptr)
{
    SetName("Prefab");
}

PrefabUI::~PrefabUI()
{

}

int PrefabUI::render_update()
{
    ResUI::render_update();

    Ptr<CPrefab> pPrefab = (CPrefab*)GetTargetRes().Get();
    const wstring path = pPrefab->GetRelativePath();

    ImGui::Text("LayerIndex  ");
    ImGui::SameLine();
    ImGui::InputInt("##Index", &mLayerIdx);

    ImGui::Text("Position  ");
    ImGui::SameLine();
    ImGui::DragFloat3("##Size", mPosition);

    // ===== Material
    ImGui::Text("Material");
    ImGui::SameLine();

    ImGui::Text("Name  ");
    ImGui::SameLine();
    ImGui::InputText("##Name", mName, sizeof(mName));
  
    wstring wstr(mName, mName + strlen(mName));

    if (ImGui::Button("Instantiate"))
    {
       wstring strPath = CPathMgr::GetInst()->GetContentPath();
       strPath += path;
       pPrefab->Load(strPath);
       CGameObject* obj = pPrefab->Instantiate(mPosition, mLayerIdx);
       obj->SetName(wstr);
       SpawnGameObject(obj, mPosition, mLayerIdx);
    }

    return 0;
}