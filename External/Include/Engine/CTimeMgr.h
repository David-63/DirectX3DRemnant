#pragma once

class CTimeMgr : public CSingleton<CTimeMgr>
{	
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llFrequency;

	UINT			m_iCallCount;
	float			m_fDeltaTime;
	float			m_fTime;
	float			m_fDeltaTime_Scaled;
	float			m_fTimeScale;

public:
	void init();
	void tick();
	void render();

public:
	float GetTimeScale() { return m_fTimeScale; }
	void SetTimeScale(float _scale) { m_fTimeScale = _scale; }
	float GetDeltaTime() { return m_fDeltaTime; }
	float GetScaledDeltaTime() { return m_fDeltaTime_Scaled; }
};

