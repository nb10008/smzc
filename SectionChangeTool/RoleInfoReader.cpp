#include "StdAfx.h"
#include "RoleInfoReader.h"

CRoleInfoReader::CRoleInfoReader(void)
{
}

CRoleInfoReader::~CRoleInfoReader(void)
{
}

BOOL CRoleInfoReader::Load( LPCTSTR szFileName, TRoleMap &mapRoleMap )
{
	XmlDocument doc;

	if ( !doc.LoadFile(NULL, szFileName) )
	{
		return FALSE;
	}

	XmlHandle docHandle( &doc );
	XmlHandle eleHandle = docHandle.FirstChildElement("root").FirstChildElement();
	XmlElement* pElement = eleHandle.Element();

	if( !pElement )	// 是否符合存盘格式
		return FALSE;

	while( pElement )	// 读入所有element
	{
		tagRole* roleInfoTemp = new tagRole();
		XmlAttribute* pAttrib = pElement->FirstAttribute();
		
		// 遍历此记录中的所有属性
		if( pAttrib->ValueStr().empty() )	// 如果此属性没有值则跳过
		{
			return FALSE;
		}

		roleInfoTemp->dwSectionID = strtoul(pAttrib->Value(), NULL, 10);

		pAttrib = pAttrib->Next();
		roleInfoTemp->dwWorldID = strtoul( pAttrib->Value(), NULL, 10 );

		pAttrib = pAttrib->Next();
		m_pUtil->Unicode8ToUnicode( pAttrib->Value(), roleInfoTemp->szAccoutName );

		pAttrib = pAttrib->Next();
		m_pUtil->Unicode8ToUnicode( pAttrib->Value(), roleInfoTemp->szRoleName );

		mapRoleMap.Add( mapRoleMap.Size(), roleInfoTemp );
		pElement = pElement->NextSiblingElement();
	}

	return TRUE;
}