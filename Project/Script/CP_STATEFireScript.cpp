#include "pch.h"
#include "CP_STATEFireScript.h"
#include "CP_FSMScript.h"


CP_STATEFireScript::CP_STATEFireScript()
{
	SetStateType(static_cast<UINT>(eP_States::FIRE));
}

CP_STATEFireScript::~CP_STATEFireScript()
{
}

void CP_STATEFireScript::tick()
{
}

void CP_STATEFireScript::Enter()
{
	CParticleSystem* particle = m_PHQ->GetBullet()->ParticleSystem();
	tParticleModule ModuleData = particle->GetModuleData();
	ModuleData.bDead = false;
	particle->SetModuleData(ModuleData);
	particle->ActiveParticle();
	particle = m_PHQ->GetMuzzelFlash()->ParticleSystem();	
	ModuleData = particle->GetModuleData();
	ModuleData.bDead = false;
	particle->SetModuleData(ModuleData);
	particle->ActiveParticle();
	// ��� ������ �����ͼ� �ʿ��Ѱ� �������ֱ�
	//tParticleModule ModuleData = particle->GetModuleData();
	//ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
	//ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::GRAVITY] = false;
	//ModuleData.bDead = false;											// �̰� ���� true ���� Ȯ���ϱ�
	//particle->SetModuleData(ModuleData);

}

void CP_STATEFireScript::Exit()
{
}
