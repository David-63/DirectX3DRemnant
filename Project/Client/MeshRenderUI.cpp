#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine\CMeshRender.h>
#include <Engine\CResMgr.h>

#include "ImGuiMgr.h"
#include "ListUI.h"

#include "TreeUI.h"



MeshRenderUI::MeshRenderUI()
	: ComponentUI("##MeshRender", COMPONENT_TYPE::MESHRENDER)
{
	SetName("MeshRender");
}

MeshRenderUI::~MeshRenderUI()
{
}

int MeshRenderUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	char szBuff[200] = {};

	Ptr<CMeshData> pMeshData = GetTarget()->MeshRender()->GetMeshData();
	ImGui::Text("MeshData ");
	ImGui::SameLine();
	GetResKey(pMeshData.Get(), szBuff, 50);
	ImGui::InputText("##MeshDataName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);
	// Mesh 드랍 체크
	if (ImGui::BeginDragDropTarget())
	{
		// 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
		const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
		if (pPayLoad)
		{
			TreeNode* pNode = (TreeNode*)pPayLoad->Data;
			CRes* pRes = (CRes*)pNode->GetData();
			if (RES_TYPE::MESHDATA == pRes->GetType())
			{
				GetTarget()->MeshRender()->SetMeshData((CMeshData*)pRes);
			}
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::SameLine();
	if (ImGui::Button("##MeshDataSelectBtn", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapMeshData = CResMgr::GetInst()->GetResources(RES_TYPE::MESHDATA);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("MeshData List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapMeshData)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMeshData);
	}
	ImGui::Separator();


	ImGui::Separator();

	bool isFrustum = GetTarget()->MeshRender()->IsUseFrustumCheck();
	ImGui::Text("Frustum");
	ImGui::SameLine();
	ImGui::Checkbox("##Check Frustum", &isFrustum);
	GetTarget()->MeshRender()->SetFrustumCheck(isFrustum);

	bool isDynamic = GetTarget()->MeshRender()->IsDynamicShadow();
	ImGui::Text("DynamicShadow");
	ImGui::SameLine();
	ImGui::Checkbox("##Check DynamicShadow", &isDynamic);
	GetTarget()->MeshRender()->SetDynamicShadow(isDynamic);

	float fBounding = GetTarget()->MeshRender()->GetBounding();
	ImGui::Text("BoundingSize");
	ImGui::SameLine();
	ImGui::DragFloat("##Mesh BoundingSize", &fBounding);
	GetTarget()->MeshRender()->SetBounding(fBounding);


	return TRUE;
}

void MeshRenderUI::SelectMeshData(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->MeshRender()->SetMeshData(pMeshData);
}
