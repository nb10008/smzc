#include "StdAfx.h"
#include "ListBoxEx.h"

ListBoxEx::ListBoxEx(void)
{
}

ListBoxEx::~ListBoxEx(void)
{
}

VOID ListBoxEx::Render()
{
	if( !IsRefresh() )
		return GUIWnd::Render();

	// 渲染底图
	m_pCanvas->rc.SetLTWH(m_ptView, m_ptSize);
	m_pRender->Draw(m_pCanvas, m_pPic, m_dwPicColor, m_eAlphaWrite);

	// 画出选择棒
	for(INT nY=0, nRow = m_nCurrentTextRow; nY<m_ptSize.y; nY += m_nRowHeight, nRow++)
	{
		if( nRow == m_nCurSelectedRow )
		{
			m_pCanvas->rc.Set(m_ptView.x, m_ptView.y + nY, m_ptView.x + m_ptSize.x, 
				m_ptView.y + nY + m_nRowHeight);
			if( m_pCanvas->rc.bottom > m_ptView.y + m_ptSize.y )
				m_pCanvas->rc.bottom = m_ptView.y + m_ptSize.y;

			if( nRow == m_nCurSelectedRowWhenMouseDown )
				m_pRender->Draw(m_pCanvas, m_pSelectStickPic, m_dwSelectStickActiveColor, m_eAlphaWrite);
			else
				m_pRender->Draw(m_pCanvas, m_pSelectStickPic, m_dwSelectStickColor, m_eAlphaWrite);
		}
	}

	if( m_dwRowLineColor )	// 是否需要画出分界线
	{
		for(INT nCol=0, nWidth = 0; nCol<m_mapWidth.Size()-1; nCol++)	// 画竖线
		{
			nWidth += m_mapWidth.Peek(nCol);
			if( GT_INVALID != nWidth && nWidth < m_ptSize.x )
			{
				m_pCanvas->rc.Set(m_ptView.x + nWidth, m_ptView.y, m_ptView.x + nWidth, 
					m_ptView.y + m_ptSize.y);
				m_pRender->Line(m_pCanvas, m_dwRowLineColor);
			}
		}

		for(INT nY=m_nRowHeight; nY < m_ptSize.y; nY += m_nRowHeight)	// 横线
		{
			m_pCanvas->rc.Set(m_ptView.x, nY + m_ptView.y, m_ptView.x + m_ptSize.x, nY + m_ptView.y);
			m_pRender->Line(m_pCanvas, m_dwRowLineColor);
		}
	}

	// 显示listbox中的文本
	tagGUIListItem* pItem = NULL;
	INT nLeftX = m_rcText.left;
	for(INT nCol=0; nCol<m_mapWidth.Size(); nCol++)
	{
		INT nTextY = m_nCurrentTextRow; // 应该从哪一行文本开始显示
		for(INT nYOffset = m_rcText.top; 
			nYOffset < m_ptSize.y; nYOffset += m_nRowHeight, nTextY++)
		{
			DWORD dwPos = MAKELONG(nTextY, nCol);
			pItem = m_mapItem.Peek(dwPos);
			if( !P_VALID(pItem) )	// 当前单元格没有内容
				continue;

			INT nPicOffsetX = 0;
			if( P_VALID(pItem->pPic) )
			{
				m_pCanvas->rc.SetLTWH(m_ptView.x+nLeftX+m_ptPicOffset.x-m_rcText.left, 
					m_ptView.y+nYOffset+m_ptPicOffset.y-m_rcText.top, 
					m_ptPicSize.x, m_ptPicSize.y);
				m_pRender->Draw(m_pCanvas, pItem->pPic, pItem->dwPicColor, m_eAlphaWrite);
				nPicOffsetX = m_ptPicOffset.x + m_ptPicSize.x;
			}

			m_pCanvas->rc.left = m_ptView.x + nLeftX + nPicOffsetX;
			m_pCanvas->rc.right = m_pCanvas->rc.left + m_mapWidth.Peek(nCol) - m_rcText.left - nPicOffsetX;
			if( nCol == m_mapWidth.Size() - 1 ) // 最右边一列的列宽可以加大
				m_pCanvas->rc.right = m_ptView.x + m_ptSize.x;

			m_pCanvas->rc.top = m_ptView.y + nYOffset;
			m_pCanvas->rc.bottom = m_pCanvas->rc.top + m_nRowHeight - 2;

			if( m_pCanvas->rc.right > m_ptView.x + m_ptSize.x )
				m_pCanvas->rc.right = m_ptView.x + m_ptSize.x;

			if( m_pCanvas->rc.bottom > m_ptView.y + m_ptSize.y )
				m_pCanvas->rc.bottom = m_ptView.y + m_ptSize.y;

			m_pRender->Text(m_pCanvas, pItem->strText.c_str(), m_pFont, 
			pItem->dwColor, m_dwShadeColor, m_eTextAlign, m_bTextWriteAlpha);
		}

		nLeftX += m_mapWidth.Peek(nCol);
		if( GT_INVALID == nLeftX || nLeftX > m_ptSize.x )
			break;
	}

	// 调用基类渲染
	GUIWnd::Render();
}