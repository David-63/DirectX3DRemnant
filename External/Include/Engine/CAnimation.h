#pragma once
#include "CRes.h"
class CAnimation : public CRes
{


private:
	// ���Ϸκ��� �ε�
	virtual int Load(const wstring& _strFilePath) = 0;

public:
	// ���Ϸ� ����
	virtual int Save(const wstring&) = 0;

};
