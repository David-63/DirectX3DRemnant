#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
    public CRes
{
private:
	CGameObject*	m_ProtoObj;

public:
	CGameObject* Instantiate();

public:
	virtual int Load(const wstring& _strFilePath);
	virtual int Save(const wstring& _strRelativePath);

private:
	virtual void UpdateData() override {}

public:
	void RegisterProtoObject(CGameObject* _Proto);

public:
	CPrefab();
	~CPrefab();
};

