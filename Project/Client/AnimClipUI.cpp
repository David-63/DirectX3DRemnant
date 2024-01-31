#include "pch.h"
#include "AnimClipUI.h"


AnimClipUI::AnimClipUI()
	: ResUI(RES_TYPE::ANIMCLIP)
{
	SetName("AnimClip");
}

AnimClipUI::~AnimClipUI()
{
}


int AnimClipUI::render_update()
{
	ResUI::render_update();

	return 0;
}
