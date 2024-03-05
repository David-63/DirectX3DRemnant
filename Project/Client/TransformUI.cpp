#include "pch.h"
#include "TransformUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CResMgr.h>

TransformUI::TransformUI()
	: ComponentUI("##TransformUI", COMPONENT_TYPE::TRANSFORM)	
{
	SetName("TransformUI");
}

TransformUI::~TransformUI()
{
}

int TransformUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	Vec3 vPos = GetTarget()->Transform()->GetRelativePos();
	Vec3 vScale = GetTarget()->Transform()->GetRelativeScale();
	Vec3 vRotation = GetTarget()->Transform()->GetRelativeRot();
	vRotation = (vRotation / XM_PI) * 180.f;

	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Position", vPos);
	GetTarget()->Transform()->SetRelativePos(vPos);

	ImGui::Text("Scale   ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Scale", vScale);
	GetTarget()->Transform()->SetRelativeScale(vScale);

	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Rotation", vRotation);
	vRotation = (vRotation / 180.f) * XM_PI;
	GetTarget()->Transform()->SetRelativeRot(vRotation);


	float scaleRatio = 1.f;
	ImGui::Text("ScaleRatio");
	ImGui::SameLine();
	ImGui::DragFloat("##Relative Rotation", &scaleRatio, 0.02f);	
	GetTarget()->Transform()->SetScaleRatio(scaleRatio);






	if (ImGui::TreeNode("Create Prefab"))
	{
		ImGui::InputText("##Name", mName, sizeof(mName));

		wstring wstr(mName, mName + strlen(mName));

		const wstring& strPath = L"prefab\\" + wstr + L".pref";

		if (ImGui::Button("Create Prefab This"))
		{
			CResMgr::GetInst()->SavePrefab(GetTarget(), strPath);
		}

		ImGui::TreePop();
	}

	return TRUE;
}
