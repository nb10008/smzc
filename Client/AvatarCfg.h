#pragma once

/**
	class: AvatarCfg
	brief: 采用单键模式，由程序来指定模型、纹理帖图的路径
*/
#define AVATAR_HAIR_TYPE 8
#define AVATAR_HAIR_COLOR 5
#define AVATAR_FACE 10
#define AVATAR_FACE_DETAIL 7

#define AVATAR_HAIR_TYPE_EX 8
#define AVATAR_FACE_EX 10

#define AVATAR_DRESS 2          // 可选新手时装样式的数目

class AvatarCfg
{
public:
	AvatarCfg(void);
	~AvatarCfg(void);
	static AvatarCfg* Inst();

	VOID LoadFromFile();

	int GetNumHairType()const	{ return AVATAR_HAIR_TYPE; }
	int GetNumHairTypeEx()const	{ return AVATAR_HAIR_TYPE_EX; }
	int GetNumHairColor()const	{ return AVATAR_HAIR_COLOR; }
	int GetNumFace()const		{ return AVATAR_FACE; }
	int GetNumFaceEx()const		{ return AVATAR_FACE_EX; }
	int GetNumFaceDetail()const	{ return AVATAR_FACE_DETAIL; }
	int GetNumDress()const		{ return AVATAR_DRESS; }

	const TCHAR* GetHairTypeMdlPath(BYTE bySex, WORD id, BYTE byTransSkinIndex = 0);						//头发模型路径
	const TCHAR* GetHairColorTexPath(BYTE bySex, WORD hairID, WORD colorID, BYTE byTransSkinIndex = 0);	//头发纹理贴图路径
	const TCHAR* GetFaceMdlPath(BYTE bySex, WORD id, BYTE byTransSkinIndex = 0);							//面部模型路径
	const TCHAR* GetFaceDtlTexPath(BYTE bySex, WORD id);						//面部纹理贴图路径
	/** \获取内衣模型的路径
	*/
	const TCHAR* GetUpperUWMdlPath(BYTE bySex,BOOL bFashion, BYTE byTransSkinIndex = 0);					//上衣模型路径
	const TCHAR* GetLowerUWMdlPath(BYTE bySex,BOOL bFashion, BYTE byTransSkinIndex = 0);					//裤子模型路径
	const TCHAR* GetBootUWMdlPath(BYTE bySex,BOOL bFashion, BYTE byTransSkinIndex = 0);					//鞋模型路径	
	const TCHAR* GetGloveUWMdlPath(BYTE bySex,BOOL bRight,BOOL bFashion, BYTE byTransSkinIndex = 0);		//手套型路路径(分左右)
	const TCHAR* GetBackUWMdlPath(BYTE bySex, BYTE byTransSkinIndex = 0);		// 披风

	/** \根据性别及序号生成一套新手时装结构
	*/
	void MakeNewPlayerFahion( const BYTE bySex, const int n, tagAvatarEquip& out ) const;

	// 变身皮肤
	struct tagTransSkin
	{
		DWORD dwIndex;
		tstring strTSFace;
		tstring strTSHat;
		tstring strTSHair;
		tstring strTSHairColor;
		tstring strTSUpper;
		tstring strTSLower;
		tstring strTSGlove_r;
		tstring strTSGlove_l;
		tstring strTSBoot;
		tstring strTSMantle;
	};
private:
	tstring							m_strPath;
	map<DWORD, tagTransSkin>		m_mapTransSkin;
};
