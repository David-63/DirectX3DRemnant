#include "pch.h"
#include "DecalUI.h"
#include "ImGuiMgr.h"
#include "ListUI.h"

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
	ImGui::Text("Deferred");
	ImGui::SameLine();
	ImGui::Checkbox("##Decal Deferred", &isDeferred);

	bool isEmissive = GetTarget()->Decal()->isEmissive();
	ImGui::Text("Emissive");
	ImGui::SameLine();
	ImGui::Checkbox("##Decal Emissive", &isEmissive);

	bool bShow = GetTarget()->Decal()->IsDebugRendering();
	ImGui::Text("DebugRender");
	ImGui::SameLine();
	ImGui::Checkbox("##Light DebugRender", &bShow);

	GetTarget()->Decal()->SetDeferredDecal(isDeferred);
	GetTarget()->Decal()->ActivateEmissive(isEmissive);
	GetTarget()->Decal()->SetDebugRendering(bShow);

	// 여기에 텍스쳐 바인딩하는 기능도?
	Ptr<CTexture> pTex = GetTarget()->Decal()->GetTexture();
	char szBuff[50] = {};

	if (pTex.Get())
	{
		ImGui::Text("Texture    ");
		ImGui::SameLine();
		GetResKey(pTex.Get(), szBuff, 50);
		ImGui::InputText("##MeshName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

	}

	if (ImGui::Button("##TextureSelectBtn", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapTex = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Texture List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapTex)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&DecalUI::SelectTexture);
	}
	return TRUE;
}

void DecalUI::SelectTexture(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->Decal()->SetTexture(pTex);
}
