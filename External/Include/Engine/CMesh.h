#pragma once
#include "CRes.h"
#include "CFBXLoader.h"

class CStructuredBuffer;
struct tIndexInfo
{
	ComPtr<ID3D11Buffer>    pIB;
	D3D11_BUFFER_DESC       tIBDesc;
	UINT				    iIdxCount;
	void*					pIdxSysMem;
};

class CMesh : public CRes
{
private:
	ComPtr<ID3D11Buffer>	m_VB;
	D3D11_BUFFER_DESC		m_tVBDesc;
	UINT					m_VtxCount;
	void*					m_pVtxSys;

	// �ϳ��� ���ؽ����ۿ� �������� �ε������۰� ����
	vector<tIndexInfo>		m_vecIdxInfo;

	// mtrl ����
	UINT					m_mtrlCount;

	// Animation 3D ����
	vector<tMTAnimClip>		m_vecAnimClip;		// �⺻ ����
	vector<tMTBone>			m_pVecBones;		// �⺻ ����

	CStructuredBuffer*		m_pBoneFrameData;	// ��ü �� �������� ��� ���� | m_pVecBones �κ��� ������
	CStructuredBuffer*		m_pBoneOffset;		// 1��¥�� �� ���� offset ��� | m_pVecBones �κ��� ������

public:
	Vtx* GetVtxSysMem() { return (Vtx*)m_pVtxSys; }
	UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }

	vector<tMTBone> GetMTBones() { return m_pVecBones; }
	UINT GetMTBoneCount() { return (UINT)m_pVecBones.size(); }
	vector<tMTAnimClip> GetMTAnimClips() { return m_vecAnimClip; }
	bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

	CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; }
	CStructuredBuffer* GetBoneOffsetBuffer() { return m_pBoneOffset; }

public:
	void SetMtrlCount(UINT _count) { m_mtrlCount = _count; }

public:
	static CMesh* CreateFromContainer(CFBXLoader& _loader);
	void Create(void* _VtxSysMem, UINT _iVtxCount, void* _IdxSysMem, UINT _IdxCount);

private:
	virtual int Load(const wstring& _strFilePath);
public:
	virtual int Save(const wstring& _strRelativePath);

	void render(UINT _iSubset);
	void render_particle(UINT _iParticleCount);
	void render_instancing(UINT _isubset);

private:
	void UpdateData(UINT _iSubset);
	void UpdateData_Inst(UINT _iSubset);

public:
	CMesh(bool _bEngine = false);
	~CMesh();
};