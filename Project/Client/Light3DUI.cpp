#include "pch.h"
#include "Light3DUI.h"
#include <Engine\CGameObject.h>
#include <Engine/CLight3D.h>

Light3DUI::Light3DUI() : ComponentUI("##Light3D", COMPONENT_TYPE::LIGHT3D)
{
	SetName("Light3D");
}

Light3DUI::~Light3DUI()
{
}

int Light3DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	int eLightType = static_cast<int>(GetTarget()->Light3D()->GetLightType());
	Vec3 vColor = GetTarget()->Light3D()->GetLightColor();
	Vec3 vAmbient = GetTarget()->Light3D()->GetLightAmbient();
	float fRadius = GetTarget()->Light3D()->GetRadius();
	bool bShow = GetTarget()->Light3D()->GetDebugRendering();

	ImGui::Text("0: Dir, 1: Point, 2: Spot");
	ImGui::Text("LightType");
	ImGui::SameLine();
	ImGui::SliderInt("##Light Type", &eLightType, 0, static_cast<int>(LIGHT_TYPE::End) - 2);

	ImGui::Text("Radius	");
	ImGui::SameLine();
	ImGui::DragFloat("##Light Radius", &fRadius);

	ImGui::Text("Color	");
	ImGui::SameLine();
	ImGui::SliderFloat3("##Light Color", vColor, 0.f, 1.f);

	ImGui::Text("Ambient");
	ImGui::SameLine();
	ImGui::SliderFloat3("##Light Ambient", vAmbient, 0.f, 1.f);

	ImGui::Text("DebugRender");
	ImGui::SameLine();
	ImGui::Checkbox("##Light DebugRender", &bShow);

	if (static_cast<int>(LIGHT_TYPE::End) <= eLightType)
		eLightType = static_cast<int>(LIGHT_TYPE::End) - 1;
	if (0 >= eLightType)
		eLightType = static_cast<int>(LIGHT_TYPE::DIRECTIONAL);

	GetTarget()->Light3D()->SetLightType(static_cast<LIGHT_TYPE>(eLightType));
	GetTarget()->Light3D()->SetRadius(fRadius);
	GetTarget()->Light3D()->SetLightColor(vColor);
	GetTarget()->Light3D()->SetLightAmbient(vAmbient);
	GetTarget()->Light3D()->SetDebugRendering(bShow);


	return TRUE;
}
