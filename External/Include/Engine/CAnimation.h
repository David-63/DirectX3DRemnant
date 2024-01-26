#pragma once
#include "CRes.h"
class CAnimation : public CRes
{


private:
	// 파일로부터 로딩
	virtual int Load(const wstring& _strFilePath) = 0;

public:
	// 파일로 저장
	virtual int Save(const wstring&) = 0;

};
