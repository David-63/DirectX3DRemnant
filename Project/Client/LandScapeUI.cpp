#include "pch.h"
#include "LandScapeUI.h"

#include <Engine\CLandScape.h>
#include <Engine\CResMgr.h>

#include "ListUI.h"
#include "TreeUI.h"

LandScapeUI::LandScapeUI()
	: ComponentUI("##LandScapeUI", COMPONENT_TYPE::LANDSCAPE)
{
	SetName("LandScapeUI");
}

LandScapeUI::~LandScapeUI()
{
}

int LandScapeUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "FaceMash");
	ImGui::Separator();

	// 메쉬 선택하는 기능
	char szMeshBuff[50] = {};

	Ptr<CMesh> pMesh = GetTarget()->LandScape()->GetMesh();
	ImGui::Text("Mesh    ");
	ImGui::SameLine();
	GetResKey(pMesh.Get(), szMeshBuff, 50);
	ImGui::InputText("##MeshName", szMeshBuff, 50, ImGuiInputTextFlags_ReadOnly);

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
				GetTarget()->LandScape()->SetMesh((CMesh*)pRes);
			}
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::SameLine();
	if (ImGui::Button("##MeshSelectBtn", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapMesh = CResMgr::GetInst()->GetResources(RES_TYPE::MESH);
		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Mesh List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapMesh)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}
		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&LandScapeUI::SelectFaceMesh);
	}
	ImGui::Separator();
	tUINTS faceSize = GetTarget()->LandScape()->GetFaceSize();
	int inoutFace[2] = { faceSize.X, faceSize.Y };
	ImGui::InputInt2("Set FaceSize", inoutFace);
	if (ImGui::Button("Change FaceSize", ImVec2(108, 18)))
	{
		GetTarget()->LandScape()->SetFaceSize(inoutFace[0], inoutFace[1]);
	}
	
	ImGui::Separator();
	static char inoutMeshName[128] = "sampleFaceMesh";
	static int face[2];
	if (ImGui::TreeNode("Create FaceMesh"))
	{
		ImGui::InputText("input Name", inoutMeshName, IM_ARRAYSIZE(inoutMeshName));
		ImGui::DragInt("input FaceX", &face[0], 1, 1); // 1씩 증가, 최소 값은 1
		ImGui::DragInt("input FaceZ", &face[1], 1, 1);
		if (ImGui::Button("Create Now!"))
		{
			std::string outName = inoutMeshName;
			GetTarget()->LandScape()->MakeFaceMesh(outName, face[0], face[1]);				
		}
		ImGui::TreePop();
	}

	ImGui::Separator();
	ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "HeightMap");
	ImGui::Separator();
	char szBuff[50] = {};
	Ptr<CTexture> pheightMapTex = GetTarget()->LandScape()->GetHeightMap();
	ImGui::Text("HeightmapTex ");
	ImGui::SameLine();
	GetResKey(pheightMapTex.Get(), szBuff, 50);
	ImGui::InputText("##TextureName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
		if (pPayLoad)
		{
			TreeNode* pNode = (TreeNode*)pPayLoad->Data;
			CRes* pRes = (CRes*)pNode->GetData();
			if (RES_TYPE::TEXTURE == pRes->GetType())
			{
				GetTarget()->LandScape()->SetHeightMap((CTexture*)pRes);
			}
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::SameLine();
	if (ImGui::Button("##HeightMapSelectBtn", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapTex = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);
		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Texture List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapTex)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&LandScapeUI::SelectHeightMapTex);
	}
	return TRUE;
}

void LandScapeUI::SelectFaceMesh(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CMesh> pFaceMesh = CResMgr::GetInst()->FindRes<CMesh>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->LandScape()->SetFaceMesh(pFaceMesh);
}

void LandScapeUI::SelectHeightMapTex(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pHeightMapTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->LandScape()->SetHeightMap(pHeightMapTex);
}
