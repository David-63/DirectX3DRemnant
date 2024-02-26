#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
    public CRes
{
private:
	CGameObject*	m_ProtoObj;

public:
	CGameObject* Instantiate(Vec3 _pos, int _layerIdx);

private:
	void SaveGameObject(CGameObject* _Object, FILE* _File);
	CGameObject* LoadGameObject(FILE* _File);

public:
	virtual int Save(const wstring& _strRelativePath);
	virtual int Load(const wstring& _strFilePath);
	int LoadOnUI(const wstring& _strRelativePath);

private:
	virtual void UpdateData() override {}

public:
	void RegisterProtoObject(CGameObject* _Proto);

public:
	CPrefab();
	~CPrefab();
};

