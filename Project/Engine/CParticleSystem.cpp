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
	, m_SpawnTime(0.0f)
	, m_bLoop(false)


{



	SetFrustumCheck(false);
	m_ModuleData.iMaxParticleCount = 1000;


	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = false;
	m_ModuleData.SpawnRate = 2;
	m_ModuleData.vSpawnColor = Vec4(0.4f, 1.f, 1.f, 0.8f);
	m_ModuleData.vSpawnScaleMin = Vec3(15.f, 15.f, 1.f);
	m_ModuleData.vSpawnScaleMax = Vec3(20.f, 20.f, 1.f);

	m_ModuleData.SpawnShapeType = 0;
	m_ModuleData.vBoxShapeScale = Vec3(200.f, 200.f, 200.f);	
	m_ModuleData.fSpawnAreaOffsetFactor = 0.0f;
	m_ModuleData.Space = 0; // �ùķ��̼� ��ǥ��

	m_ModuleData.MinLifeTime = 3.f;
	m_ModuleData.MaxLifeTime = 5.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
	m_ModuleData.StartScale = 1.5f;
	m_ModuleData.EndScale = 0.2f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = false;
	m_ModuleData.vStartColor = Vec4(0.2f, 0.3f, 1.0f, 1.0f);
	m_ModuleData.vEndColor = Vec4(0.4f, 1.f, 0.4f, 1.0f);

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
	m_ModuleData.VelocityScale = true;
	m_ModuleData.vMaxVelocityScale = Vec3(15.f, 1.f, 1.f);
	m_ModuleData.vMaxSpeed = 500.f;

	m_ModuleData.bStrongColor = false;



	

	// ���� �޽�
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	// ��ƼŬ ���� ����
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"), 0);

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


	m_ParticleTex = CResMgr::GetInst()->Load<CTexture>(L"Particle_0", L"texture\\particle\\HardRain.png");
	m_NoiseTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\noise\\noise_01.png");



}

CParticleSystem::CParticleSystem(const CParticleSystem& _other)
	: CRenderComponent(_other)
	, m_ParticleBuffer(nullptr)
	, m_RWBuffer(nullptr)
	, m_ModuleDataBuffer(nullptr)
	, m_ModuleData(_other.m_ModuleData) // excute�ϱ� ���� ���� ���ۿ� ������ �뵵�̹Ƿ� ���� ������
	, m_AccTime(0.f)
{

	SetFrustumCheck(false);
	m_ModuleData.iMaxParticleCount = 1000;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = false;
	m_ModuleData.SpawnRate = 2;
	m_ModuleData.vSpawnColor = Vec4(0.4f, 1.f, 1.f, 0.8f);
	m_ModuleData.vSpawnScaleMin = Vec3(15.f, 15.f, 1.f);
	m_ModuleData.vSpawnScaleMax = Vec3(20.f, 20.f, 1.f);

	m_ModuleData.SpawnShapeType = 0;
	m_ModuleData.vBoxShapeScale = Vec3(200.f, 200.f, 200.f);
	m_ModuleData.fSpawnAreaOffsetFactor = 0.0f;
	m_ModuleData.Space = 0; // �ùķ��̼� ��ǥ��

	m_ModuleData.MinLifeTime = 3.f;
	m_ModuleData.MaxLifeTime = 5.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
	m_ModuleData.StartScale = 1.5f;
	m_ModuleData.EndScale = 0.2f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
	m_ModuleData.vStartColor = Vec4(0.2f, 0.3f, 1.0f, 1.f);
	m_ModuleData.vEndColor = Vec4(0.4f, 1.f, 0.4f, 1.f);


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
	m_ModuleData.VelocityScale = true;
	m_ModuleData.vMaxVelocityScale = Vec3(15.f, 1.f, 1.f);
	m_ModuleData.vMaxSpeed = 500.f;

	m_ModuleData.bStrongColor = false;
	// ===============
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	// == ������ ���� �����ؾ����� �ƴϸ� ���� ������� �����غ��� �Ʒ� ��ġ��
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"), 0);



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

	m_ParticleTex = CResMgr::GetInst()->Load<CTexture>(L"Particle_0", L"texture\\particle\\HardRain.png");
	m_NoiseTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\noise\\noise_01.png");



}


CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_RWBuffer)
		delete m_RWBuffer;

	if (nullptr != m_ModuleDataBuffer)
		delete m_ModuleDataBuffer;
}


void CParticleSystem::finaltick()
{
	//// ���̳븻, ź��Ʈ, �븻 
	//Vector3 normal = Vector3(-1.f, 0.f, 0.f); // ���� ���� ����
	//Vector3 up = Vector3(0.f, 1.f, 0.f); // up ����
	//Vector3 right = Vector3(1.f, 0.f, 0.f); // right ����

	m_ModuleData.vRandomSpark = CreateRandomDirection(Vec3(0.f, 0.f, 1.f), 45.f);


	//m_ModuleData.vRandomSpark(CreateRandomDirection


	// ===== ť�� �޽� �븻���� 
	/*  ���� 0, 1, 0
		�Ʒ��� 0, -1, 0
		���� - 1, 0, 0
		������ 1, 0, 0

		�ո� 0, 0, -1
		�޸� 0, 0, 1 (�޸��� ���� �ϼ�) */

	
	//m_ModuleData.vRandomSpark = CreateRandomDirection(Vec3(0.f, 0.f, 1.f), 10.f);

	// =================== Spawn Rate ===================
	// ���ڰ� �� �̻� �ȳ����� �Ϸ��� ���⼭ �Ʒ�ó�� bool������ �����ָ� ��!!!!!!!! 

	if (!m_bOnceExcute)
	{
		if (m_bBursts)
		{
	
			// ���ۿ� ���� ī��Ʈ ����
			tRWParticleBuffer rwbuffer = { m_ModuleData.iMaxParticleCount, };
			m_RWBuffer->SetData(&rwbuffer);
			m_ModuleData.SpawnRate = 0;

		}

		else
		{

			// ���� ����Ʈ ���
			// 1�� ���� �ð�
			float fTimePerCount = 1.f / (float)m_ModuleData.SpawnRate;
			m_AccTime += DT;

			// �����ð��� ���� �����ð��� �Ѿ��
			if (fTimePerCount < m_AccTime)
			{
				// �ʰ� ���� ==> ���� ����
				float fData = m_AccTime / fTimePerCount;

				// �������� ���� �ð�
				m_AccTime = fTimePerCount * (fData - floor(fData));

				// ���ۿ� ���� ī��Ʈ ����
				tRWParticleBuffer rwbuffer = { (int)fData, };
				m_RWBuffer->SetData(&rwbuffer);
			}
		}

	}


	// ��ƼŬ ������Ʈ ��ǻƮ ���̴�

		m_ModuleDataBuffer->SetData(&m_ModuleData);

		m_UpdateCS->SetParticleBuffer(m_ParticleBuffer); 
		m_UpdateCS->SetRWParticleBuffer(m_RWBuffer); 
		m_UpdateCS->SetModuleData(m_ModuleDataBuffer);



	if (nullptr != m_NoiseTex)
	{
		m_UpdateCS->SetNoiseTexture(m_NoiseTex);
	}


	m_UpdateCS->SetParticleObjectPos(Transform()->GetWorldPos());
		

	m_UpdateCS->Execute();	


}

void CParticleSystem::render()
{

		// View ��İ� ���� ����� ������Ʈ �ϱ� ���� Ʈ������ ���ε�
		Transform()->UpdateData();

		// ����ȭ ���۷� ���� �����͸� t20�� ���ε�

		m_ParticleBuffer->UpdateData(20, PIPELINE_STAGE::PS_ALL);


		// ��� ������ t21 �� ���ε�
		m_ModuleDataBuffer->UpdateData(21, PIPELINE_STAGE::PS_GEOMETRY);



		// ��ƼŬ ���� ���̴� ������Ʈ
		GetMaterial(0)->SetTexParam(TEX_0, m_ParticleTex);

		GetMaterial(0)->UpdateData();


		// �ν��Ͻ����� �ϳ��� ���������ο��� ��ƼŬ �ִ� ������ŭ ������ ����
		GetMesh()->render_particle(m_ModuleData.iMaxParticleCount);




		m_ParticleBuffer->Clear();

		m_ModuleDataBuffer->Clear();

}

void CParticleSystem::render(UINT _iSubset) // �̰� �������̵��� �Լ��� ����� �غ��� �ҿ� ����. 
{

	render();


}

void CParticleSystem::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);
	
	SaveWString(GetName(), _File);

	fwrite(&m_ModuleData, sizeof(tParticleModule), 1, _File);
	fwrite(&m_bOnceExcute, sizeof(bool), 1, _File);
	fwrite(&m_bBursts, sizeof(bool), 1, _File);
	fwrite(&m_SpawnTime, sizeof(float), 1, _File);
	fwrite(&m_bLoop, sizeof(bool), 1, _File);

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


	fread(&m_ModuleData, sizeof(tParticleModule), 1, _File);
	fread(&m_bOnceExcute, sizeof(bool), 1, _File);
	fread(&m_bBursts, sizeof(bool), 1, _File);
	fread(&m_SpawnTime, sizeof(float), 1, _File);
	fread(&m_bLoop, sizeof(bool), 1, _File);



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
	//	axis = normal.Cross(Vector3::UnitY).Normalize();
	//}
	float angle = acos(normal.Dot(Vector3(0, 1, 0)));
	Matrix rotationMatrix = Matrix::CreateFromAxisAngle(axis, angle);

	return Vector3::Transform(randomDir, rotationMatrix);
} // �� �������� ������ �� �ۿ� �ȵ� 

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
