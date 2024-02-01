#include "pch.h"
#include "LandScapeUI.h"

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


	return TRUE;
}
