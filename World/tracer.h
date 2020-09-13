#pragma once

class Tracer
{
	BOOL m_bOn;
	Tracer();
public:
	static Tracer& getInst();
	BOOL IsOn()const { return m_bOn; }
	void TurnOn(BOOL bOn = TRUE) { InterlockedExchange((LPLONG)&m_bOn, bOn); }
	void Trace(const CHAR* pFromat,...);
	void Trace(const WCHAR* pFromat,...);
};

extern void RegisterSysTracerCmd();

#define TheTracer							(Tracer::getInst())
#define ITRACE								TheTracer.Trace