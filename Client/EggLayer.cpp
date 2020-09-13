#include "StdAfx.h"
#include "EggLayer.h"
#include "EggButton.h"
#include "IconStatic.h"

extern const TCHAR* szEggPic;

EggLayer::EggLayer(void)
{
	rectEmpty =  tagRect(666,130,724,192);
	rectNormalBreak =tagRect(666,195,724,257);
	rectColourBreak = tagRect(666,260,724,322);
	m_vecEgg.clear();
	m_vecEggBack.clear();
}

EggLayer::~EggLayer(void)
{
}

void EggLayer::SetEnable(bool bEnable)
{
	if (m_vecEgg.empty())
		return;

	for (vector<EggButton*>::iterator it = m_vecEgg.begin();
		it != m_vecEgg.end(); ++it)
	{
		(*it)->SetEnable(bEnable);
		(*it)->SetRefresh(TRUE);
	}
}

void EggLayer::init(EEggLayerNum layer)
{
	m_nLayer = layer;
	int tempPos = 0;
	for (vector<EggButton*>::iterator it = m_vecEgg.begin();
		it != m_vecEgg.end(); ++it)
	{
		(*it)->InitEggData(m_nLayer,tempPos);
		tempPos++;
	}
}

void EggLayer::pushback(EggButton* pBn,IconStatic* pSt)
{
	m_vecEgg.push_back(pBn);
	m_vecEggBack.push_back(pSt);
}

void EggLayer::clear()
{
	for (vector<EggButton*>::iterator it = m_vecEgg.begin();
		it != m_vecEgg.end(); ++it)
	{
		(*it)->Change2NormalEgg();
		(*it)->SetInvisible(FALSE);
	}

 	for (vector<IconStatic*>::iterator it = m_vecEggBack.begin();
 		it != m_vecEggBack.end(); ++it)
 	{
 		(*it)->SetInvisible(TRUE);
 	}
}

void EggLayer::SetColourEgg(EEggPosNum pos)
{
	if (m_vecEgg.size() < pos)
		return;
	m_vecEgg[pos]->Change2ColuorEgg();
}

void EggLayer::SetNormalEgg(EEggPosNum pos)
{
	if (m_vecEgg.size() < pos)
		return;
	m_vecEgg[pos]->Change2NormalEgg();
}

void EggLayer::SetBrokedEgg(EEggPosNum pos ,bool bEmpty/*=false*/,bool bColour/*=false*/)
{
	if (m_vecEgg.size() < pos)
		return;
	m_vecEgg[pos]->SetInvisible(TRUE);
	if (bEmpty)
		m_vecEggBack[pos]->SetPic(szEggPic,rectEmpty);
	else
	{
		if (bColour)
			m_vecEggBack[pos]->SetPic(szEggPic,rectColourBreak);
		else
			m_vecEggBack[pos]->SetPic(szEggPic,rectNormalBreak);
	}
		
	m_vecEggBack[pos]->SetInvisible(FALSE);
}