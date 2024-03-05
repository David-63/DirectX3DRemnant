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

	char szBuff[50] = {};

	Ptr<CMeshData> pMeshData = GetTarget()->MeshRender()->GetMeshData();
	Ptr<CMesh> pMesh = GetTarget()->MeshRender()->GetMesh();
	Ptr<CMaterial> pMtrl = GetTarget()->MeshRender()->GetMaterial(0);
	ImGui::Text("MeshData ");
	ImGui::SameLine();
	GetResKey(pMesh.Get(), szBuff, 50);
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


	if (ImGui::Button("##MeshSelectBtn", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapMeshData = CResMgr::GetInst()->GetResources(RES_TYPE::MESHDATA);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("MeshData List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapMeshData)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMeshData);
	}

	bool isFrustum = GetTarget()->MeshRender()->IsUseFrustumCheck();
	bool isDynamic = GetTarget()->MeshRender()->IsDynamicShadow();
	float fBounding = GetTarget()->MeshRender()->GetBounding();

	ImGui::Text("Frustum");
	ImGui::SameLine();
	ImGui::Checkbox("##Check Frustum", &isFrustum);
	GetTarget()->MeshRender()->SetFrustumCheck(isFrustum);

	ImGui::Text("DynamicShadow");
	ImGui::SameLine();
	ImGui::Checkbox("##Check DynamicShadow", &isDynamic);
	GetTarget()->MeshRender()->SetDynamicShadow(isDynamic);

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

void MeshRenderUI::SelectMesh(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->MeshRender()->SetMesh(pMesh);
}

void MeshRenderUI::SelectMaterial(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->MeshRender()->SetMaterial(pMtrl, 0);
}