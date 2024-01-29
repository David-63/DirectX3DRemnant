#include "pch.h"
#include "SkyBoxUI.h"

#include <Engine\CSkyBox.h>
#include <Engine\CResMgr.h>

#include "ImGuiMgr.h"
#include "ListUI.h"

#include "TreeUI.h"

SkyBoxUI::SkyBoxUI() : ComponentUI("##Skybox", COMPONENT_TYPE::SKYBOX)
{
	SetName("Skybox");
}

SkyBoxUI::~SkyBoxUI()
{
}

int SkyBoxUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	char szBuff[50] = {};
	int skyType = static_cast<int>(GetTarget()->SkyBox()->GetSkyBoxType());
	Ptr<CTexture> skyTexture = GetTarget()->SkyBox()->GetSkyBoxTexture();


	ImGui::Text("SkyType");
	ImGui::SameLine();
	ImGui::SliderInt("##Sky Type", &skyType, 0, 1);


	ImGui::Text("SkyTexture");
	ImGui::SameLine();
	GetResKey(skyTexture.Get(), szBuff, 50);
	ImGui::InputText("##TextureName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

	// Texture 드랍 체크
	if (ImGui::BeginDragDropTarget())
	{
		// 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
		const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
		if (pPayLoad)
		{
			TreeNode* pNode = (TreeNode*)pPayLoad->Data;
			CRes* pRes = (CRes*)pNode->GetData();
			if (RES_TYPE::TEXTURE == pRes->GetType())
			{
				GetTarget()->SkyBox()->SetSkyBoxTexture((CTexture*)pRes);
			}
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::SameLine();

	// Texture 선택 버튼
	if (ImGui::Button("##TextureSelectBtn", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapTexture = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Texture List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapTexture)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&SkyBoxUI::SelectTexture);
	}

	GetTarget()->SkyBox()->SetSkyBoxType(static_cast<SKYBOX_TYPE>(skyType));

	return TRUE;
}

void SkyBoxUI::SelectTexture(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pTexture = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->SkyBox()->SetSkyBoxTexture(pTexture);
}
