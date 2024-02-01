#include "pch.h"
#include "Animator3DUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CTimeMgr.h>

Animator3DUI::Animator3DUI() : ComponentUI("##Animator3DUI", COMPONENT_TYPE::ANIMATOR3D)
{
	SetName("Animator3DUI");
}

Animator3DUI::~Animator3DUI()
{
}

int Animator3DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;
	
	
	// �ӵ� ����
	float fTimeScale = CTimeMgr::GetInst()->GetTimeScale();
	ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "Change Time Scale");
	
	ImGui::SliderFloat("Scale", &fTimeScale, 0.01f, 5.49f, "ratio = %.2f");
	CTimeMgr::GetInst()->SetTimeScale(fTimeScale);
	
	// ================================
	// ���⿡ �ִϸ��̼� ���� �� ��ƾ߰ڴ�
	// ================================
	bool repeat = GetTarget()->Animator3D()->IsRepeat();												// �ݺ� �ɼ�
	map<wstring, CAnim3D*> anims = GetTarget()->Animator3D()->GetAnims();								// map AnimClip
	const vector<tMTAnimClip>* AnimClips = GetTarget()->Animator3D()->GetMTAnimClips();					// MT Clips
	CAnim3D* curAnim = GetTarget()->Animator3D()->GetCurAnim();											// Current AnimClip
	ImGui::Separator();






	
	// �ִ� Ŭ�� ����Ʈ
	
	static vector<const char*> animList;	
	for (const auto& anim : anims)
	{
		animList.push_back(anim.second->GetAnimName().c_str());
	}	
	static int item_current_idx = 0; // Here we store our selection data as an index.
	const char* combo_preview_value = animList[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)	
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> charToWstring;
	ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "Map List");
	if (ImGui::BeginCombo("Select", combo_preview_value))
	{
		for (int idx = 0; idx < animList.size(); idx++)
		{
			const bool is_selected = (item_current_idx == idx);
			if (ImGui::Selectable(animList[idx], is_selected))
			{
				item_current_idx = idx;
				GetTarget()->Animator3D()->Play(charToWstring.from_bytes(animList[item_current_idx]), repeat);
			}
	
			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	
	if (ImGui::Button("Add"))
	{
		// Load From File ���� �� ������ ��� �߰�
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove"))
	{
		// ���� ����		
	}	
	ImGui::Separator();





	ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "Clip Editor");
	int curClipIdx = curAnim->GetClipIdx();
	int iClipCount = AnimClips->size();
	float fTimeLength = static_cast<float>(AnimClips->at(curClipIdx).dTimeLength);
	float fBeginTime = curAnim->GetBeginTime();
	float fEndTime = curAnim->GetEndTime();
	
	// �ִϸ��̼� ����
	static char inputAnimName[128] = "Write AnimName";
	static float inputStartTime = 0.001f;
	static float inputEndTime = 0.001f;	
	if (ImGui::TreeNode("New Clip"))
	{
		// �Է� ����
		ImGui::InputText("input text", inputAnimName, IM_ARRAYSIZE(inputAnimName));
	
		ImGui::InputFloat("input StartTime	", &inputStartTime, 0.0f, fTimeLength, "%.2f");
		ImGui::InputFloat("input EndTime	", &inputEndTime, inputStartTime, fTimeLength, "%.2f");
	
		if (ImGui::Button("Create Now!"))
		{
			wstring outputName(inputAnimName, &inputAnimName[128]);
			GetTarget()->Animator3D()->NewAnimClip(outputName, curClipIdx, inputStartTime, inputEndTime);
		}
	
		ImGui::TreePop();
	}
	
	
	float fEditBeginTime;
	float fEditEndTime;
	static float v2EditTime[2] = { 0.f, 0.f };
	if (ImGui::TreeNode("Clip Info"))
	{
		ImGui::Text("AnimClipCount	  %i", iClipCount);
		ImGui::Text("TimeLength  %.1f", fTimeLength);
		ImGui::Text("FrameLenght %i", AnimClips->at(curClipIdx).iFrameLength);
		ImGui::Text("Begin To End %.1f - %.1f", fBeginTime, fEndTime);
	
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Edit Interface");
		ImGui::DragFloat2("Edit Time", v2EditTime, 0.01f, 0.0f, fTimeLength, "%.1f");
		if (ImGui::Button("Edit Now!"))
		{
			curAnim->Edit(v2EditTime[0], v2EditTime[1]);
		}	
		ImGui::TreePop();
	}	
	ImGui::Separator();
	




	// �ִϸ��̼� ����
	float fFinishTime = curAnim->GetFinishTime();
	float fCurTime = curAnim->GetCurTime();
	int curFrame = curAnim->GetCurFrame();
	
	ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "Animation");
	// �ִ� Ŭ�� ���
	
	if (ImGui::Button("Play"))
	{
		GetTarget()->Animator3D()->Play(charToWstring.from_bytes(animList[item_current_idx]), repeat);
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		curAnim->Stop();
	}
	ImGui::SameLine();
	ImGui::Checkbox("Is Repeat?", &repeat);
	GetTarget()->Animator3D()->SetRepeat(repeat);
	
	
	if (ImGui::TreeNode("Anim Info"))
	{
		ImGui::Text("FinishTime	%.2f", fFinishTime);
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "UpdateTime	%.1f", fCurTime);
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "UpdateFrame % i", curFrame);
	
		// save ��ư �߰��ؼ� anim���� �����ϴ� ��ɵ� �߰��ؾ��ҵ�
		ImGui::TreePop();
	}
	
	animList.clear();
	return TRUE;
}
