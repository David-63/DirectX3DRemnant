#include "pch.h"
#include "PrefabUI.h"
#include <Engine\CPrefab.h>
#include <Engine\CResMgr.h>

PrefabUI::PrefabUI()
    :ResUI(RES_TYPE::PREFAB)
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

    if (ImGui::Button("Instantiate"))
    {
        CGameObject* obj = CResMgr::GetInst()->LoadPrefab(path)->Instantiate(mPosition, mLayerIdx);
        SpawnGameObject(obj, mPosition, mLayerIdx);
    }

    return 0;
}