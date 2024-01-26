#include "pch.h"

#include "CEventMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRenderMgr.h"
#include "ptr.h"
#include "CResMgr.h"

void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, int _LayerIdx)
{
	_NewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};

	evn.Type = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_NewObject;
	evn.lParam = _LayerIdx;

	CEventMgr::GetInst()->AddEvent(evn);
}
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, const wstring& _LayerName)
{
	_NewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};

	evn.Type = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_NewObject;
	evn.lParam = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(_LayerName)->GetLayerIndex();

	CEventMgr::GetInst()->AddEvent(evn);
}

void DestroyObject(CGameObject* _DeletObject)
{
	if (_DeletObject->IsDead())
		return;

	tEvent evn = {};

	evn.Type = EVENT_TYPE::DELETE_OBJECT;
	evn.wParam = (DWORD_PTR)_DeletObject;
	
	CEventMgr::GetInst()->AddEvent(evn);
}




void DrawDebugRect(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor
	, Vec3 _vRotation, float _fTime, bool DepthTest)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::RECT;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_vWorldScale.x, _vWorldScale.y, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;
	info.bDepthTest = DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}
void DrawDebugRect(const Matrix& _matWorld, Vec4 _vColor, float _fTime, bool DepthTest)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::RECT;
	info.fMaxTime = _fTime;	
	info.vColor = _vColor;
	info.bDepthTest = DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}


void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation
					, float _fTime, bool DepthTest)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::CIRCLE;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;
	info.bDepthTest = DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}
void DrawDebugCircle(const Matrix& _matWorld, Vec4 _vColor, float _fTime, bool DepthTest)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::CIRCLE;
	info.fMaxTime = _fTime;	
	info.vColor = _vColor;
	info.bDepthTest = DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}


void DrawDebugCube(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor
					, Vec3 _vRotation, float _fTime, bool DepthTest)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::CUBE;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;
	info.bDepthTest = DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}
void DrawDebugCube(const Matrix& _matWorld, Vec4 _vColor, float _fTime, bool DepthTest)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::CUBE;
	info.fMaxTime = _fTime;
	info.vColor = _vColor;
	info.bDepthTest = DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}


void DrawDebugSphere(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor
	, Vec3 _vRotation, float _fTime, bool DepthTest)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::SPHERE;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;
	info.bDepthTest = DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}
void DrawDebugSphere(const Matrix& _matWorld, Vec4 _vColor, float _fTime, bool DepthTest)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::SPHERE;
	info.fMaxTime = _fTime;
	info.vColor = _vColor;
	info.bDepthTest = DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}


bool IsValidObj(CGameObject*& _Target)
{
	if (nullptr == _Target)
		return false;

	if (_Target->IsDead())
	{
		_Target = nullptr;
		return false;
	}

	return true;
}



const char* ToString(RES_TYPE type)
{
	return RES_TYPE_STR[(UINT)type];
}
const wchar_t* ToWString(RES_TYPE type)
{
	return RES_TYPE_WSTR[(UINT)type];
}

const char* ToString(COMPONENT_TYPE type)
{
	return COMPONENT_TYPE_STR[(UINT)type];
}
const wchar_t* ToWString(COMPONENT_TYPE type)
{
	return COMPONENT_TYPE_WSTR[(UINT)type];
}


wstring GetRelativePath(const wstring& _strBase, const wstring& _strPath)
{
	wstring strRelativePath;
	if (-1 == _strPath.find(_strBase))
	{
		return strRelativePath;
	}

	strRelativePath = _strPath.substr(_strBase.length(), _strPath.length());
	return strRelativePath;

	return wstring();
}


void SaveWString(const wstring& _str, FILE* _File)
{	
	UINT iLen = (UINT)_str.length();
	fwrite(&iLen, sizeof(UINT), 1, _File);
	fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
}
void LoadWString(wstring& _str, FILE* _File)
{
	wchar_t szBuffer[256] = {};

	UINT iLen = 0;
	fread(&iLen, sizeof(UINT), 1, _File);
	fread(szBuffer, sizeof(wchar_t), iLen, _File);	

	_str = szBuffer;
}


Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat)
{
	Matrix mat;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat.m[i][j] = (float)_mat.Get(i, j);
		}
	}
	return mat;
}


void SaveResRef(Ptr<CRes> _Res, FILE* _File)
{
	int i = 0;
	if (nullptr == _Res)
	{
		fwrite(&i, sizeof(i), 1, _File);
	}
	else
	{
		i = 1;
		fwrite(&i, sizeof(i), 1, _File);
		SaveWString(_Res->GetKey(), _File);
		SaveWString(_Res->GetRelativePath(), _File);
	}
}
