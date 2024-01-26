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
	//	// Mesh ��� üũ
	//	if (ImGui::BeginDragDropTarget())
	//	{
	//		// �ش� ��忡�� ���콺 �� ���, ������ PayLoad Ű���� ��ġ�� ���
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
	//		// �׸� ���ý� ȣ����� ��������Ʈ ���
	//		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMesh);
	//	}
	//}

	// �ӵ� ����
	float fTimeScale = CTimeMgr::GetInst()->GetTimeScale();

	ImGui::SliderFloat("TimeScale :	", &fTimeScale, 0.01f, 5.49f, "ratio = %.2f");
	CTimeMgr::GetInst()->SetTimeScale(fTimeScale);

	// �ִϸ����� ����
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

	// �ִϸ��̼� ����
	static char inputAnimName[128] = "Hello, world!";
	static float inputStartTime = 0.001f;
	static float inputEndTime = 0.001f;

	if (ImGui::TreeNode("New Create Anim"))
	{
		// �Է� ����
		ImGui::InputText("input text", inputAnimName, IM_ARRAYSIZE(inputAnimName));

		ImGui::InputFloat("input StartTime	", &inputStartTime, 0.0f, fTimeLength, "%.2f");
		ImGui::InputFloat("input EndTime	", &inputEndTime, inputStartTime, fTimeLength, "%.2f");

		if (ImGui::Button("Create!"))
		{
			GetTarget()->Animator3D()->CreateAnimation3D(inputAnimName, curClipIdx, inputStartTime, inputEndTime);
		}

		ImGui::TreePop();
	}

	// �ִϸ��̼� ����Ʈ
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Select Anims");
	AnimList();

	// �ִϸ��̼� ���
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
	

	// �ִϸ��̼� ����
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

		// save ��ư �߰��ؼ� anim���� �����ϴ� ��ɵ� �߰��ؾ��ҵ�
		ImGui::TreePop();
	}
	
	
	return TRUE;
}

void Animator3DUI::AnimList()
{
	static vector<const char*> animList;
	map<string, CAnim3D*> anims = GetTarget()->Animator3D()->GetAnims();
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
				GetTarget()->Animator3D()->Change(animList[idx]);
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	animList.clear();
}
