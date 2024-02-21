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

	// 하나의 버텍스버퍼에 여러개의 인덱스버퍼가 연결
	vector<tIndexInfo>		m_vecIdxInfo;

	// mtrl 정보
	UINT					m_mtrlCount;

	// Animation 3D 정보
	vector<tMTAnimClip>		m_vecAnimClip;		// 기본 정보
	vector<tMTBone>			m_pVecBones;		// 기본 정보

	CStructuredBuffer*		m_pBoneFrameData;	// 전체 폰 프레임의 행렬 정보 | m_pVecBones 로부터 추출함
	CStructuredBuffer*		m_pBoneOffset;		// 1행짜리 각 뼈의 offset 행렬 | m_pVecBones 로부터 추출함

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