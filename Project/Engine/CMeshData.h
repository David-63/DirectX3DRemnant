#pragma once
#include "CRes.h"

#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"

class CMeshData : public CRes
{
	Ptr<CMesh>				m_pMesh;
	vector<Ptr<CMaterial>>	m_vecMtrl;

public:
	Ptr<CMesh> GetMeshFromData() { return m_pMesh; }
	vector<Ptr<CMaterial>> GetMtrlFromData() { return m_vecMtrl; }

public:
	static CMeshData* LoadFromFBX(const wstring& _strFilePath);

	virtual int Save(const wstring& _strFilePath) override;
	virtual int Load(const wstring& _strFilePath) override;

	CGameObject* Instantiate();
	CGameObject* InstMesh();		// Animator를 생성을 강제로 막는 버전

	CLONE_DISABLE(CMeshData)
public:
	CMeshData(bool _bEngine = false);
	virtual ~CMeshData();
};