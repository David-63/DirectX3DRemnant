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
	m_ModuleData.Space = 0; // 시뮬레이션 좌표계

	m_ModuleData.MinLifeTime = 3.f;
	m_ModuleData.MaxLifeTime = 5.f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
	m_ModuleData.StartScale = 1.5f;
	m_ModuleData.EndScale = 0.2f;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = false;
	m_ModuleData.vStartColor = Vec4(0.2f, 0.3f, 1.0f, 1.0f);
	m_ModuleData.vEndColor = Vec4(0.4f, 1.f, 0.4f, 1.0f);


	// 아래 옵션은 아예 끄면 안됨. 
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



	// 입자 메쉬
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	m_pMesh = GetMesh();

	// 파티클 전용 재질
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"), 0);
	m_pMtrl = GetMaterial(0);

	// 파티클 업데이트 컴퓨트 쉐이더	
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();

	// 파티클 버퍼
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);


	// 파티클 스폰 개수 전달용 버퍼
	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);


	// 모듈 활성화 및 모듈 설정 정보 버퍼
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
	, m_ModuleData(_other.m_ModuleData) // excute하기 전에 단지 버퍼에 복사할 용도이므로 얕은 복사함
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
	m_ModuleData.Space = _other.m_ModuleData.Space; // 시뮬레이션 좌표계

	m_ModuleData.MinLifeTime = _other.m_ModuleData.MinLifeTime;
	m_ModuleData.MaxLifeTime = _other.m_ModuleData.MaxLifeTime;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = _other.m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE];
	m_ModuleData.StartScale = _other.m_ModuleData.StartScale;
	m_ModuleData.EndScale = _other.m_ModuleData.EndScale;

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = _other.m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE];
	m_ModuleData.vStartColor = _other.m_ModuleData.vStartColor;
	m_ModuleData.vEndColor = _other.m_ModuleData.vEndColor;


	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true; // 아예 끄면 안되는 옵션이라 true로 고정
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

	// == 재질 분류
	if (m_IsFoward)
		SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"), 0);

	else
		SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"D_ParticleRenderMtrl"), 0);



	// 파티클 업데이트 컴퓨트쉐이더
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();



	// ==============
	// 파티클 버퍼
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);


	// 파티클 스폰 갯수 전달용 버퍼
	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

	// 모듈 활성화 및 모듈 설정 정보 버퍼
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
	
	// 진입 조건 (Active) SetExcute
	// m_bWantExcute 이 값을 기본적으로 false
	

	///
	//if (!m_bOnceExcute)
	//{
	//	// 종료 조건
	//	if (++m_iSpawnCount >= 1 && m_bWantExcute)
	//	{
	//		m_bOnceExcute = true;
	//		m_iSpawnCount = 0;
	//	}

	//	if (m_bBursts)
	//	{

	//		// 버퍼에 스폰 카운트 전달
	//		tRWParticleBuffer rwbuffer = { m_ModuleData.iMaxParticleCount, };
	//		m_RWBuffer->SetData(&rwbuffer);
	//		m_ModuleData.SpawnRate = 0;

	//	}

	//	else
	//	{

	//		// 스폰 레이트 계산
	//		// 1개 스폰 시간
	//		float fTimePerCount = 1.f / (float)m_ModuleData.SpawnRate;
	//		m_AccTime += DT;

	//		// 누적시간이 개당 생성시간을 넘어서면
	//		if (fTimePerCount < m_AccTime)
	//		{
	//			// 초과 배율 ==> 생성 개수
	//			float fData = m_AccTime / fTimePerCount;

	//			// 나머지는 남은 시간
	//			m_AccTime = fTimePerCount * (fData - floor(fData));

	//			// 버퍼에 스폰 카운트 전달
	//			tRWParticleBuffer rwbuffer = { (int)fData, };
	//			m_RWBuffer->SetData(&rwbuffer);
	//		}
	//	}

	//	
	//	// 리셋 버튼 만들어주기 스폰카운트 0으로 해주는....


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

	// 파티클 업데이트 컴퓨트 쉐이더
	float originalSpawnTime = m_SpawnTime;

	if (m_UseTimeSpawn)
	{
		// 설정한 m_SpawnTime 수치가 있다면
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

	// View 행렬과 투영 행렬을 업데이트 하기 위해 트랜스폼 바인딩
	Transform()->UpdateData();

	// 구조화 버퍼로 입자 데이터를 t20에 바인딩

	m_ParticleBuffer->UpdateData(20, PIPELINE_STAGE::PS_ALL);


	// 모듈 데이터 t21 에 바인딩
	m_ModuleDataBuffer->UpdateData(21, PIPELINE_STAGE::PS_ALL);



	// 파티클 렌더 셰이더 업데이트
	GetMaterial(0)->SetTexParam(TEX_0, m_ParticleTex);

	GetMaterial(0)->UpdateData();


	// 인스턴싱으로 하나의 파이프라인에서 파티클 최대 개수만큼 렌더링 수행
	GetMesh()->render_particle(m_ModuleData.iMaxParticleCount);




	m_ParticleBuffer->Clear();
	m_ModuleDataBuffer->Clear();
	m_RWBuffer->Clear();	


}

void CParticleSystem::render(UINT _iSubset) // 이건 오버라이딩한 함수라서 여기다 해봤자 소용 없음. 
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
//	// 회전 축 생성 (여기서는 Y축을 기준으로 회전한다고 가정)
//	Vector3 rotationAxis = normal.Cross(Vector3(0, 1, 0)).Normalize();
//
//	// 랜덤 각도 생성
//	float angle = RandomFloat(-coneAngle / 2, coneAngle / 2);
//
//	// 회전 행렬 생성
//	Matrix rotationMatrix = Matrix::CreateFromAxisAngle(rotationAxis, angle);
//
//	// 노말 벡터 회전
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
//	// 원뿔의 중심 방향 벡터에 대한 극좌표에서의 랜덤 각도 생성
//	float theta = dis(gen) * DirectX::XM_2PI; // 0에서 2PI 사이
//	float phi = dis(gen) * openAngle; // 0에서 coneHalfAngleInRadians 사이
//
//	// 극좌표를 카테시안 좌표로 변환
//	float x = sinf(phi) * cosf(theta);
//	float y = sinf(phi) * sinf(theta);
//	float z = cosf(phi);
//
//	// 랜덤 방향 벡터 생성
//	Vector3 randomDir(x, y, z);
//
//	// 원뿔의 중심 방향 벡터를 기준으로 랜덤 방향 벡터를 회전
//	Vector3 axis = normal.Cross(Vector3(1, 0, 0)).Normalize();
//	if (axis.LengthSquared() == 0.0f) {
//		// coneDir이 이미 Z축과 평행한 경우, Y축을 회전 축으로 사용
//		axis = normal.Cross(Vector3::UnitY).Normalize();
//	}
//	float angle = acos(normal.Dot(Vector3(0, 1, 0)));
//	Matrix rotationMatrix = Matrix::CreateFromAxisAngle(axis, angle);
//
//	return Vector3::Transform(randomDir, rotationMatrix);
//} // 한 방향으로 나가는 것 밖에 안됨 



Vec3 CParticleSystem::CreateRandomDirection(const Vector3& normal, float openAngle)
{
	openAngle = XMConvertToRadians(openAngle);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0, 1);

	// 원뿔의 중심 방향 벡터에 대한 극좌표에서의 랜덤 각도 생성
	float theta = dis(gen) * DirectX::XM_2PI; // 0에서 2PI 사이
	float phi = dis(gen) * openAngle; // 0에서 coneHalfAngleInRadians 사이

	// 극좌표를 카테시안 좌표로 변환
	float x = sinf(phi) * cosf(theta);
	float y = sinf(phi) * sinf(theta);
	float z = cosf(phi);

	// 랜덤 방향 벡터 생성
	Vector3 randomDir(x, y, z);

	// 원뿔의 중심 방향 벡터를 기준으로 랜덤 방향 벡터를 회전
	Vector3 axis = normal.Cross(Vector3(1, 0, 0)).Normalize();

	//if (axis.LengthSquared() == 0.0f) {
	//	// coneDir이 이미 Z축과 평행한 경우, Y축을 회전 축으로 사용
	//	axis = normal.Cross(Vector3(0, 1, 0).Normalize());
	//}
	float angle = acos(normal.Dot(Vector3(0, 1, 0)));
	Matrix rotationMatrix = Matrix::CreateFromAxisAngle(axis, angle);

	return Vector3::Transform(randomDir, rotationMatrix);
}

float CParticleSystem::RandomFloat(float min, float max)
{
	// 난수 생성 엔진 초기화
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);

	return dis(gen);
}




//Vec3 CParticleSystem::CreateRandomDirection(const Vector3& normal, const Vector3& up, const Vector3& Front) // 사방으로 퍼져나가는 류 
//{
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	std::uniform_real_distribution<> dis(0, DirectX::XM_2PI); // 0에서 2PI 사이의 랜덤 값
//
//	// 랜덤 각도 생성
//	float angle = dis(gen);
//
//	// 법선 벡터와 up 벡터를 기준으로 회전 행렬 생성
//	Matrix rotMatrix = Matrix::CreateFromAxisAngle(normal, angle);
//
//	// 회전 행렬을 사용하여 랜덤 방향 생성
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
