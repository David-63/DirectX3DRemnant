#include "pch.h"
#include "Collider3DUI.h"


Collider3DUI::Collider3DUI() : ComponentUI("##Collider3DUI", COMPONENT_TYPE::COLLIDER3D)
{
	SetName("Collider3DUI");
}

Collider3DUI::~Collider3DUI()
{
}

int Collider3DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "Collider Type");
	ImGui::Separator();

	UINT curIdx = (UINT)GetTarget()->Collider3D()->GetType();

	const char* ColTypes[4] = { "NONE", "Player", "Wall", "Monster" };


	if (ImGui::BeginCombo("Select", ColTypes[curIdx]))
	{
		for (int idx = 0; idx < IM_ARRAYSIZE(ColTypes); idx++)
		{
			const bool is_selected = (curIdx == idx);
			if (ImGui::Selectable(ColTypes[idx], is_selected))
			{
				curIdx = idx;
				GetTarget()->Collider3D()->SetType((COLLIDER3D_TYPE)curIdx);
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	return 0;
}
