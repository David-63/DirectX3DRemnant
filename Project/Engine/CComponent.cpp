#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _Type)
	: m_pOwner(nullptr)
	, m_Type(_Type)	
{
}

CComponent::CComponent(const CComponent& _Other)
	: CEntity(_Other)
	, m_pOwner(nullptr)
	, m_Type(_Other.m_Type)
{
}

CComponent::~CComponent()
{
}
