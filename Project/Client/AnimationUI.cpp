#include "pch.h"
#include "AnimationUI.h"
#include <Engine\CMesh.h>

AnimationUI::AnimationUI() : ResUI(RES_TYPE::MESH)
{
    SetName("Animation");
}

AnimationUI::~AnimationUI()
{
}

int AnimationUI::render_update()
{
    ResUI::render_update();

    ImGui::Text("Animation ");
    ImGui::SameLine();

    Ptr<CMesh> pMesh = (CMesh*)GetTargetRes().Get();
    string strKey = string(pMesh->GetKey().begin(), pMesh->GetKey().end());
    ImGui::InputText("##AnimUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

    return 0;
}