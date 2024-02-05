#include "pch.h"
#include "AnimClipUI.h"

#include <Engine/CAnimClip.h>

AnimClipUI::AnimClipUI()
	: ResUI(RES_TYPE::ANIMCLIP)
{
	SetName("AnimClip");
}

AnimClipUI::~AnimClipUI()
{
}


int AnimClipUI::render_update()
{
	ResUI::render_update();

	ImGui::Text("AnimClip ");


	Ptr<CAnimClip> pAnim = (CAnimClip*)GetTargetRes().Get();
	string strKey = string(pAnim->GetKey().begin(), pAnim->GetKey().end());
	ImGui::InputText("##AnimClipUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();
	return 0;
}
