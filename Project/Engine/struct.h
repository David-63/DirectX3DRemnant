#pragma once


struct tTimeCtrl
{
	float curTime;
	float maxTime;
	bool active;

	tTimeCtrl() : curTime(0.f), maxTime(1.f), active(false) {}
	tTimeCtrl(float _value) : curTime(0.f), maxTime(_value), active(false) {}

	bool IsFinish() { return (curTime >= maxTime); }
	bool IsActivate() { return active; }

	void SetFinishTime(float _value) { maxTime = _value; }
	void Activate() { active = true; }
	void ResetTime() { curTime = 0; active = false; }

};

struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;

	Vec3 vNormal;
	Vec3 vTangent;
	Vec3 vBinormal;

	// Animation 가중치 및 인덱스
	Vec4 vWeights;
	Vec4 vIndices;
};

typedef tVertex Vtx;




// Event
struct tEvent
{
	EVENT_TYPE	Type;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};


struct tDebugShapeInfo
{
	SHAPE_TYPE	eShape;
	Matrix		matWorld;
	Vec3		vWorldPos;
	Vec3		vWorldScale;
	Vec3		vWorldRotation;
	Vec4		vColor;
	float		fMaxTime;
	float		fCurTime;
	bool		bDepthTest;
};



struct tLightColor
{
	Vec4 vDiffuse;	// 빛의 색상
	Vec4 vAmbient;	// 주변 광(환경 광)
};

// LightInfo
struct tLightInfo
{
	tLightColor Color;		 // 빛의 색상

	Vec4		vWorldPos;   // 광원의 월드 스페이스 위치
	Vec4		vWorldDir;	 // 빛을 보내는 방향

	UINT		LightType;   // 빛의 타입(방향성, 점, 스포트)
	float		Radius;		 // 빛의 반경(사거리)
	float		Angle;		 // 빛의 각도
	int			padding;
};


// TileMap
struct tTile
{
	Vec2 vLeftTop;
	Vec2 vSlice;
};

// Animator2D
struct tAnim2DFrm
{
	Vec2	LeftTopUV;
	Vec2	SliceUV;
	Vec2	Offset;
	float	fDuration;
};


// Particle
struct tParticle
{
	Vec4	vLocalPos;		// 오브젝트로부터 떨어진 거리
	Vec4	vWorldPos;		// 파티클 최종 월드위치
	Vec4	vWorldScale;	// 파티클 크기	
	Vec4	vColor;			// 파티클 색상
	Vec4	vVelocity;		// 파티클 현재 속도
	Vec4	vForce;			// 파티클에 주어진 힘
	Vec4	vRandomForce;	// 파티클에 적용되는 랜덤방향 힘

	float   Age;			// 생존 시간
	float   PrevAge;		// 이전 프레임 생존 시간
	float   NomalizedAge;	// 수명대비 생존시간을 0~1로 정규화 한 값
	float	LifeTime;		// 수명
	float	Mass;			// 질량
	float   ScaleFactor;	// 추가 크기 배율

	int     Active;			// 파티클 활성화 여부
	int     pad;
};

struct tRWParticleBuffer
{
	int		iSpawnCount;			// 스폰 시킬 파티클 개수
	int		padding[3];
};


struct tParticleModule
{
	Vec3    vRandomSpark;
	int     iGunSpark;

	// 스폰 모듈
	Vec4    vSpawnColor;
	Vec4	vSpawnScaleMin;
	Vec4	vSpawnScaleMax;	
	Vec3	vBoxShapeScale;
	
	float	fSphereShapeRadius;	
	float   fSphereOffset;
	int		SpawnShapeType;		// 0 : BOX, 1 : Sphere
	int     BoxPad[4];

	float   fSpawnAreaOffsetFactor;
	int		SpawnRate;			// 초당 생성 개수
	int		Space;				// 파티클 업데이트 좌표계 ( 0 : World,  1 : Local)	
	
	float   MinLifeTime;		// 최소 수명	
	float   MaxLifeTime;		// 최대 수명
	int     spawnpad;

	// Color Change 모듈
	Vec4	vStartColor;		// 초기 색상
	Vec4	vEndColor;			// 최종 색상
	bool    bStrongColor;		// 1 : 강한 색상, 0 : 약한 색상

	// Scale Change 모듈
	float	StartScale;			// 초기 배율
	float	EndScale;			// 최종 배율	

	// 버퍼 최대크기
	int		iMaxParticleCount;
	int		ipad;

	// Add Velocity 모듈
	Vec4	vVelocityDir;	
	int     AddVelocityType;	// 0 : From Center, 1: To Center, 2 : Fixed Direction	
	float	OffsetAngle;
	float	Speed;
	int     addvpad;

	// Drag 모듈 - 속도 제한
	float	StartDrag;
	float	EndDrag;

	// Gravity  - 중력
	float	fGravityForce;

	// NoiseForce 모듈 - 랜덤 힘 적용	
	float	fNoiseTerm;		// 랜덤 힘 변경 간격
	float	fNoiseForce;	// 랜덤 힘 크기

	// Render 모듈
	int		VelocityAlignment;	// 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
	int		VelocityScale;		// 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함	
	float   vMaxSpeed;			// 최대 크기에 도달하는 속력
	Vec4	vMaxVelocityScale;	// 속력에 따른 크기 변화량 최대치	
	float   fRotAngle;
	float   fRotSpeed;
	int     bRot;				// 1 : 회전 사용, 0: 사용 안함

	// Module Check
	int		ModuleCheck[(UINT)PARTICLE_MODULE::END];
};


// 광선 구조체
struct tRay
{
	Vec3 vStart;
	Vec3 vDir;
};

// Raycast 결과를 받을 구조체
struct tRaycastOut
{
	Vec2	vUV;
	float	fDist;
	int		bSuccess;
};

// Animation 3D
struct tFrameTrans
{
	Vec4	vTranslate;
	Vec4	vScale;
	Vec4	vRot;
};

// MT : Meshdata sTruct
struct tMTKeyFrame
{
	double	dTime;
	int		iFrame;
	Vec3	vTranslate;
	Vec3	vScale;
	Vec4	qRot;
};

struct tMTBone
{
	wstring				strBoneName;
	int					iDepth;
	int					iParentIdx;
	Matrix				matOffset;
	Matrix				matBone;
	vector<tMTKeyFrame>	vecKeyFrame;

	FbxTime::EMode		eMode;
};

struct tMTAnimClip
{
	wstring			strAnimName;
	int				iStartFrame;
	int				iEndFrame;
	int				iFrameLength;
	
	double			dStartTime;
	double			dEndTime;
	double			dTimeLength;
	float			fUpdateTime;

	FbxTime::EMode	eMode;
};


// ===========
// Instancing
// ===========
union uInstID
{
	struct {
		UINT iMesh;
		WORD iMtrl;
		WORD iMtrlIdx;
	};
	ULONG64 llID;
};

class CGameObject;
struct tInstObj
{
	CGameObject* pObj;
	UINT		 iMtrlIdx;
};

struct tInstancingData
{
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
	int    iRowIdx;
};

// ===================
// 상수버퍼 대응 구조체
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matWorldInv;
	Matrix matView;
	Matrix matViewInv;
	Matrix matProj;
	Matrix matProjInv;

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_transform;

// Material 계수
struct tMtrlData
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
	Vec4 vEmv;
};

struct tMtrlConst
{
	tMtrlData mtrl;

	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];

	// 텍스쳐 세팅 true / false 용도
	int arrTex[(UINT)TEX_PARAM::TEX_END];

	// 3D Animation 정보
	int	arrAnimData[4];
};


struct tGlobal
{
	Vec2  Resolution;
	float tDT;
	float tAccTime;
	UINT  Light2DCount;
	UINT  Light3DCount;
	int	  globalpadding[2];
};

extern tGlobal GlobalData;

struct tMassProperties
{
	tMassProperties(float _staticFriction = 0.75f, float _dynamicFriction = 0.55f, float _restitution = 0.603f)
		: staticFriction(_staticFriction)
		, dynamicFriction(_dynamicFriction)
		, restitution(_restitution)
	{ }

	float staticFriction;
	float dynamicFriction;
	float restitution;
};

struct Geometries
{
	Geometries(GEOMETRY_TYPE _geometryType, Vector3 vBoxHalfSize)
		: eGeomType(GEOMETRY_TYPE::Box)
	{
		if (GEOMETRY_TYPE::Box == _geometryType)
		{
			boxGeom = physx::PxBoxGeometry(physx::PxVec3(vBoxHalfSize.x, vBoxHalfSize.y, vBoxHalfSize.z));
		}
	}

	Geometries(GEOMETRY_TYPE _geometryType, float fRadius, float fHalfHeight)
		: eGeomType(GEOMETRY_TYPE::Capsule)
	{
		if (GEOMETRY_TYPE::Capsule == _geometryType)
		{
			capsuleGeom = physx::PxCapsuleGeometry(fRadius, fHalfHeight);
		}
	}

	Geometries(GEOMETRY_TYPE _geometryType, float fRadius)
		: eGeomType(GEOMETRY_TYPE::Sphere)
	{
		if (GEOMETRY_TYPE::Sphere == _geometryType)
		{
			sphereGeom = physx::PxSphereGeometry(fRadius);
		}
	}

	Geometries(GEOMETRY_TYPE _geometryType)
		: eGeomType(GEOMETRY_TYPE::Plane)
	{
		// RigidStatic일 떄,
		if (GEOMETRY_TYPE::Plane == _geometryType)
		{
			planeGeom = physx::PxPlaneGeometry();
		}
	}

	physx::PxBoxGeometry boxGeom;
	physx::PxCapsuleGeometry capsuleGeom;
	physx::PxPlaneGeometry planeGeom;
	physx::PxSphereGeometry sphereGeom;
	GEOMETRY_TYPE eGeomType;
};

//struct tPhysicsInfo
//{
//	tPhysicsInfo()
//		: eActorType(ACTOR_TYPE::Static)
//		, eGeomType(GEOMETRY_TYPE::Box)
//		, size(1.f, 1.f, 1.f)
//		, massProperties(tMassProperties())
//		, pGeometries(nullptr)
//		, filterData{}
//	{
//	}
//
//	ACTOR_TYPE eActorType;
//	GEOMETRY_TYPE eGeomType;
//	Vector3 size;
//	tMassProperties massProperties;
//	Geometries* pGeometries;
//	physx::PxFilterData filterData;
//};

struct tShapeInfo
{
	tShapeInfo()
		: eGeomType(GEOMETRY_TYPE::Sphere)
		, size(1.f,1.f,1.f)
		, massProperties(tMassProperties())
		, filterData{}
		, offset(0.f,0.f,0.f)
		, isPlayer(false)
	{}

	GEOMETRY_TYPE eGeomType;
	Vector3 size;
	tMassProperties massProperties;
	physx::PxFilterData filterData;
	Vector3 offset;
	bool isPlayer;
};

struct tRayCastInfo
{
	physx::PxRigidActor* hitActor;
	Vec3 hitPos;
	Vec3 hitNormal;
	bool hit;
	float dist;
};

