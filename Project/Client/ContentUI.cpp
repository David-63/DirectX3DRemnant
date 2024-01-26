#include "pch.h"
#include "ContentUI.h"

#include <Engine\CResMgr.h>
#include <Engine\CPathMgr.h>
#include <Engine\CEventMgr.h>

#include "TreeUI.h"
#include "ImGuiMgr.h"
#include "InspectorUI.h"



ContentUI::ContentUI()
    : UI("##Content")
{
    SetName("Content");

    // ContentUI �ȿ� �ڽ����� Tree �� �߰��Ѵ�.
    m_Tree = new TreeUI;
    m_Tree->SetName("ContentTree");
    m_Tree->SetActive(true);
    m_Tree->ShowRoot(false);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&ContentUI::SetTargetToInspector);
	m_Tree->SetDragDropID("Resource");
    AddChildUI(m_Tree);   
}

ContentUI::~ContentUI()
{

}

void ContentUI::init()
{
	Reload();
}

void ContentUI::tick()
{
    UI::tick();

	if (CResMgr::GetInst()->IsResourceChanged())
	{
		ResetContent();
	}
}

int ContentUI::render_update()
{   
    return 0;
}

void ContentUI::Reload()
{
	// Content ������ �ִ� ���� �̸����� Ȯ��
	m_vecResPath.clear();
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	FindFileName(strContentPath);

	// ���ϸ����� ���ҽ� �ε�
	for (size_t i = 0; i < m_vecResPath.size(); ++i)
	{
		RES_TYPE type = GetResTypeByExt(m_vecResPath[i]);

		if (type == RES_TYPE::END)
			continue;

		switch (type)
		{
		case RES_TYPE::MESHDATA:
			CResMgr::GetInst()->Load<CMeshData>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::MATERIAL:
			CResMgr::GetInst()->Load<CMaterial>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::PREFAB:

			break;
		case RES_TYPE::MESH:
			CResMgr::GetInst()->Load<CMesh>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::TEXTURE:
			CResMgr::GetInst()->Load<CTexture>(m_vecResPath[i], m_vecResPath[i]);
			break;
		case RES_TYPE::SOUND:
			CResMgr::GetInst()->Load<CSound>(m_vecResPath[i], m_vecResPath[i]);
			break;		
		}
	}


	// ���ҽ��� �������� üũ
	for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

		for (const auto& pair : mapRes)
		{
			if (pair.second->IsEngineRes())
				continue;

			wstring strFilePath = strContentPath + pair.first;
			if (!filesystem::exists(strFilePath))
			{
				tEvent evn = {};
				evn.Type = EVENT_TYPE::DELETE_RESOURCE;
				evn.wParam = (DWORD_PTR)i;
				evn.lParam = (DWORD_PTR)pair.second.Get();
				CEventMgr::GetInst()->AddEvent(evn);
			}
		}
	}

	// Ʈ�� ����
	ResetContent();
}


void ContentUI::ResetContent()
{
	// Tree Clear
	m_Tree->Clear();
	m_Tree->AddItem("Root", 0);

	// ���ҽ� �Ŵ������� ���� ��� ���ҽ� ��� �޾ƿ�
	for (size_t i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

		// m_Tree �� ���� ���ҽ� ����� AddItem
		TreeNode* pCategory = m_Tree->AddItem(ToString((RES_TYPE)i), 0);
        pCategory->SetCategoryNode(true);

		for (const auto& pair : mapRes)
		{
			m_Tree->AddItem(string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get(), pCategory);
		}
	}
}

void ContentUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
	CRes* pSelectObject = (CRes*)pSelectedNode->GetData();

	if (nullptr == pSelectObject)
		return;

	// Inspector �� ���õ� Resource �� �˷��ش�.	
	InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetResource(pSelectObject);
}


void ContentUI::FindFileName(const wstring& _FolderPath)
{
	WIN32_FIND_DATA FindData = {};

	wstring FolderPath = _FolderPath + L"*.*";

	HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);

	while (FindNextFile(hFindHandle, &FindData))
	{
		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!wcscmp(FindData.cFileName, L".."))
			{
				continue;
			}

			FindFileName(_FolderPath + FindData.cFileName + L"\\");
			continue;
		}

		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
		wstring RelativePath = _FolderPath + FindData.cFileName;
		RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());

		m_vecResPath.push_back(RelativePath);
	}

	FindClose(hFindHandle);
}

RES_TYPE ContentUI::GetResTypeByExt(const wstring& _relativepath)
{
	wchar_t szExt[50] = {};
	_wsplitpath_s(_relativepath.c_str(), 0, 0, 0, 0, 0, 0, szExt, 50);	
	wstring strExt = szExt;
		
	if (L".mdat" == strExt)
		return RES_TYPE::MESHDATA;
	else if (L".pref" == strExt)
		return RES_TYPE::PREFAB;
	else if (L".mesh" == strExt)
		return RES_TYPE::MESH;
	else if (L".mtrl" == strExt)
		return RES_TYPE::MATERIAL;
	else if (L".png" == strExt || L".jpg" == strExt
		|| L".jpeg" == strExt || L".bmp" == strExt
		|| L".tga" == strExt || L".dds" == strExt)
		return RES_TYPE::TEXTURE;
	else if (L".mp3" == strExt || L".wav" == strExt || L".oga" == strExt)
		return RES_TYPE::SOUND;
	else
		return RES_TYPE::END;
}

