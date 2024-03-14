#pragma once
#include <Engine/CScript.h>

class CP_MouseCtrlScript : public CScript
{
private:

public:
	virtual void tick() override;


public:
	CLONE(CP_MouseCtrlScript);

public:
	CP_MouseCtrlScript();
	virtual ~CP_MouseCtrlScript();
};

