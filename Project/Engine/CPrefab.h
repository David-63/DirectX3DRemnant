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
	int SaveGameObject(CGameObject* _Object, const wstring& _strRelativePath);

private:
	void SaveGameObject(CGameObject* _Object, FILE* _File);
	CGameObject* LoadGameObject(FILE* _File);

public:
	virtual int Save(const wstring& _strRelativePath);
	virtual int Load(const wstring& _strFilePath);

private:
	virtual void UpdateData() override {}

public:
	void RegisterProtoObject(CGameObject* _Proto);
	CGameObject* GetProtoObj() { return m_ProtoObj; }


public:
	CPrefab();
	~CPrefab();
};

