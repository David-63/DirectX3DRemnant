#pragma once
#include "UI.h"


class MenuUI :
    public UI
{
private:


public:
    virtual void finaltick() override;
    virtual int render_update() override;


private:
    void CreateEmptyObject();
    void CreateEmptyMaterial();
    void AddComponent(COMPONENT_TYPE _type);
    void AddScript(const wstring& _strScriptName);

    void AddScalarParam(int _ParamNum, string _ParamName);
    void DeleteScalarParam(int _ParamNum, string _ParamName);

public:
    MenuUI();
    ~MenuUI();
};

