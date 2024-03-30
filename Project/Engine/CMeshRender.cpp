#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)		
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
}

void CMeshRender::render()
{	
	if (m_bRenderOff)
		return;

	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
		return;

	// Transform �� UpdateData ��û
	Transform()->UpdateData();

	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}
	
	if (Animator3D() && nullptr != Animator3D()->GetCurAnim())
	{
		Animator3D()->UpdateData();

		for (UINT idx = 0; idx < GetMtrlCount(); ++idx)
		{
			if (nullptr == GetMaterial(idx))
				continue;

			GetMaterial(idx)->SetAnim3D(true);
		}
	}

	UINT iSubsetCount = GetMesh()->GetSubsetCount();

	for (size_t idx = 0; idx < iSubsetCount; ++idx)
	{
		if (nullptr != GetMaterial(idx))
		{
			// ���� ������Ʈ
			GetMaterial(idx)->UpdateData();

			// ����
			GetMesh()->render(idx);
		}
	}

	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();
	if (Animator3D())
		Animator3D()->ClearData();
}

void CMeshRender::render(UINT _iSubset)
{
	if (m_bRenderOff)
		return;

	if (nullptr == GetMesh() || nullptr == GetMaterial(_iSubset))
		return;

	// Transform �� UpdateData ��û
	Transform()->UpdateData();

	// Animator2D ������Ʈ�� �ִٸ�
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// Animator3D ������Ʈ
	if (Animator3D())
	{
		Animator3D()->UpdateData();
		GetMaterial(_iSubset)->SetAnim3D(true); // Animation Mesh �˸���
		//GetMaterial(_iSubset)->SetBoneCount(Animator3D()->GetMTBoneCount());
	}

	// ����� ���� ������Ʈ
	GetMaterial(_iSubset)->UpdateData();

	// ����� �޽� ������Ʈ �� ������
	GetMesh()->render(_iSubset);

	// Animation ���� ���� ����
	if (Animator2D())
		Animator2D()->Clear();

	if (Animator3D())
		Animator3D()->ClearData();
}

void CMeshRender::SetMeshData(Ptr<CMeshData> _meshData)
{
	m_meshData = _meshData;
	SetMesh(m_meshData->GetMeshFromData());

	vector<Ptr<CMaterial>> pMtrl = m_meshData->GetMtrlFromData();
	for (UINT i = 0; i < pMtrl.size(); ++i)
	{
		SetMaterial(pMtrl[i], i);
	}
}

void CMeshRender::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);
	SaveResRef(m_meshData.Get(), _File);
	fwrite(&m_bRenderOff, 1, 1, _File);
}

void CMeshRender::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);
	LoadResRef(m_meshData, _File);
	fread(&m_bRenderOff, 1, 1, _File);
}
