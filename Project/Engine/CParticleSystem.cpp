#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"
#include <random>


CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_RWBuffer(nullptr)
	, m_ModuleData{}
	, m_AccTime(0.f)
	, m_ParticleTex(nullptr)
	, m_NoiseTex(nullptr)
	, m_bOnceExcute(false)
	, m_bBursts(false)
	, m_SpawnTime(5.0f)
	, m_bLoop(false)
	, m_bUseSpark(false)
	, m_pMesh(nullptr)
	, m_pMtrl(nullptr)
	, m_IsFoward(true)
	, m_iSpawnCount(0)
	, m_bWantExcute(true)
	, m_UseTimeSpawn(false)
	, m_bFinish(false)
	, m_BackUpModuleData{}
	, m_UseOnceSpawn(false)

{



	SetFrustumCheck(false);
	m_ModuleData.iMaxParticleCount = 1000;

	m_ModuleData.bRandomPos = false;
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = false;
	m_ModuleData.SpawnRate = 2;
	m_ModuleData.vSpawnColor = Vec4(0.4f, 1.f, 1.f, 0.8f);
	m_ModuleData.vSpawnScaleMin = Vec3(15.f, 15.f, 1.f);
	m_ModuleData.vSpawnScaleMax = Vec3(20.f, 20.f, 1.f);

	m_ModuleData.SpawnShapeType = 0;
	m_ModuleData.vBoxShapeScale = Vec3(200.f, 200.f, 200.f);
	m_ModuleData.vSpawnAreaOffsetFactor = Vec3(0.f, 0.f, 0.f);
	m_ModuleData.Space = 0; // �ùķ��̼� ��ǥ��

	m_ModuleData.MinLifeTime = 3.f;
	m_ModuleData.MaxLifeTime = 5.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
	m_ModuleData.StartScale = 1.5f;
	m_ModuleData.EndScale = 0.2f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = false;
	m_ModuleData.vStartColor = Vec4(0.2f, 0.3f, 1.0f, 1.0f);
	m_ModuleData.vEndColor = Vec4(0.4f, 1.f, 0.4f, 1.0f);


	// �Ʒ� �ɼ��� �ƿ� ���� �ȵ�. 
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_ModuleData.AddVelocityType = 0; // From Center
	m_ModuleData.Speed = 300.f;
	m_ModuleData.vVelocityDir = Vec3(0.f, 1.f, 0.f);
	m_ModuleData.OffsetAngle;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
	m_ModuleData.StartDrag = 500.f;
	m_ModuleData.EndDrag = -500.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_ModuleData.fNoiseTerm = 1.f;
	m_ModuleData.fNoiseForce = 100.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_ModuleData.VelocityAlignment = true;
	m_ModuleData.VelocityScale = false;
	m_ModuleData.vMaxVelocityScale = Vec3(15.f, 1.f, 1.f);
	m_ModuleData.vMaxSpeed = 500.f;
	m_ModuleData.AnimLoop = false;
	m_ModuleData.AnimUse = false;
	m_ModuleData.fAnimSpeed = 1.f;
	m_ModuleData.fAnimFrmTime = 0.1f;
	m_ModuleData.iAnimXCount = 1;
	m_ModuleData.iAnimYCount = 1;

	m_ModuleData.bStrongColor = false;
	m_ModuleData.bDead = false;

	m_BackUpModuleData = m_ModuleData;



	// ���� �޽�
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	m_pMesh = GetMesh();

	// ��ƼŬ ���� ����
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"), 0);
	m_pMtrl = GetMaterial(0);

	// ��ƼŬ ������Ʈ ��ǻƮ ���̴�	
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();

	// ��ƼŬ ����
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);


	// ��ƼŬ ���� ���� ���޿� ����
	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);


	// ��� Ȱ��ȭ �� ��� ���� ���� ����
	m_ModuleDataBuffer = new CStructuredBuffer;
	m_ModuleDataBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);


	m_ParticleTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\AlphaCircle.png");
	m_NoiseTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\noise\\noise_01.png");



}

CParticleSystem::CParticleSystem(const CParticleSystem& _other)
	: CRenderComponent(_other)
	, m_ParticleBuffer(nullptr)
	, m_RWBuffer(nullptr)
	, m_ModuleDataBuffer(nullptr)
	, m_ModuleData(_other.m_ModuleData) // excute�ϱ� ���� ���� ���ۿ� ������ �뵵�̹Ƿ� ���� ������
	, m_AccTime(_other.m_AccTime)
	, m_ParticleTex(_other.m_ParticleTex)
	, m_NoiseTex(_other.m_NoiseTex)
	, m_bOnceExcute(_other.m_bOnceExcute)
	, m_bBursts(_other.m_bBursts)
	, m_SpawnTime(_other.m_SpawnTime)
	, m_bLoop(_other.m_bLoop)
	, m_bUseSpark(_other.m_bUseSpark)
	, m_IsFoward(_other.m_IsFoward)
	, m_iSpawnCount(0)
	, m_UseTimeSpawn(false)
	, m_bFinish(false)
	, m_bWantExcute(true)
	, m_UseOnceSpawn(false)
{

	SetFrustumCheck(false);
	m_ModuleData.iMaxParticleCount = _other.m_ModuleData.iMaxParticleCount;
	m_ModuleData.bRandomPos = _other.m_ModuleData.bRandomPos;


	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = _other.m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN];
	m_ModuleData.SpawnRate = _other.m_ModuleData.SpawnRate;
	m_ModuleData.vSpawnColor = _other.m_ModuleData.vSpawnColor;
	m_ModuleData.vSpawnScaleMin = _other.m_ModuleData.vSpawnScaleMin;
	m_ModuleData.vSpawnScaleMax = _other.m_ModuleData.vSpawnScaleMax;

	m_ModuleData.SpawnShapeType = _other.m_ModuleData.SpawnShapeType;
	m_ModuleData.vBoxShapeScale = _other.m_ModuleData.vBoxShapeScale;
	m_ModuleData.vSpawnAreaOffsetFactor = _other.m_ModuleData.vSpawnAreaOffsetFactor;
	m_ModuleData.Space = _other.m_ModuleData.Space; // �ùķ��̼� ��ǥ��

	m_ModuleData.MinLifeTime = _other.m_ModuleData.MinLifeTime;
	m_ModuleData.MaxLifeTime = _other.m_ModuleData.MaxLifeTime;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = _other.m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE];
	m_ModuleData.StartScale = _other.m_ModuleData.StartScale;
	m_ModuleData.EndScale = _other.m_ModuleData.EndScale;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = _other.m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE];
	m_ModuleData.vStartColor = _other.m_ModuleData.vStartColor;
	m_ModuleData.vEndColor = _other.m_ModuleData.vEndColor;


	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true; // �ƿ� ���� �ȵǴ� �ɼ��̶� true�� ����
	m_ModuleData.AddVelocityType = _other.m_ModuleData.AddVelocityType; // From Center
	m_ModuleData.Speed = _other.m_ModuleData.Speed;
	m_ModuleData.vVelocityDir = _other.m_ModuleData.vVelocityDir;
	m_ModuleData.OffsetAngle = _other.m_ModuleData.OffsetAngle;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = _other.m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG];
	m_ModuleData.StartDrag = _other.m_ModuleData.StartDrag;
	m_ModuleData.EndDrag = _other.m_ModuleData.EndDrag;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = _other.m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE];
	m_ModuleData.fNoiseTerm = _other.m_ModuleData.fNoiseTerm;
	m_ModuleData.fNoiseForce = _other.m_ModuleData.fNoiseForce;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = _other.m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER];
	m_ModuleData.VelocityAlignment = _other.m_ModuleData.VelocityAlignment;
	m_ModuleData.VelocityScale = _other.m_ModuleData.VelocityScale;
	m_ModuleData.vMaxVelocityScale = _other.m_ModuleData.vMaxVelocityScale;
	m_ModuleData.vMaxSpeed = _other.m_ModuleData.vMaxSpeed;
	m_ModuleData.AnimLoop = _other.m_ModuleData.AnimLoop;
	m_ModuleData.AnimUse = _other.m_ModuleData.AnimUse;
	m_ModuleData.fAnimSpeed = _other.m_ModuleData.fAnimSpeed;
	m_ModuleData.fAnimFrmTime = _other.m_ModuleData.fAnimFrmTime;
	m_ModuleData.iAnimXCount = _other.m_ModuleData.iAnimXCount;
	m_ModuleData.iAnimYCount = _other.m_ModuleData.iAnimYCount;

	m_ModuleData.bStrongColor = _other.m_ModuleData.bStrongColor;
	m_ModuleData.bDead = false;


	m_BackUpModuleData = _other.m_ModuleData;

	m_bWantExcute = _other.m_bWantExcute;

	// ===============
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	// == ���� �з�
	if (m_IsFoward)
		SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"), 0);

	else
		SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"D_ParticleRenderMtrl"), 0);



	// ��ƼŬ ������Ʈ ��ǻƮ���̴�
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();



	// ==============
	// ��ƼŬ ����
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);


	// ��ƼŬ ���� ���� ���޿� ����
	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

	// ��� Ȱ��ȭ �� ��� ���� ���� ����
	m_ModuleDataBuffer = new CStructuredBuffer;
	m_ModuleDataBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);

	m_ParticleTex = CResMgr::GetInst()->FindRes<CTexture>(_other.m_strTexName);
	m_NoiseTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\noise\\noise_01.png");



}


CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
	{
		delete m_ParticleBuffer;
		m_ParticleBuffer = nullptr;
	}


	if (nullptr != m_RWBuffer)
	{
		delete m_RWBuffer;
		m_RWBuffer = nullptr;
	}

	if (nullptr != m_ModuleDataBuffer)
	{
		delete m_ModuleDataBuffer;
		m_ModuleDataBuffer = nullptr;
	}

}


void CParticleSystem::finaltick()
{



	// =================== Spawn Rate ===================
	

	if (m_bWantExcute)
	{
		if (m_UseOnceSpawn)
		{
			if (m_bWantExcute)
				m_bWantExcute = false;
		}
		if (m_bBursts)
		{

			tRWParticleBuffer rwbuffer = { m_ModuleData.iMaxParticleCount, };
			m_RWBuffer->SetData(&rwbuffer);
			m_ModuleData.SpawnRate = 0;
		}
		else
		{
			float fTimePerCount = 1.f / (float)m_ModuleData.SpawnRate;
			m_AccTime += DT;

			if (fTimePerCount < m_AccTime)
			{
				float fData = m_AccTime / fTimePerCount;
				m_AccTime = fTimePerCount * (fData - floor(fData));

				tRWParticleBuffer rwbuffer = { (int)fData, };
				m_RWBuffer->SetData(&rwbuffer);
			}
		}
	}
	
	// ���� ���� (Active) SetExcute
	// m_bWantExcute �� ���� �⺻������ false
	

	///
	//if (!m_bOnceExcute)
	//{
	//	// ���� ����
	//	if (++m_iSpawnCount >= 1 && m_bWantExcute)
	//	{
	//		m_bOnceExcute = true;
	//		m_iSpawnCount = 0;
	//	}

	//	if (m_bBursts)
	//	{

	//		// ���ۿ� ���� ī��Ʈ ����
	//		tRWParticleBuffer rwbuffer = { m_ModuleData.iMaxParticleCount, };
	//		m_RWBuffer->SetData(&rwbuffer);
	//		m_ModuleData.SpawnRate = 0;

	//	}

	//	else
	//	{

	//		// ���� ����Ʈ ���
	//		// 1�� ���� �ð�
	//		float fTimePerCount = 1.f / (float)m_ModuleData.SpawnRate;
	//		m_AccTime += DT;

	//		// �����ð��� ���� �����ð��� �Ѿ��
	//		if (fTimePerCount < m_AccTime)
	//		{
	//			// �ʰ� ���� ==> ���� ����
	//			float fData = m_AccTime / fTimePerCount;

	//			// �������� ���� �ð�
	//			m_AccTime = fTimePerCount * (fData - floor(fData));

	//			// ���ۿ� ���� ī��Ʈ ����
	//			tRWParticleBuffer rwbuffer = { (int)fData, };
	//			m_RWBuffer->SetData(&rwbuffer);
	//		}
	//	}

	//	
	//	// ���� ��ư ������ֱ� ����ī��Ʈ 0���� ���ִ�....


	//}

	//if (!m_bWantExcute)
	//{
	//	m_iSpawnCount = 0;
	//	m_bOnceExcute = false;
	//}

	//else if (m_bWantExcute)
	//	{
	//		m_iSpawnCount++;
	//	}

	// ��ƼŬ ������Ʈ ��ǻƮ ���̴�
	float originalSpawnTime = m_SpawnTime;

	if (m_UseTimeSpawn)
	{
		// ������ m_SpawnTime ��ġ�� �ִٸ�
		if (m_SpawnTime > 0.0f)
		{
			m_SpawnTime -= DT;
		}

		else if (m_SpawnTime <= 0.0f)
		{
			m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = false;
			m_SpawnTime = 0.0f;
			m_bFinish = true;
		}
	}

	

	m_ModuleDataBuffer->SetData(&m_ModuleData);

	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetRWParticleBuffer(m_RWBuffer);
	m_UpdateCS->SetModuleData(m_ModuleDataBuffer);

	if (nullptr != m_NoiseTex)
	{
		m_UpdateCS->SetNoiseTexture(m_NoiseTex);
	}

	m_UpdateCS->SetParticleObjectPos(Transform()->GetWorldPos());
	m_UpdateCS->SetParticleObjectDir(Transform()->GetWorldDir(DIR_TYPE::FRONT));

	m_UpdateCS->Execute();
}

void CParticleSystem::render()
{

	// View ��İ� ���� ����� ������Ʈ �ϱ� ���� Ʈ������ ���ε�
	Transform()->UpdateData();

	// ����ȭ ���۷� ���� �����͸� t20�� ���ε�

	m_ParticleBuffer->UpdateData(20, PIPELINE_STAGE::PS_ALL);


	// ��� ������ t21 �� ���ε�
	m_ModuleDataBuffer->UpdateData(21, PIPELINE_STAGE::PS_ALL);



	// ��ƼŬ ���� ���̴� ������Ʈ
	GetMaterial(0)->SetTexParam(TEX_0, m_ParticleTex);

	GetMaterial(0)->UpdateData();


	// �ν��Ͻ����� �ϳ��� ���������ο��� ��ƼŬ �ִ� ������ŭ ������ ����
	GetMesh()->render_particle(m_ModuleData.iMaxParticleCount);




	m_ParticleBuffer->Clear();
	m_ModuleDataBuffer->Clear();
	m_RWBuffer->Clear();	


}

void CParticleSystem::render(UINT _iSubset) // �̰� �������̵��� �Լ��� ����� �غ��� �ҿ� ����. 
{

	render();


}

void CParticleSystem::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);

	SaveWString(GetName(), _File);
	SaveWString(GetTexName(), _File);

	fwrite(&m_ModuleData, sizeof(tParticleModule), 1, _File);
	fwrite(&m_bOnceExcute, sizeof(bool), 1, _File);
	fwrite(&m_bBursts, sizeof(bool), 1, _File);
	fwrite(&m_SpawnTime, sizeof(float), 1, _File);
	fwrite(&m_bLoop, sizeof(bool), 1, _File);
	fwrite(&m_bUseSpark, sizeof(bool), 1, _File);
	fwrite(&m_IsFoward, sizeof(bool), 1, _File);
	fwrite(&m_bWantExcute, sizeof(bool), 1, _File);
	fwrite(&m_iSpawnCount, sizeof(int), 1, _File);

	SaveResRef(m_UpdateCS.Get(), _File);
	SaveResRef(m_ParticleTex.Get(), _File);
	SaveResRef(m_NoiseTex.Get(), _File);


}

void CParticleSystem::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);

	wstring name;
	LoadWString(name, _File);
	SetName(name);

	LoadWString(m_strTexName, _File);


	fread(&m_ModuleData, sizeof(tParticleModule), 1, _File);
	fread(&m_bOnceExcute, sizeof(bool), 1, _File);
	fread(&m_bBursts, sizeof(bool), 1, _File);
	fread(&m_SpawnTime, sizeof(float), 1, _File);
	fread(&m_bLoop, sizeof(bool), 1, _File);
	fread(&m_bUseSpark, sizeof(bool), 1, _File);
	fread(&m_IsFoward, sizeof(bool), 1, _File);
	fread(&m_bWantExcute, sizeof(bool), 1, _File);
	fread(&m_iSpawnCount, sizeof(int), 1, _File);

	int i = 0;
	fread(&i, sizeof(i), 1, _File);

	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get();
	}

	LoadResRef(m_ParticleTex, _File);
	LoadResRef(m_NoiseTex, _File);


}

CGameObject* CParticleSystem::LoadSetting(CGameObject* Obj)
{
	CGameObject* pObject = Obj;

	pObject->ParticleSystem()->m_ModuleData = m_ModuleData;


	return pObject;

}

//Vec3 CParticleSystem::CreateRandomDirection(Vector3 normal, float coneAngle)
//{
//	// ȸ�� �� ���� (���⼭�� Y���� �������� ȸ���Ѵٰ� ����)
//	Vector3 rotationAxis = normal.Cross(Vector3(0, 1, 0)).Normalize();
//
//	// ���� ���� ����
//	float angle = RandomFloat(-coneAngle / 2, coneAngle / 2);
//
//	// ȸ�� ��� ����
//	Matrix rotationMatrix = Matrix::CreateFromAxisAngle(rotationAxis, angle);
//
//	// �븻 ���� ȸ��
//	Vector3 randomDirection = Vector3::Transform(normal, rotationMatrix);
//
//	return randomDirection;
//}
//
//Vec3 CParticleSystem::CreateRandomDirection(const Vector3& normal, float openAngle)
//{
//	openAngle = XMConvertToRadians(openAngle);
//
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	std::uniform_real_distribution<float> dis(0, 1);
//
//	// ������ �߽� ���� ���Ϳ� ���� ����ǥ������ ���� ���� ����
//	float theta = dis(gen) * DirectX::XM_2PI; // 0���� 2PI ����
//	float phi = dis(gen) * openAngle; // 0���� coneHalfAngleInRadians ����
//
//	// ����ǥ�� ī�׽þ� ��ǥ�� ��ȯ
//	float x = sinf(phi) * cosf(theta);
//	float y = sinf(phi) * sinf(theta);
//	float z = cosf(phi);
//
//	// ���� ���� ���� ����
//	Vector3 randomDir(x, y, z);
//
//	// ������ �߽� ���� ���͸� �������� ���� ���� ���͸� ȸ��
//	Vector3 axis = normal.Cross(Vector3(1, 0, 0)).Normalize();
//	if (axis.LengthSquared() == 0.0f) {
//		// coneDir�� �̹� Z��� ������ ���, Y���� ȸ�� ������ ���
//		axis = normal.Cross(Vector3::UnitY).Normalize();
//	}
//	float angle = acos(normal.Dot(Vector3(0, 1, 0)));
//	Matrix rotationMatrix = Matrix::CreateFromAxisAngle(axis, angle);
//
//	return Vector3::Transform(randomDir, rotationMatrix);
//} // �� �������� ������ �� �ۿ� �ȵ� 



Vec3 CParticleSystem::CreateRandomDirection(const Vector3& normal, float openAngle)
{
	openAngle = XMConvertToRadians(openAngle);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0, 1);

	// ������ �߽� ���� ���Ϳ� ���� ����ǥ������ ���� ���� ����
	float theta = dis(gen) * DirectX::XM_2PI; // 0���� 2PI ����
	float phi = dis(gen) * openAngle; // 0���� coneHalfAngleInRadians ����

	// ����ǥ�� ī�׽þ� ��ǥ�� ��ȯ
	float x = sinf(phi) * cosf(theta);
	float y = sinf(phi) * sinf(theta);
	float z = cosf(phi);

	// ���� ���� ���� ����
	Vector3 randomDir(x, y, z);

	// ������ �߽� ���� ���͸� �������� ���� ���� ���͸� ȸ��
	Vector3 axis = normal.Cross(Vector3(1, 0, 0)).Normalize();

	//if (axis.LengthSquared() == 0.0f) {
	//	// coneDir�� �̹� Z��� ������ ���, Y���� ȸ�� ������ ���
	//	axis = normal.Cross(Vector3(0, 1, 0).Normalize());
	//}
	float angle = acos(normal.Dot(Vector3(0, 1, 0)));
	Matrix rotationMatrix = Matrix::CreateFromAxisAngle(axis, angle);

	return Vector3::Transform(randomDir, rotationMatrix);
}

float CParticleSystem::RandomFloat(float min, float max)
{
	// ���� ���� ���� �ʱ�ȭ
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);

	return dis(gen);
}




//Vec3 CParticleSystem::CreateRandomDirection(const Vector3& normal, const Vector3& up, const Vector3& Front) // ������� ���������� �� 
//{
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	std::uniform_real_distribution<> dis(0, DirectX::XM_2PI); // 0���� 2PI ������ ���� ��
//
//	// ���� ���� ����
//	float angle = dis(gen);
//
//	// ���� ���Ϳ� up ���͸� �������� ȸ�� ��� ����
//	Matrix rotMatrix = Matrix::CreateFromAxisAngle(normal, angle);
//
//	// ȸ�� ����� ����Ͽ� ���� ���� ����
//	Vector3 randomDir = Vector3::Transform(up, rotMatrix);
//
//	return randomDir;
//}

void CParticleSystem::SetParticleBufferSize()
{
	if (m_ParticleBuffer->GetElementCount() < m_ModuleData.iMaxParticleCount)
	{
		m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);
	}
}
