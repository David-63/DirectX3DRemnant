#include "pch.h"
#include "DecalUI.h"


#include <Engine/CDecal.h>
#include <Engine/CResMgr.h>

DecalUI::DecalUI() : ComponentUI("##DecalUI", COMPONENT_TYPE::DECAL)
{
	SetName("DecalUI");
}

DecalUI::~DecalUI()
{
}

int DecalUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	bool isDeferred = GetTarget()->Decal()->IsDeferredDecal();
	bool isEmissive = GetTarget()->Decal()->isEmissive();
	bool bShow = GetTarget()->Decal()->IsDebugRendering();

	ImGui::Text("Deferred");
	ImGui::SameLine();
	ImGui::Checkbox("##Decal Deferred", &isDeferred);

	ImGui::Text("Emissive");
	ImGui::SameLine();
	ImGui::Checkbox("##Decal Emissive", &isEmissive);

	ImGui::Text("DebugRender");
	ImGui::SameLine();
	ImGui::Checkbox("##Light DebugRender", &bShow);

	GetTarget()->Decal()->SetDeferredDecal(isDeferred);
	GetTarget()->Decal()->ActivateEmissive(isEmissive);
	GetTarget()->Decal()->SetDebugRendering(bShow);

	// 여기에 텍스쳐 바인딩하는 기능도?

	return TRUE;
}
