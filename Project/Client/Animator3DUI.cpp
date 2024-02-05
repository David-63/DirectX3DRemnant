#include "pch.h"
#include "Animator3DUI.h"

#include "ListUI.h"
#include "TreeUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CMeshRender.h>

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
	ImGui::Separator();
	
	// 속도 조절
	float fTimeScale = CTimeMgr::GetInst()->GetTimeScale();
	ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "Change Time Scale");
	
	ImGui::SliderFloat("Scale", &fTimeScale, 0.01f, 5.49f, "ratio = %.2f");
	CTimeMgr::GetInst()->SetTimeScale(fTimeScale);
	ImGui::Separator();

	// ====================================================================================================================
	

	bool repeat = GetTarget()->Animator3D()->IsRepeat();
	map<wstring, Ptr<CAnimClip>> anims = GetTarget()->Animator3D()->GetAnims();
	static vector<const char*> animList;
	for (const auto& anim : anims)
	{
		animList.push_back(anim.second.Get()->GetAnimName().c_str());
	}	
	static int item_current_idx = 0; // Here we store our selection data as an index.
	const char* combo_preview_value = animList[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)	
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> charToWstring;
	
	
	// 필요한 변수 anims
	ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "AnimClip List");
	ImGui::Separator();
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
	ImGui::Text("Add");
	ImGui::SameLine();
	if (ImGui::Button("##AnimClipSelectBtn", ImVec2(18, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapAnimClip = CResMgr::GetInst()->GetResources(RES_TYPE::ANIMCLIP);
		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("AnimClip List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapAnimClip)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}
		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&Animator3DUI::SelectAnimClip);
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove"))
	{
		
	}	
	ImGui::Separator();





	ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "AnimClip");
	ImGui::Separator();



	// ===================================================================================================

	// Mesh 선택
	Ptr<CMesh> pMesh = GetTarget()->MeshRender()->GetMesh();	// 초기에 보여주는 용도 외로는 안쓰는건가?
	char szBuff[50] = {};	
	GetResKey(pMesh.Get(), szBuff, 50);
	
	// Mesh가 있다면 MTAnimClip 정보 표시

	static int mtClipCount = 0;
	static int selectClipIdx = 0;
	static float selectTimeLength = 0.f;
	vector<tMTAnimClip> mtAnimClip;
	if (nullptr != m_SelectMesh)
	{
		if (m_SelectMesh->IsAnimMesh())
		{
			mtAnimClip = m_SelectMesh->GetMTAnimClips();
			mtClipCount = m_SelectMesh->GetMTAnimClips().size();
		}
	}

	// animclip//AnimTest

	// 애니메이션 생성
	static char inputAnimName[128] = "Write AnimName";
	static wstring inputClipName;
	static float inputStartTime = 0.001f;
	static float inputEndTime = 0.001f;

	// MT 정보
	//static char meshName[128] = "Write AnimName";

	if (ImGui::TreeNode("Create AnimClip"))
	{
		ImGui::Text("Select Mesh");

		// Mesh 파일명과 선택 버튼
		ImGui::InputText("##MeshName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);
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
			pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&Animator3DUI::SelectMesh);
		}


		// MT Datas
		ImGui::Text("MT ClipCount %d", mtClipCount);
		ImGui::InputInt("Select Clip Idx", &selectClipIdx);
		if (nullptr != m_SelectMesh)
		{
			if (m_SelectMesh->IsAnimMesh())
			{
				selectTimeLength = static_cast<float>(mtAnimClip[selectClipIdx].dTimeLength);
			}
		}
		
		ImGui::Text("TimeLength %.1f", selectTimeLength);

		// AnimClip
		ImGui::InputText("input Name", inputAnimName, IM_ARRAYSIZE(inputAnimName));

		ImGui::InputFloat("input StartTime	", &inputStartTime, 0.0f, selectTimeLength, "%.2f");
		ImGui::InputFloat("input EndTime	", &inputEndTime, inputStartTime, selectTimeLength, "%.2f");

		if (ImGui::Button("Create Now!"))
		{
			// 참조 복사 테스트 : 결론, 와이드 바이트에서 생기는 문제였음, 걍 복사 쓰면됨
			std::string outName = inputAnimName;
			std::string outPath;
			outPath += "animclip\\";
			outPath += outName + ".animclip";



			if (nullptr != m_SelectMesh)
				GetTarget()->Animator3D()->NewAnimClip(outPath, selectClipIdx, inputStartTime, inputEndTime, m_SelectMesh);
		}		

		ImGui::TreePop();
	}
	ImGui::Separator();
	ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "AnimClip Info");



	// ===================================================================================================


	// Cur Anim Info
	CAnimClip* curAnim = GetTarget()->Animator3D()->GetCurAnim(); // Current AnimClip
	float fFinishTime = curAnim->GetFinishTime();
	float fCurTime = curAnim->GetCurTime();
	int curFrame = curAnim->GetCurFrame();

	// Edit Clip values
	float fEditBeginTime;
	float fEditEndTime;
	static float v2EditTime[2] = { 0.f, 0.f };
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
	ImGui::Text("FinishTime	%.2f", fFinishTime);
	ImGui::Text("UpdateTime	%.1f", fCurTime);
	ImGui::Text("UpdateFrame % i", curFrame);
	ImGui::Separator();



	int curClipIdx = curAnim->GetClipIdx();
	vector<tMTAnimClip> AnimClips = curAnim->GetMTAnimClips();
	int iClipCount = AnimClips.size();
	float curTimeLength = static_cast<float>(AnimClips.at(curClipIdx).dTimeLength);
	float fBeginTime = curAnim->GetBeginTime();
	float fEndTime = curAnim->GetEndTime();

	if (ImGui::TreeNode("Edit Clip"))
	{
		ImGui::Text("AnimClipCount	  %i", iClipCount);
		ImGui::Text("TimeLength  %.1f", curTimeLength);
		ImGui::Text("FrameLenght %i", AnimClips.at(curClipIdx).iFrameLength);
		ImGui::Text("Begin To End %.1f - %.1f", fBeginTime, fEndTime);

		ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Edit Interface");
		ImGui::DragFloat2("Edit Time", v2EditTime, 0.01f, 0.0f, curTimeLength, "%.1f");
		if (ImGui::Button("Edit Now!"))
		{
			curAnim->Edit(v2EditTime[0], v2EditTime[1]);
		}
		ImGui::TreePop();
	}
	
	ImGui::Separator();
	



	// blend debug ui
	float fblendUpdate = GetTarget()->Animator3D()->GetBlendUpdateTime();
	float fblendRatio = GetTarget()->Animator3D()->GetBlendRatio();
	int iprevFrame = GetTarget()->Animator3D()->GetPrevFrameIdx();

	ImGui::Text("Blend Update %.1f", fblendUpdate);
	ImGui::Text("Blend Ratio  %.1f", fblendRatio);
	ImGui::Text("Prev Frame % i", iprevFrame);

	
	

	
	animList.clear();
	return TRUE;
}

void Animator3DUI::SelectMesh(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	m_SelectMesh = CResMgr::GetInst()->FindRes<CMesh>(wstring(strKey.begin(), strKey.end()));
}

void Animator3DUI::SelectAnimClip(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	m_SelectAnimClip = CResMgr::GetInst()->FindRes<CAnimClip>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->Animator3D()->Add(m_SelectAnimClip);
}
