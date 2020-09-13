/********************************************************************
	created:	2010/01/10
	created:	10:1:2010   20:09
	filename: 	e:\loong\trunk\LoongWorld\external_links_define.h
	file path:	e:\loong\trunk\LoongWorld
	file base:	external_links_define
	file ext:	h
	author:		Jason
	
	purpose:	外部链接结构定义
*********************************************************************/

#ifndef external_links_define_h__
#define external_links_define_h__

struct tagSExternalLink
{
	tstring LinkName;
	tstring LinkURL;
	tagSExternalLink(){}
	tagSExternalLink(int){}
	tagSExternalLink(const tagSExternalLink & t)
		:LinkURL(t.LinkURL),LinkName(t.LinkName)
	{
	}
	tagSExternalLink & operator = (const tagSExternalLink & t)
	{
		LinkName	= t.LinkName;
		LinkURL		= t.LinkURL;
		return *this;
	}
};

#endif // external_links_define_h__

