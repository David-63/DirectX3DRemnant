#include "pch.h"
#include "RigidBodyUI.h"


RigidBodyUI::RigidBodyUI() : ComponentUI("##RigidBodyUI", COMPONENT_TYPE::RIGIDBODY)
{
	SetName("RigidBodyUI");
}

RigidBodyUI::~RigidBodyUI()
{
}

int RigidBodyUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	//actor Type
	{
		UINT curIdx = (UINT)GetTarget()->RigidBody()->GetActorType();

		ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "Actor Type");

		const char* ActorTypes[3] = { "Static", "Dynamic", "Kinetic" };


		if (ImGui::BeginCombo("Select", ActorTypes[curIdx]))
		{
			for (int idx = 0; idx < IM_ARRAYSIZE(ActorTypes); idx++)
			{
				const bool is_selected = (curIdx == idx);
				if (ImGui::Selectable(ActorTypes[idx], is_selected))
				{
					curIdx = idx;
				}

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::SameLine();
		if (ImGui::Button("##ActorTypeSelectBtn", ImVec2(18, 18)))
		{
			GetTarget()->RigidBody()->SetActorType((ACTOR_TYPE)curIdx);
		}

		ImGui::Separator();
	}
	if (ImGui::TreeNode("Create Shape"))
	{
		bool isSphere = GetTarget()->RigidBody()->GetTempGeomType();
		bool isBox = !isSphere;
		ImGui::Checkbox("Sphere", &isSphere);
		ImGui::SameLine();
		ImGui::Checkbox("Box", &isBox);
		GetTarget()->RigidBody()->SetTempGeomType(isSphere);

		Vec3 shapeSize = GetTarget()->RigidBody()->GetTempSize();
		Vec3 shapeOffset = GetTarget()->RigidBody()->GetTempOffset();

		ImGui::Text("Size  ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Size", shapeSize);

		ImGui::Text("Offset");
		ImGui::SameLine();
		ImGui::DragFloat3("##Offset", shapeOffset);

		GetTarget()->RigidBody()->SetTempSize(shapeSize);
		GetTarget()->RigidBody()->SetTempOffset(shapeOffset);

		if (ImGui::Button("PushBack ShapeInfo"))
		{
			tShapeInfo info = {};
			if (isSphere)
				info.eGeomType = GEOMETRY_TYPE::Sphere;
			else
				info.eGeomType = GEOMETRY_TYPE::Box;

			info.size = shapeSize;
			info.offset = shapeOffset;

			GetTarget()->RigidBody()->GetShapeInfos()->push_back(info);
		}

		ImGui::TreePop();
	}

	//Shape
	{
		std::vector<tShapeInfo>* infoVec = GetTarget()->RigidBody()->GetShapeInfos();

		ImGui::TextColored(ImVec4(0.10f, 0.80f, 0.30f, 1.0f), "ShapeInfo");
		const char* GeomTypes[2] = { "Sphere", "Box" };

		int shapeCount = infoVec->size();

		ImGui::Text("Shape Count");
		ImGui::SameLine();
		ImGui::DragInt("##ShapeCount", &shapeCount);
		ImGui::Separator();

		for (int i = 0; i < shapeCount; ++i)
		{
			UINT GeomIdx = (UINT)(*infoVec)[0].eGeomType;
			Vec3 offset = (*infoVec)[0].offset;
			Vec3 Size = (*infoVec)[0].size;

			ImGui::Text("Geom Type:");
			ImGui::SameLine();
			ImGui::Text(GeomTypes[GeomIdx]);
			ImGui::Text("Size  ");
			ImGui::SameLine();
			ImGui::DragFloat3("##Size", Size);
			ImGui::Text("Offset");
			ImGui::SameLine();
			ImGui::DragFloat3("##Offset", offset);

			ImGui::Separator();
		}
	}



	return 0;
}
