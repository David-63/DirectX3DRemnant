#include "pch.h"
#include "MaterialUI.h"

#include "ParamUI.h"
#include "TreeUI.h"
#include "ListUI.h"
#include "ContentUI.h"
#include "ImGuiMgr.h"
#include "InspectorUI.h"

#include <Engine\CResMgr.h>
#include <Engine\CMaterial.h>

MaterialUI::MaterialUI()
    : ResUI(RES_TYPE::MATERIAL)
{
    SetName("Material");
}

MaterialUI::~MaterialUI()
{
}

int MaterialUI::render_update()
{
    ResUI::render_update();

    // Material 이름
    ImGui::Text("Material"); 
    ImGui::SameLine();

    Ptr<CMaterial> pMtrl = (CMaterial*)GetTargetRes().Get();
    string strKey = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
    ImGui::InputText("##MtrlUIName", (char*)strKey.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);


     //메터리얼 삭제
    if (false == pMtrl->IsEngineRes())
    {
        // Delete Resource Button
        if (ImGui::Button("Del##Resource")) {
            // Resource 삭제
            // 1. Content UI 창에서 정보 삭제
            CResMgr::GetInst()->DeleteMaterial(pMtrl->GetKey());
            ContentUI* content = (ContentUI*)ImGuiMgr::GetInst()->FindUI("##Content");
            content->ResetContent();

            // 2. Inspector UI창에서 정보 삭제 (사라진 정보를 띄우는 것은 말이 안되므로)
            InspectorUI* Inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
            Inspector->ClearTargetResource();


        }
    }

    // ===========================
// 재질 생성 기능 
    ImGui::SameLine();
    if (ImGui::Button("Create New Mtrl"))
    {
        // 비긴팝모달이랑 꼭 문자열 똑같이 해야됨
        ImGui::OpenPopup("Making New Mtrl");
    }

    bool unused_open = true;
    if (ImGui::BeginPopupModal("Making New Mtrl", &unused_open))
    {
        ImGui::Text("SetName \n\n");
        static char buf[512];
        ImGui::InputText("##SetName", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_None);


        if (ImGui::Button("Complete"))
        {
            string name = buf;
            wstring newName = wstring(name.begin(), name.end());

            Ptr<CMaterial> pNewMtrl = new CMaterial(false); // Create Material 
            pNewMtrl->SetKey(newName);


            // ====== 상대경로로 지정 
            wstring MtrlResKey = L"material\\" + newName + L".mtrl";
            pNewMtrl->SetRelativePath(MtrlResKey);

            // 위에서 한 내용을 토대로 리소스 추가
            CResMgr::GetInst()->AddRes<CMaterial>(newName, pNewMtrl);

            // 실제 경로의 파일로 저장
            pNewMtrl->Save(MtrlResKey);

            ContentUI* content = (ContentUI*)ImGuiMgr::GetInst()->FindUI("##Content");
            if (CResMgr::GetInst()->IsResourceChanged())
            {
                content->ResetContent(); // 이건 쌤꺼 
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }









    // GraphicsShader 이름
    ImGui::Text("Shader  ");
    ImGui::SameLine();

    Ptr<CGraphicsShader> pShader = pMtrl->GetShader();
    if (nullptr != pShader)
    {
        string strKey = string(pShader->GetKey().begin(), pShader->GetKey().end());
        ImGui::InputText("##ShaderUIName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    }
    else
    {
        char szEmtpy[10] = {};
        ImGui::InputText("##ShaderUIName", szEmtpy, 10, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
    }

    // GraphicsShader 드랍 체크    
    if (ImGui::BeginDragDropTarget())
    {
        // 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
        const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("Resource");
        if (pPayLoad)
        {
            TreeNode* pNode = (TreeNode*)pPayLoad->Data;
            CRes* pRes = (CRes*)pNode->GetData();
            if (RES_TYPE::GRAPHICS_SHADER == pRes->GetType())
            {
                ((CMaterial*)GetTargetRes().Get())->SetShader((CGraphicsShader*)pRes);
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::SameLine();

    if (ImGui::Button("##ShaderSelectBtn", ImVec2(18, 18)))
    {
        const map<wstring, Ptr<CRes>>& mapShader = CResMgr::GetInst()->GetResources(RES_TYPE::GRAPHICS_SHADER);

        ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
        pListUI->Reset("GraphicsShader List", ImVec2(300.f, 500.f));
        for (const auto& pair : mapShader)
        {
            pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
        }

        // 항목 선택시 호출받을 델리게이트 등록
        pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MaterialUI::SelectShader);
    }


    ImGui::NewLine();
    ImGui::Text("Parameter");
    

    // Shader 에서 요구하는 ScalarParameter 를 UI 에 노출
    if (nullptr == pShader)
        return 0;

    const vector<tScalarParam>& vecScalarParam = pShader->GetScalarParam();

    for (size_t i = 0; i < vecScalarParam.size(); ++i)
    {
        switch (vecScalarParam[i].eParam)
        {
        // Shader Parameter 가 Int 타입인 경우
        case INT_0:
        case INT_1:
        case INT_2:
        case INT_3:
        {
            // 현재 머티리얼에 세팅된 값을 전달   
            int data = 0;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Int(vecScalarParam[i].strDesc, &data))
            {
                // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
            break;
        case FLOAT_0:
        case FLOAT_1:
        case FLOAT_2:
        case FLOAT_3:
        {
            // 현재 머티리얼에 세팅된 값을 전달   
            float data = 0;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Float(vecScalarParam[i].strDesc, &data))
            {
                // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
            break;
        case VEC2_0:
        case VEC2_1:
        case VEC2_2:
        case VEC2_3:
        {
            // 현재 머티리얼에 세팅된 값을 전달   
            Vec2 data;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Vec2(vecScalarParam[i].strDesc, &data))
            {
                // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
            break;
        case VEC4_0:
        case VEC4_1:
        case VEC4_2:
        case VEC4_3:
        {
            // 현재 머티리얼에 세팅된 값을 전달   
            Vec4 data;
            pMtrl->GetScalarParam(vecScalarParam[i].eParam, &data);
            if (ParamUI::Param_Vec4(vecScalarParam[i].strDesc, &data))
            {
                // UI 쪽에서 값이 변경되었으면, 실제 머티리얼 에도 적용
                pMtrl->SetScalarParam(vecScalarParam[i].eParam, &data);
            }
        }
            break;
        case MAT_0:
        case MAT_1:
        case MAT_2:
        case MAT_3:
            break;
        }        
    }

    // Shader 에서 요구하는 Texture Parameter 를 UI 에 노출
    const vector<tTexParam>& vecTexParam = pShader->GetTexParam();
    for (size_t i = 0; i < vecTexParam.size(); ++i)
    {
        ImGui::NewLine();
        Ptr<CTexture> pCurTex = pMtrl->GetTexParam(vecTexParam[i].eParam);
        if (ParamUI::Param_Tex(vecTexParam[i].strDesc, pCurTex, this, (UI_DELEGATE_1)&MaterialUI::SelectTexture))
        {
            m_eSelected = vecTexParam[i].eParam;
        }
        pMtrl->SetTexParam(vecTexParam[i].eParam, pCurTex);
    }

    if (ImGui::Button("Save Material##MaterialSaveBtn", ImVec2(200, 18)))
    {
        pMtrl->Save(pMtrl->GetRelativePath());
    }

    return 0;
}


void MaterialUI::SelectTexture(DWORD_PTR _Key)
{
    string strKey = (char*)_Key;
    Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));

    Ptr<CMaterial> pMtrl = (CMaterial*)GetTargetRes().Get();
    pMtrl->SetTexParam(m_eSelected, pTex);
}

void MaterialUI::SelectShader(DWORD_PTR _data)
{
    string strKey = (char*)_data;
    Ptr<CGraphicsShader> pShader = CResMgr::GetInst()->FindRes<CGraphicsShader>(wstring(strKey.begin(), strKey.end()));
    ((CMaterial*)GetTargetRes().Get())->SetShader(pShader);
}
