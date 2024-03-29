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

struct tUINTS
{
	UINT X, Y;

	tUINTS() : X(0), Y(0) {}
	tUINTS(UINT _x, UINT _y) : X(_x), Y(_y) {}
	~tUINTS() {}
	tUINTS& set(UINT _x, UINT _y)
	{
		X = _x;
		Y = _y;
		return *this;
	}
};

struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;

	Vec3 vNormal;
	Vec3 vTangent;
	Vec3 vBinormal;

	// Animation ����ġ �� �ε���
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
	Vec4 vDiffuse;	// ���� ����
	Vec4 vAmbient;	// �ֺ� ��(ȯ�� ��)
};

// LightInfo
struct tLightInfo
{
	tLightColor Color;		 // ���� ����

	Vec4		vWorldPos;   // ������ ���� �����̽� ��ġ
	Vec4		vWorldDir;	 // ���� ������ ����

	UINT		LightType;   // ���� Ÿ��(���⼺, ��, ����Ʈ)
	float		Radius;		 // ���� �ݰ�(��Ÿ�)
	float		Angle;		 // ���� ����
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
	Vec4	vLocalPos;		// ������Ʈ�κ��� ������ �Ÿ�
	Vec4	vWorldPos;		// ��ƼŬ ���� ������ġ
	Vec4	vWorldScale;	// ��ƼŬ ũ��	
	Vec4	vColor;			// ��ƼŬ ����
	Vec4	vVelocity;		// ��ƼŬ ���� �ӵ�
	Vec4	vForce;			// ��ƼŬ�� �־��� ��
	Vec4	vRandomForce;	// ��ƼŬ�� ����Ǵ� �������� ��

	float   Age;			// ���� �ð�
	float   PrevAge;		// ���� ������ ���� �ð�
	float   NomalizedAge;	// ������ �����ð��� 0~1�� ����ȭ �� ��
	float	LifeTime;		// ����
	float	Mass;			// ����
	float   ScaleFactor;	// �߰� ũ�� ����

	int     Active;			// ��ƼŬ Ȱ��ȭ ����
	int     pad;
};

struct tRWParticleBuffer
{
	int		iSpawnCount;			// ���� ��ų ��ƼŬ ����
	int		padding[3];
};


struct tParticleModule
{
	Vec3    vRandomSpark;

	// ���� ���
	Vec4    vSpawnColor;
	Vec4	vSpawnScaleMin;
	Vec4	vSpawnScaleMax;
	Vec3	vBoxShapeScale;

	float	fSphereShapeRadius;
	float   fSphereOffset;
	int		SpawnShapeType;		// 0 : BOX, 1 : Sphere

	float   fSpawnAreaOffsetFactor;
	int		SpawnRate;			// �ʴ� ���� ����
	int		Space;				// ��ƼŬ ������Ʈ ��ǥ�� ( 0 : World,  1 : Local)	

	float   MinLifeTime;		// �ּ� ����	
	float   MaxLifeTime;		// �ִ� ����
	int     spawnpad[3];

	// Color Change ���
	Vec4	vStartColor;		// �ʱ� ����
	Vec4	vEndColor;			// ���� ����
	bool    bStrongColor;		// 1 : ���� ����, 0 : ���� ����

	// Scale Change ���
	float	StartScale;			// �ʱ� ����
	float	EndScale;			// ���� ����	

	// ���� �ִ�ũ��
	int		iMaxParticleCount;
	int		ipad[4];

	// Add Velocity ���
	Vec4	vVelocityDir;
	int     AddVelocityType;	// 0 : From Center, 1: To Center, 2 : Fixed Direction	
	float	OffsetAngle;
	float	Speed;
	int     addvpad;

	// Drag ��� - �ӵ� ����
	float	StartDrag;
	float	EndDrag;

	// Gravity  - �߷�
	float	fGravityForce;

	// NoiseForce ��� - ���� �� ����	
	float	fNoiseTerm;		// ���� �� ���� ����
	float	fNoiseForce;	// ���� �� ũ��

	// Render ���
	int		VelocityAlignment;	// 1 : �ӵ����� ���(�̵� �������� ȸ��) 0 : ��� ����
	int		VelocityScale;		// 1 : �ӵ��� ���� ũ�� ��ȭ ���, 0 : ��� ����
	int		AnimUse;		// 1 : �ִϸ��̼� ���, 
	int		AnimLoop;		// 1 : �ִϸ��̼� ����, 0 : ������ ����ŭ 1�� ��� 
	int		iAnimXCount;
	int		iAnimYCount;
	float	fAnimFrmTime;
	float   fAnimSpeed;
	float   vMaxSpeed;			// �ִ� ũ�⿡ �����ϴ� �ӷ�
	Vec4	vMaxVelocityScale;	// �ӷ¿� ���� ũ�� ��ȭ�� �ִ�ġ	
	float   fRotAngle;
	float   fRotSpeed;
	int     bRot;				// 1 : ȸ�� ���, 0: ��� ����
	int		bUseSpark;
	int     bEmissive;
	Vec4	vEmissiveColor;


	// Module Check
	int		ModuleCheck[(UINT)PARTICLE_MODULE::END];
};


// ���� ����ü
struct tRay
{
	Vec3 vStart;
	Vec3 vDir;
};

// Raycast ����� ���� ����ü
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
// ������� ���� ����ü
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

// Material ���
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

	// �ؽ��� ���� true / false �뵵
	int arrTex[(UINT)TEX_PARAM::TEX_END];

	// 3D Animation ����
	int	arrAnimData[4];
};


struct tPassIndices
{
	UINT curidx;
	UINT targetIdx;
	UINT padd2;
	UINT padd3;
	bool operator==(const int _idx) const
	{
		return curidx == _idx;
	}
	bool operator==(const tPassIndices& rhs) const
	{
		return curidx == rhs.curidx;
	}
	tPassIndices(UINT _idx) : curidx(_idx), targetIdx(0), padd2(0), padd3(0) {}
	tPassIndices(UINT _idx, UINT _parent) : curidx(_idx), targetIdx(_parent), padd2(0), padd3(0) {}
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
		// RigidStatic�� ��,
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
		, CollideType(1)
		, boneIdx(0)
	{}

	GEOMETRY_TYPE eGeomType;
	Vector3 size;
	tMassProperties massProperties;
	physx::PxFilterData filterData;
	Vector3 offset;
	UINT CollideType;    //  1:Other 2:Player 3:Monster
	int boneIdx;
};

struct tRayCastInfo
{
	physx::PxRigidActor* hitActor;
	Vec3 hitPos;
	Vec3 hitNormal;
	bool hit;
	float dist;
};

struct tPNode
{
	tPNode()
		: pPrevNode(nullptr)
		, fFromParent(0.f)
		, fToDest(0.f)
		, fFinal(0.f)
		, iIdxX(0)
		, iIdxY(0)
		, bMove(true)
		, bOpen(false)
		, bClosed(false)
	{}
	tPNode*     pPrevNode;   // ���� ���
	float		fFromParent; // ���� ��忡�� ���� �������� �Ÿ�
	float		fToDest;	 // ���� ��忡�� ������ ������ �Ÿ�
	float		fFinal;		 // ���� �� ���� ��ģ ��(�켱���� ����)

	int			iIdxX;
	int			iIdxY;
	bool		bMove = true;   // �̵� ���� �Ұ���
	bool		bOpen = false;   // OpenList �� ������
	bool		bClosed = false; // ClostList �� ����ִ���

};

//struct tPairHash
//{
//	size_t operator()(const std::pair<int, int>& p) const
//	{
//		return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
//	}
//};

struct tYX
{
	tYX(int _y, int _x)
		: x(_x), y(_y)
	{}
	int x;
	int y;
};

struct tRangeYX
{
	tRangeYX(int _y1, int _x1, int _y2, int _x2)
		: y1(_y1), y2(_y2), x1(_x1), x2(_x2)
	{};

	int x1;
	int x2;
	int y1;
	int y2;
};