#include "pch.h"
#include "CameraUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CCamera.h>

CameraUI::CameraUI()
    : ComponentUI("##CameraUI", COMPONENT_TYPE::CAMERA)
{
    SetName("CameraUI");
}

CameraUI::~CameraUI()
{
}

int CameraUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

    float fFov = GetTarget()->Camera()->GetFov();

    ImGui::Text("Fov Value");
    ImGui::SameLine();
    ImGui::SliderFloat("##Camera Fov", &fFov, XM_PIDIV4, XM_PIDIV2);

    GetTarget()->Camera()->SetFov(fFov);

    return TRUE;
}
