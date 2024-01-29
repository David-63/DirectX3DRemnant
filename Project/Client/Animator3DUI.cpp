#include "pch.h"
#include "Animator3DUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CTimeMgr.h>

Animator3DUI::Animator3DUI() : ComponentUI("##Animator3D", COMPONENT_TYPE::ANIMATOR3D)
{
	SetName("Animator3D");
}

Animator3DUI::~Animator3DUI()
{
}

int Animator3DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	/*if (ImGui::Button("Play"))
	{
		GetTarget()->Animator3D()->Continue(repeat);
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		GetTarget()->Animator3D()->Stop();
	}*/


	//{
	//	ImGui::Text("Mesh    ");
	//	ImGui::SameLine();
	//	GetResKey(pMesh.Get(), szBuff, 50);
	//	ImGui::InputText("##MeshName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);
	//	// Mesh 드랍 체크
	//	if (ImGui::BeginDragDropTarget())
	//	{
	//		// 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
	//		const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
	//		if (pPayLoad)
	//		{
	//			TreeNode* pNode = (TreeNode*)pPayLoad->Data;
	//			CRes* pRes = (CRes*)pNode->GetData();
	//			if (RES_TYPE::MESH == pRes->GetType())
	//			{
	//				GetTarget()->MeshRender()->SetMesh((CMesh*)pRes);
	//			}
	//		}
	//		ImGui::EndDragDropTarget();
	//	}
	//	ImGui::SameLine();
	//	if (ImGui::Button("##MeshSelectBtn", ImVec2(18, 18)))
	//	{
	//		const map<wstring, Ptr<CRes>>& mapMesh = CResMgr::GetInst()->GetResources(RES_TYPE::MESH);
	//		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
	//		pListUI->Reset("Mesh List", ImVec2(300.f, 500.f));
	//		for (const auto& pair : mapMesh)
	//		{
	//			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
	//		}
	//		// 항목 선택시 호출받을 델리게이트 등록
	//		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMesh);
	//	}
	//}

	// 속도 조절
	float fTimeScale = CTimeMgr::GetInst()->GetTimeScale();

	ImGui::SliderFloat("TimeScale :	", &fTimeScale, 0.01f, 5.49f, "ratio = %.2f");
	CTimeMgr::GetInst()->SetTimeScale(fTimeScale);

	// 애니메이터 정보
	const vector<tMTAnimClip>* AnimClips = GetTarget()->Animator3D()->GetAnimClip();
	int curClipIdx = GetTarget()->Animator3D()->GetClipIdx();
	int iClipCount = AnimClips->size();

	float fTimeLength = static_cast<float>(AnimClips->at(curClipIdx).dTimeLength);
	
	if (ImGui::TreeNode("Clip Info"))
	{
		ImGui::Text("AnimClipCount %i", iClipCount);
		ImGui::Text("Clip TimeLength %.1f", fTimeLength);
		ImGui::Text("Clip FrameLenght %i", AnimClips->at(curClipIdx).iFrameLength);
		ImGui::TreePop();
	}

	// 애니메이션 생성
	static char inputAnimName[128] = "Hello, world!";
	static float inputStartTime = 0.001f;
	static float inputEndTime = 0.001f;

	if (ImGui::TreeNode("New Create Anim"))
	{
		// 입력 인자
		ImGui::InputText("input text", inputAnimName, IM_ARRAYSIZE(inputAnimName));

		ImGui::InputFloat("input StartTime	", &inputStartTime, 0.0f, fTimeLength, "%.2f");
		ImGui::InputFloat("input EndTime	", &inputEndTime, inputStartTime, fTimeLength, "%.2f");

		if (ImGui::Button("Create!"))
		{
			wstring outputName(inputAnimName, &inputAnimName[128]);
			GetTarget()->Animator3D()->CreateAnimation3D(outputName, curClipIdx, inputStartTime, inputEndTime);
		}

		ImGui::TreePop();
	}

	// 애니메이션 리스트
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Select Anims");
	AnimList();

	// 애니메이션 재생
	bool repeat = GetTarget()->Animator3D()->IsRepeat();
	ImGui::Checkbox("Is Repeat?", &repeat);
	GetTarget()->Animator3D()->SetRepeat(repeat);
	ImGui::SameLine();
	if (ImGui::Button("Play"))
	{
		GetTarget()->Animator3D()->Continue(repeat);
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		GetTarget()->Animator3D()->Stop();
	}
	

	// 애니메이션 정보
	float* fStartTime = GetTarget()->Animator3D()->GetStartTime();
	float* fEndTime = GetTarget()->Animator3D()->GetEndTime();
	float fCurTime = GetTarget()->Animator3D()->GetCurTime();
	int curFrame = GetTarget()->Animator3D()->GetCurFrame();

	if (ImGui::TreeNode("Animation Info"))
	{
		ImGui::SliderFloat("StartTime	", fStartTime, 0.0f, fTimeLength, "ratio = %.2f");

		if (fStartTime >= fEndTime)
			fEndTime = fStartTime;
		ImGui::SliderFloat("EndTime		", fEndTime, *fStartTime, fTimeLength, "ratio = %.2f");
				
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "UpdateTime	%.1f", fCurTime);
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "UpdateFrame % i", curFrame);

		// save 버튼 추가해서 anim파일 생성하는 기능도 추가해야할듯
		ImGui::TreePop();
	}
	
	
	return TRUE;
}

void Animator3DUI::AnimList()
{
	static vector<const char*> animList;
	map<wstring, CAnim3D*> anims = GetTarget()->Animator3D()->GetAnims();
	for (const auto& anim : anims)
	{
		animList.push_back(anim.second->GetAnimName().c_str());
	}

	static int item_current_idx = 0; // Here we store our selection data as an index.
	const char* combo_preview_value = animList[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
	if (ImGui::BeginCombo("Anim Key List", combo_preview_value))
	{
		for (int idx = 0; idx < animList.size(); idx++)
		{
			const bool is_selected = (item_current_idx == idx);
			if (ImGui::Selectable(animList[idx], is_selected))
			{
				item_current_idx = idx;
				std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;				
				GetTarget()->Animator3D()->Change(converter.from_bytes(animList[idx]));
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	animList.clear();
}
