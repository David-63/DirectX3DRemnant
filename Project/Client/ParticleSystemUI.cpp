#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine/CParticleSystem.h>
#include <Engine/CMaterial.h>
#include <Engine/CMesh.h>
#include <Engine/CTexture.h>
#include <Engine\CResMgr.h>
#include <Engine\CTimeMgr.h>
#include "ImGuiMgr.h"
#include "ListUI.h"



ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("##ParticleSystem", COMPONENT_TYPE::PARTICLESYSTEM)
	, m_Particle(nullptr)
	, m_ParticleTex(nullptr)
	, m_NoiseTex(nullptr)
	, m_AccTime(0.0f)
	, m_fDebugTime(0.0f)

{
	SetName("ParticleSystem");
}

ParticleSystemUI::~ParticleSystemUI()
{
}


int ParticleSystemUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;


	m_Particle = GetTarget()->ParticleSystem();

	Ptr<CMaterial> pMaterial = m_Particle->GetMaterial(0);
	Ptr<CMesh> pMesh = m_Particle->GetMesh();
	tParticleModule tParticleData = m_Particle->GetModuleData();

	// ==== Mesh
	char szBuff[256] = {};
	ImGui::Text("Mesh   ");
	ImGui::SameLine();
	GetResKey(pMesh.Get(), szBuff, 50);

	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##MeshName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();

	if (ImGui::Button("Mesh Select", ImVec2(150, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapMesh =
			CResMgr::GetInst()->GetResources(RES_TYPE::MESH);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Mesh List", ImVec2(300.f, 500.f));

		for (const auto& pair : mapMesh)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&ParticleSystemUI::SelectMesh);

	}

	// ===== Material
	ImGui::Text("Material");
	ImGui::SameLine();

	GetResKey(pMaterial.Get(), szBuff, 50);
	ImGui::SetNextItemWidth(150.f);

	ImGui::InputText("##MtrlName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();

	if (ImGui::Button("##Mtrl Select", ImVec2(150, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapMtrl =
			CResMgr::GetInst()->GetResources(RES_TYPE::MATERIAL);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Material", ImVec2(300.f, 500.f));

		for (const auto& pair : mapMtrl)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&ParticleSystemUI::SelectMaterial);

	}

	bool bFoward = m_Particle->IsFoward();

	// ==== 재질이 파티클인지 아닌지 
	ImGui::Text("Is FowardRendering?");
	ImGui::SameLine();
	if (ImGui::Checkbox("##FowardOrNo", &bFoward))
	{
		m_Particle->SetFoward(bFoward);

	}


	// ===== Texture
	ImGui::Text("Texture");
	ImGui::SameLine();

	GetResKey(m_ParticleTex.Get(), szBuff, 50);

	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##Tex Name", szBuff, 50, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();

	if (ImGui::Button("##Tex Select", ImVec2(150, 18)))
	{
		const map<wstring, Ptr<CRes>>& mapTex =
			CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Texture", ImVec2(300.f, 500.f));

		for (const auto& pair : mapTex)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택시 호출받을 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&ParticleSystemUI::SelectTexture);


	}

	if (nullptr != m_ParticleTex)
	{
		ID3D11ShaderResourceView* TexView = (m_ParticleTex->GetSRV().Get());
		ImGui::Image(TexView, ImVec2(100.f, 100.f));
	}





	// ===== Module Data
	ImGui::Text("Particle Module");

	ImGui::Separator();
	if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
	{
		// ==============
		// PARTICLE_SPAWN
		// ==============
		if (ImGui::BeginTabItem("PARTICLE_SPAWN"))
		{
			// Default Option
			if (ImGui::TreeNodeEx("Default Options", ImGuiTreeNodeFlags_Selected))
			{
				// ===== 파티클 만들 때는 아래 코드로 사용할것 				 				
				// Module On/Off
				//bool SpawnOnOff = tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN];
				//ImGui::Text("Spawn Module ON/OFF");
				//ImGui::SameLine();
				//if (ImGui::Checkbox("##ON/OFF", &SpawnOnOff))
				//{
				//	tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = SpawnOnOff;


				//}




				// ==== 파티클 다 만들었으면 아래 코드로 사용할 것
				// Spawn Loop

				bool bLoop = m_Particle->GetLoop();

				ImGui::Text("Spawn Loop On/Off");
				ImGui::SameLine();
				if (ImGui::Checkbox("##Loop_OnOff", &bLoop))
				{
					m_Particle->SetLoop(bLoop);

				}



				// Module On/Off
				bool SpawnOnOff = tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN];
				ImGui::Text("Spawn Module ON/OFF");
				ImGui::SameLine();
				if (ImGui::Checkbox("##ON/OFF", &SpawnOnOff))
				{
					tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = SpawnOnOff;
				}
				ImGui::Separator();


				if (SpawnOnOff)
				{
					m_AccTime += (float)DT;
				}

				else
				{
					m_fDebugTime = m_AccTime;
				}


				ImGui::Text("For Debuging");

				ImGui::Text("AccDuringTime : %.2f", m_AccTime); // 누적되는 시간
				ImGui::SameLine();

				if (ImGui::Button("  Time Reset", ImVec2(180, 18)))
				{
					m_AccTime = 0.0f;
				}

				ImGui::Separator();


				ImGui::Text("For Use");

				ImGui::Text("User Set Spawn Time : %.2f", m_Particle->GetSpawnTime());
				ImGui::SameLine();
				ImGui::Separator();


				//bool useTimeSpawn = m_Particle->GetTimeSpawn();
				//ImGui::Text("Use Time Spawn");
				//ImGui::SameLine();
				//if (ImGui::Checkbox("##UseTimespawn", &useTimeSpawn))
				//{
				//	m_Particle->SetTimeSpawn(useTimeSpawn);
				//}
				//ImGui::Separator();


				// ================================ 아래는 만지지 말것 

				// bursts 기능을 켤지 말지 (입자가 한꺼번에 나가게 할지 말지)
				bool bBursts = m_Particle->GetBursts();

				ImGui::Separator();
				ImGui::Text("Bursts Mode ON/OFF");
				ImGui::SameLine();
				if (ImGui::Checkbox("##Bursts ON/OFF", &bBursts))
				{
					m_Particle->SetBursts(bBursts);
				}

				ImGui::SameLine();
				//ImGui::SetNextItemWidth(200);


				// == 최대 파티클 갯수 
				int	Origin = tParticleData.iMaxParticleCount;

				ImGui::Text("MaxParitcleCount ");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(50);
				if (ImGui::DragInt("##MaxCount_", &Origin, 1.f, 0.0f, Origin, "%d", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.iMaxParticleCount = Origin;
				}

				bool Reset = false;

				ImGui::SameLine();

				ImGui::Text("   Reset Count");

				ImGui::SameLine();
				UINT OriginCount = m_Particle->GetParticleBuffer()->GetElementCount();

				if (ImGui::Checkbox("##Reset Count", &Reset))
				{
					tParticleData.iMaxParticleCount = OriginCount;

				}



				// == 초당 생성 개수
				ImGui::Separator();
				int	SpawnRate = tParticleData.SpawnRate;
				ImGui::Text("SpawnCount Per Sec   ");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(110);
				if (ImGui::DragInt("##SpawnRate", &SpawnRate, 0.25f, 0, tParticleData.iMaxParticleCount, "%d", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.SpawnRate = SpawnRate;
				}



				// 파티클 업데이트 좌표계 ( 0 : World,  1 : Local)
				int item_current_idx = tParticleData.Space;
				const char* items[2] = { "World" , "Local" };
				ImGui::Text("Update Space");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(110);
				if (ImGui::BeginCombo("##combo", items[item_current_idx]))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(items[n], is_selected))
							item_current_idx = n;

						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					tParticleData.Space = item_current_idx;

					ImGui::EndCombo();
				}


				// 수명
				float MinLifeTime = tParticleData.MinLifeTime;
				float MaxLifeTime = tParticleData.MaxLifeTime;
				ImGui::Text("LifeTime Per Particle   ");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(150);
				if (ImGui::DragFloatRange2("##LifeTime", &MinLifeTime, &MaxLifeTime, 0.25f, 0.0f, 100.0f, "Min: %.1f", "Max: %.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.MinLifeTime = MinLifeTime;
					tParticleData.MaxLifeTime = MaxLifeTime;
				}


				ImGui::TreePop();
			}

			// SpawnColor
			if (ImGui::TreeNodeEx("SpwanColor", ImGuiTreeNodeFlags_Selected))
			{

				bool UseStrongColor = tParticleData.bStrongColor;

				if (ImGui::Checkbox("UseStrongColor", &UseStrongColor))
				{
					tParticleData.bStrongColor = UseStrongColor;

				}

				ImGui::NewLine();

				Vec4 SpawnColor = tParticleData.vSpawnColor;
				ImVec4 color = ImVec4(SpawnColor.x, SpawnColor.y, SpawnColor.z, SpawnColor.w);
				ImGuiColorEditFlags flags = ImGuiColorEditFlags_::ImGuiColorEditFlags_DefaultOptions_;
				if (ImGui::ColorPicker4("Cur Color", (float*)&color, flags, NULL))
				{
					SpawnColor = Vec4(color.x, color.y, color.z, color.w);
					tParticleData.vSpawnColor = SpawnColor;

				}

				ImGui::TreePop();
			}


			// SpawnScaleMin/Max
			if (ImGui::TreeNodeEx("SpawnScale", ImGuiTreeNodeFlags_Selected))
			{
				ImGui::Text(" X");
				ImGui::SameLine();
				float beginX = tParticleData.vSpawnScaleMin.x;
				float endX = tParticleData.vSpawnScaleMax.x;
				if (ImGui::DragFloatRange2("##ScaleX", &beginX, &endX, 0.25f, 0.0f, 100.0f, "Min: %.1f", "Max: %.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.vSpawnScaleMin.x = beginX;
					tParticleData.vSpawnScaleMax.x = endX;
				}

				ImGui::Text(" Y");
				ImGui::SameLine();
				float beginY = tParticleData.vSpawnScaleMin.y;
				float endY = tParticleData.vSpawnScaleMax.y;
				if (ImGui::DragFloatRange2("##ScaleY", &beginY, &endY, 0.25f, 0.0f, 100.0f, "Min: %.1f", "Max: %.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.vSpawnScaleMin.y = beginY;
					tParticleData.vSpawnScaleMax.y = endY;
				}

				ImGui::Text(" Z");
				ImGui::SameLine();
				float beginZ = tParticleData.vSpawnScaleMin.z;
				float endZ = tParticleData.vSpawnScaleMax.z;
				if (ImGui::DragFloatRange2("##ScaleZ", &beginZ, &endZ, 0.25f, 0.0f, 100.0f, "Min: %.1f", "Max: %.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.vSpawnScaleMin.z = beginZ;
					tParticleData.vSpawnScaleMax.z = endZ;
				}

				ImGui::TreePop();
			}


			// SpawnShapeType
			if (ImGui::TreeNodeEx("SpawnShapeType", ImGuiTreeNodeFlags_Selected))
			{
				int item_current_idx = tParticleData.SpawnShapeType;
				const char* items[2] = { "Box" , "Sphere" };
				ImGui::Text("Shape Type ");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(110);
				if (ImGui::BeginCombo("##combo", items[item_current_idx]))
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(items[n], is_selected))
							item_current_idx = n;

						if (is_selected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					tParticleData.SpawnShapeType = item_current_idx;

					ImGui::EndCombo();
				}

				ImGui::Text("Shape Scale");
				ImGui::SameLine();

				// ===== Box
				if (0 == tParticleData.SpawnShapeType)
				{
					// BoxShapeScale
					float Scale[3] = { tParticleData.vBoxShapeScale.x, tParticleData.vBoxShapeScale.y, tParticleData.vBoxShapeScale.z };
					ImGui::SetNextItemWidth(110);
					if (ImGui::DragFloat2("##Scale", Scale, 0.25f, 0.0f, 3000.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
					{
						tParticleData.vBoxShapeScale.x = Scale[0];
						tParticleData.vBoxShapeScale.y = Scale[1];
						tParticleData.vBoxShapeScale.z = Scale[2];

					}

					// SpawnAreaOffsetFactor (0에 가까울수록 한 방향으로 향하는 경사진(cone-like 형태가 됨)
					float SpawnAreaOffsetFactor = tParticleData.fSpawnAreaOffsetFactor;
					ImGui::SetNextItemWidth(110);

					if (ImGui::DragFloat("BoxOffset", &SpawnAreaOffsetFactor, 0.03f, -0.5f, 1.5f, "%0.5f", ImGuiSliderFlags_AlwaysClamp))
					{
						// 0.5가 원 모양으로 퍼짐 
						tParticleData.fSpawnAreaOffsetFactor = SpawnAreaOffsetFactor;
					}



					bool UseSpark = tParticleData.bUseSpark;

					if (ImGui::Checkbox("Use Spark", &UseSpark))
					{
						tParticleData.bUseSpark = UseSpark;
					}



				}

				//// ==== Sphere (굳이 사용 안해도 될 것 같아서 뺌)
				//else if (1 == tParticleData.SpawnShapeType)
				//{
				//	//SphereShapeRadius
				//	float SphereShapeRadius = tParticleData.fSphereShapeRadius;
				//	ImGui::SetNextItemWidth(110);
				//	if (ImGui::DragFloat("##Scale", &SphereShapeRadius, 0.25f, 0.0f, 3000.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				//	{
				//		tParticleData.fSphereShapeRadius = SphereShapeRadius;
				//	}

				//	ImGui::Text("Sphere Offset");
				//	ImGui::SameLine();
				//	float SphereOffset = tParticleData.fSphereOffset;

				//	if (ImGui::DragFloat("##SphereOffset", &SphereOffset, 0.25f, -50.0f, 3000.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				//	{
				//		tParticleData.fSphereOffset = SphereOffset;
				//	}
				//}


				ImGui::TreePop();
			}


			ImGui::EndTabItem();
		}


		// ============
		// COLOR_CHANGE
		// ============
		if (ImGui::BeginTabItem("COLOR_CHANGE"))
		{
			bool UseStrongColor = tParticleData.bStrongColor;

			if (ImGui::Checkbox("UseStrongColor", &UseStrongColor))
			{
				tParticleData.bStrongColor = UseStrongColor;

			}

			ImGui::NewLine();

			// On/Off
			bool OnOff = tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE];
			ImGui::Text("Color Change Module ON/OFF");
			ImGui::SameLine();
			if (ImGui::Checkbox("##ON/OFF", &OnOff))
			{
				tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = OnOff;
			}

			// StartColor
			ImGui::Text("Start Color");
			Vec4 StartColor = tParticleData.vStartColor;
			ImVec4 sColor = ImVec4(StartColor.x, StartColor.y, StartColor.z, StartColor.w);
			ImGuiColorEditFlags flags = ImGuiColorEditFlags_::ImGuiColorEditFlags_DefaultOptions_;
			if (ImGui::ColorPicker4("Current Color##1", (float*)&sColor, flags, NULL))
			{
				StartColor = Vec4(sColor.x, sColor.y, sColor.z, sColor.w);
				tParticleData.vStartColor = StartColor;
			}


			// EndColor
			ImGui::Text("End Color");
			Vec4 EndColor = tParticleData.vEndColor;
			ImVec4 eColor = ImVec4(EndColor.x, EndColor.y, EndColor.z, EndColor.w);
			if (ImGui::ColorPicker4("Current Color##2", (float*)&eColor, flags, NULL))
			{
				EndColor = Vec4(eColor.x, eColor.y, eColor.z, eColor.w);
				tParticleData.vEndColor = EndColor;
			}

			ImGui::EndTabItem();
		}


		// ============
		// SCALE_CHANGE
		// ============
		if (ImGui::BeginTabItem("SCALE_CHANGE"))
		{
			// On/Off
			bool OnOff = tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE];
			ImGui::Text("Scale Change Module ON/OFF");
			ImGui::SameLine();
			if (ImGui::Checkbox("##ON/OFF", &OnOff))
			{
				tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = OnOff;
			}

			// 초기 배율 ~ 최종 배율
			ImGui::Text("Start Ratio");
			ImGui::SameLine();
			float Start = tParticleData.StartScale;
			ImGui::SetNextItemWidth(110);
			if (ImGui::DragFloat("##Start", &Start, 0.25f, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
			{
				tParticleData.StartScale = Start;
			}

			ImGui::Text("End  Ratio ");
			ImGui::SameLine();
			float End = tParticleData.EndScale;
			ImGui::SetNextItemWidth(110);
			if (ImGui::DragFloat("##End", &End, 0.25f, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
			{
				tParticleData.EndScale = End;
			}

			ImGui::EndTabItem();
		}


		// ============
		// ADD_VELOCITY
		// ============
		if (ImGui::BeginTabItem("ADD_VELOCITY"))
		{
			// ===== 아래 기능은 필수여야해서 뺌 
			//// On/Off
			//bool OnOff = tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY];
			//ImGui::Text("Add Velocity Module ON/OFF");
			//ImGui::SameLine();
			//if (ImGui::Checkbox("##ON/OFF", &OnOff))
			//{
			//	tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = OnOff;
			//}


			// Speed (필수 옵션)
			ImGui::Text("Speed");
			ImGui::SameLine();
			float Speed = tParticleData.Speed;
			ImGui::SetNextItemWidth(110);
			if (ImGui::DragFloat("##Speed", &Speed, 0.25f, 1.f, 10000.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
			{
				tParticleData.Speed = Speed;
			}



			// AddVelocityType (0 : From Center, 1: To Center, 2 : Fixed Direction)
			int item_current_idx = tParticleData.AddVelocityType;
			const char* items[3] = { "From Center" , "To Center" , "Fixed Direction" };
			ImGui::Text("Type ");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(110);
			if (ImGui::BeginCombo("##combo", items[item_current_idx]))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				tParticleData.AddVelocityType = item_current_idx;

				ImGui::EndCombo();
			}



			// ********* 여기 수정해서 넣기
			if (0 == tParticleData.AddVelocityType)
			{
				ImGui::Text("From Center Option");
			}

			if (1 == tParticleData.AddVelocityType)
			{
				ImGui::Text("To Center Option");
			}

			// VelocityDir, OffsetAngle (Fixed Direction 옵션)

			if (2 == tParticleData.AddVelocityType)
			{
				ImGui::Text("!!Only Fixed Direction Options!!");

				// VelocityDir
				ImGui::Text("Dir  ");
				ImGui::SameLine();
				float Dir[2] = { tParticleData.vVelocityDir.x, tParticleData.vVelocityDir.y };
				ImGui::SetNextItemWidth(110);
				if (ImGui::DragFloat2("##Dir", Dir, 0.25f, -1.0f, 1.f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.vVelocityDir = Vec4(Dir[0], Dir[1], tParticleData.vVelocityDir.z, tParticleData.vVelocityDir.w);
				}

				// OffsetAngle
				ImGui::Text("Angle");
				ImGui::SameLine();
				float Angle = tParticleData.OffsetAngle;
				ImGui::SetNextItemWidth(110);
				if (ImGui::DragFloat("##Angle", &Angle, 0.25f, -360.0f, 360.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.OffsetAngle = Angle;
				}
			}


			ImGui::EndTabItem();
		}


		// ====
		// DRAG
		// ====
		if (ImGui::BeginTabItem("DRAG"))
		{
			// On/Off
			bool OnOff = tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG];
			ImGui::Text("Drag Module ON/OFF");
			ImGui::SameLine();
			if (ImGui::Checkbox("##ON/OFF", &OnOff))
			{
				tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = OnOff;
			}

			// 제한 속도 (초기 속도 ~ 최종 속도)
			ImGui::Text("Start Speed");
			ImGui::SameLine();
			float Start = tParticleData.StartDrag;
			ImGui::SetNextItemWidth(110);
			if (ImGui::DragFloat("##Start", &Start, 0.25f, -1000.0f, 1000.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
			{
				tParticleData.StartDrag = Start;
			}

			ImGui::Text("End  Speed ");
			ImGui::SameLine();
			float End = tParticleData.EndDrag;
			ImGui::SetNextItemWidth(110);
			if (ImGui::DragFloat("##End", &End, 0.25f, -1000.0f, 1000.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
			{
				tParticleData.EndDrag = End;
			}

			ImGui::EndTabItem();
		}

		// ======
		// GRAVITY
		// ======
		if (ImGui::BeginTabItem("GRAVITY"))
		{
			// On / Off
			bool OnOff = tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::GRAVITY];
			ImGui::Text("Gravity ON/OFF");
			ImGui::SameLine();
			if (ImGui::Checkbox("##ON/OFF", &OnOff))
			{
				tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::GRAVITY] = OnOff;
			}


			// ======= Gravity Force
			ImGui::Text("Gravity Force");
			ImGui::SameLine();

			float GravityForce = tParticleData.fGravityForce;
			ImGui::SetNextItemWidth(110);

			if (ImGui::DragFloat("##GravityForce", &GravityForce, 0.25f, -1000.0f, 1000.0f,
				"%.1f", ImGuiSliderFlags_AlwaysClamp))
			{
				tParticleData.fGravityForce = GravityForce;
			}


			ImGui::EndTabItem();
		}



		// ===========
		// NOISE_FORCE
		// ===========
		if (ImGui::BeginTabItem("NOISE_FORCE"))
		{
			// On/Off
			bool OnOff = tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE];
			ImGui::Text("Noise Force Module ON/OFF");
			ImGui::SameLine();
			if (ImGui::Checkbox("##ON/OFF", &OnOff))
			{
				tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = OnOff;
			}

			if (OnOff)
			{
				// ===== 노이즈 이미지 세팅 (**Noise폴더 안의 이미지만 목록에 뜨도록 바꾸는 법도 생각해보기)
				ImGui::Text("Noise Image");
				ImGui::SameLine();

				GetResKey(m_NoiseTex.Get(), szBuff, 150);

				ImGui::SetNextItemWidth(200.f);
				ImGui::InputText("##Tex Name", szBuff, 360, ImGuiInputTextFlags_ReadOnly);

				if (ImGui::Button("Select", ImVec2(50.f, 18)))
				{
					const map<wstring, Ptr<CRes>>& mapTex =
						CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

					ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
					pListUI->Reset("Textrue", ImVec2(300.f, 500.f));

					for (const auto& pair : mapTex)
					{
						pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
					}

					pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&ParticleSystemUI::SelectNoiseTex);
				}

				if (nullptr != m_NoiseTex)
				{
					ID3D11ShaderResourceView* TexView = (m_NoiseTex->GetSRV().Get());
					ImGui::Image(TexView, ImVec2(100.f, 100.f));
				}




				// 랜덤 힘 변경 간격
				ImGui::Text("Noise Term ");
				ImGui::SameLine();
				float NoiseTerm = tParticleData.fNoiseTerm;
				ImGui::SetNextItemWidth(110);
				if (ImGui::DragFloat("##NoiseTerm", &NoiseTerm, 0.25f, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.fNoiseTerm = NoiseTerm;
				}


				// 랜덤 힘 크기
				ImGui::Text("Noise Force");
				ImGui::SameLine();
				float NoiseForce = tParticleData.fNoiseForce;
				ImGui::SetNextItemWidth(110);
				if (ImGui::DragFloat("##NoiseForce", &NoiseForce, 0.25f, 0.0f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.fNoiseForce = NoiseForce;
				}
			}

			ImGui::EndTabItem();
		}


		// ======
		// RENDER
		// ======
		if (ImGui::BeginTabItem("RENDER"))
		{
			// On/Off
			bool OnOff = tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER];
			ImGui::Text("Render Module ON/OFF");
			ImGui::SameLine();
			if (ImGui::Checkbox("##ON/OFF", &OnOff))
			{
				tParticleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = OnOff;
			}

			// ======= 속도

			// 1 : 속도정렬 사용(이동 방향으로 회전) 0 : 사용 안함
			bool Alignment = tParticleData.VelocityAlignment;
			ImGui::Text("Velocity Alignment ON/OFF    ");
			ImGui::SameLine();
			if (ImGui::Checkbox("##Alignment", &Alignment))
			{
				tParticleData.VelocityAlignment = Alignment;
			}

			// 1 : 속도에 따른 크기 변화 사용, 0 : 사용 안함
			bool Scale = tParticleData.VelocityScale;
			ImGui::Text("Scale Change ON/OFF ");
			ImGui::SameLine();
			if (ImGui::Checkbox("##ScaleChange", &Scale))
			{
				tParticleData.VelocityScale = Scale;
			}

			// 최대 크기에 도달하는 속력 & 속력에 따른 크기 변화량 최대치
			if (Scale)
			{
				// 최대 크기에 도달하는 속력
				ImGui::Text("Max Speed(max scale reached)");
				ImGui::SameLine();
				float Speed = tParticleData.vMaxSpeed;
				ImGui::SetNextItemWidth(65);
				if (ImGui::DragFloat("##MaxSpeed", &Speed, 0.25f, -1000.0f, 1000.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.vMaxSpeed = Speed;
				}

				// 속력에 따른 크기 변화량 최대치
				ImGui::Text("Max Scale Ratio X, Y(at the speed)");
				float Scale[2] = { tParticleData.vMaxVelocityScale.x, tParticleData.vMaxVelocityScale.y };
				if (ImGui::DragFloat2("##MaxScale", Scale, 0.25f, 1.f, 100.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.vMaxVelocityScale.x = Scale[0];
					tParticleData.vMaxVelocityScale.y = Scale[1];
				}
			}



			// ========== 회전
			bool RotOnOff = tParticleData.bRot;

			ImGui::Text("Rotation Module ON/OFF");
			ImGui::SameLine();

			if (ImGui::Checkbox("##RotModule", &RotOnOff))
			{
				tParticleData.bRot = RotOnOff;
			}

			if (RotOnOff)
			{
				ImGui::Text("Rotation Angle");
				float Angle = tParticleData.fRotAngle;

				if (ImGui::DragFloat("##RotAngle", &Angle, 0.25f, 0.0f, 360.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.fRotAngle = Angle;
				}

				ImGui::Text("Rotation Speed");
				float Speed = tParticleData.fRotSpeed;

				if (ImGui::DragFloat("##RotSpeed", &Speed, 0.25f, 0.0f, 360.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				{
					tParticleData.fRotSpeed = Speed;
				}
			}

			// ===== 애니메이션
			bool UseAnim = tParticleData.AnimUse;
			bool Loop = tParticleData.AnimLoop;

			ImGui::Text("AnimUse");
			if (ImGui::Checkbox("##AnimationUse", &UseAnim))
			{
				tParticleData.AnimUse = UseAnim;
			}

			if (UseAnim)
			{
				ImGui::Text("AnimLoop");
				if (ImGui::Checkbox("##AnimLooop", &Loop))
				{
					tParticleData.AnimLoop = Loop;
				}


				if (Loop)
				{
					ImGui::Text("AnimTime");
					ImGui::SameLine();
					float time = tParticleData.fAnimFrmTime;

					if (ImGui::DragFloat("##PrcLoopAnimTime", &time, 0.03f, 0.0f, 3.0f, "%0.5f", ImGuiSliderFlags_AlwaysClamp))
					{
						tParticleData.fAnimFrmTime = time;
					}
				}

				else
				{
					ImGui::Text("AnimSpeed");
					ImGui::SameLine();
					float ftime = tParticleData.fAnimSpeed;

					if (ImGui::DragFloat("##PrcAnimSpeed", &ftime, 0.01f, 0.0f, 3.0f, "%0.5f", ImGuiSliderFlags_AlwaysClamp))
					{
						tParticleData.fAnimSpeed = ftime;
					}
				}


				int AnimYCount = tParticleData.iAnimYCount;
				int AnimXCount = tParticleData.iAnimXCount;
				ImGui::Text("AnimXCount");
				ImGui::SameLine();
				ImGui::InputInt("##AnimXCount", &AnimXCount);
				{
					tParticleData.iAnimXCount = AnimXCount;
				}

				ImGui::Text("AnimYCount");
				ImGui::SameLine();
				ImGui::InputInt("##AnimYCount", &AnimYCount);
				{
					tParticleData.iAnimYCount = AnimYCount;
				}
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	m_Particle->SetModuleData(tParticleData);



	return TRUE;
}

void ParticleSystemUI::SelectMesh(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	Ptr<CMesh> pMesh = CResMgr::GetInst()->
		FindRes<CMesh>(wstring(strKey.begin(), strKey.end()));
	m_Particle->SetMesh(pMesh);
}

void ParticleSystemUI::SelectMaterial(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	m_SelectMtrlName = ToWString(strKey);

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->
		FindRes<CMaterial>(wstring(strKey.begin(), strKey.end())); // material\\ 이런식으로 들어옴 

	// ** 현재 0번에만 있는데 파티클이 여러개일 경우도 한번 생각해보기
	m_Particle->SetMaterial(pMtrl, 0);

}

void ParticleSystemUI::SelectTexture(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	m_ParticleTex =
		CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));

	wstring wstrKey = wstring(strKey.begin(), strKey.end());

	m_Particle->SetTexName(wstrKey);
	m_Particle->SetParticleTex(m_ParticleTex);


	// 현재 0번만 고려 중 
   // m_Particle->GetMaterial(0)->SetTexParam(TEX_0, m_ParticleTex)


}

void ParticleSystemUI::SelectNoiseTex(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;

	m_NoiseTex =
		CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));

	m_Particle->SetNoiseTex(m_NoiseTex);



}
