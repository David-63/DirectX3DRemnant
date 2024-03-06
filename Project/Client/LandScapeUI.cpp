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

	// FaceSize ¼¼ÆÃ
	tUINTS faceSize = GetTarget()->LandScape()->GetFaceSize();
	int faceSize2[2] = { faceSize.X, faceSize.Y };
	ImGui::DragInt2("input faceSize", faceSize2);
	faceSize.X = faceSize2[0];
	faceSize.Y = faceSize2[1];
	GetTarget()->LandScape()->SetFaceSize(faceSize);

	// FaceName
	string faceName = GetTarget()->LandScape()->GetFaceName();
	static char inOut_faceName[128] = {};
	std::strcpy(inOut_faceName, faceName.c_str());
	static wstring inputClipName;
	ImGui::InputText("input faceName", inOut_faceName, IM_ARRAYSIZE(inOut_faceName));
	GetTarget()->LandScape()->SetFaceName(inOut_faceName);
	// FaceMesh
	ImGui::Button("Create FaceMesh!", ImVec2(18, 18));
	{
		GetTarget()->LandScape()->CreateMesh();
	}

	// heightmap tex

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

void LandScapeUI::SelectHeightMapTex(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CTexture> pHeightMapTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->LandScape()->SetHeightMap(pHeightMapTex);
}
