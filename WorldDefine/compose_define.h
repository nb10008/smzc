#pragma once

#include "ItemDefine.h"
#include "suit_define.h"

#pragma pack(push, 1)

#define MAX_CONSOLIDATE_POS_QUANTITY 12
#define MAX_CONSOLIDATE_POS_HOLYEQUIP 3 //元神装备可强化位置个数
#define MAX_CONSOLIDATE_STUFF_QUANTITY 6
#define MAX_PRODUCE_STUFF_QUANTITY 6
#define MAX_CONSOLIDATE_ROLEATT 3
#define MAX_OUTPUT_QUANTITY	6
#define MAX_CASTING_ROLEATT	5
#define MAX_CASTING_QUANTITY 5
#define X_EXTRACT_MAX_PRODUCTION 6
#define MAX_EXTRACT_OUTPUT 6

//生成类型
enum EProduceType
{
	EPCT_NULL	= 0,
	EPCT_Artisan,			//工匠1
	EPCT_DanTraining,		//练丹2
	EPCT_Smith,				//锻造3
	EPCT_Casting,			//铸甲4
	EPCT_Dressmaker,		//裁缝5
	EPCT_Aechnics,			//工艺6
	EPCT_Smilt,				//冶练7
	EPCT_PointUp,			//点化8
	EPCT_GodArtisan,		//天匠9	add xtian 2009-4-13
	EPCT_Pet,				//灵兽10 add xtian 2009-8-21
	EPCT_EquipMaster,		// 装备匠师
	EPCT_EquipRefine,		// 装备萃取(F:魔匠)
	EPCT_StuffRefine,		// 材料凝炼（F:宝石雕琢）
	EPCT_TimeLimitEquip,	// 时限装备14(F:神匠)
	EPCT_Gem,				// 宝石工艺15
	EPCT_TreasureCom = 900,	// 宝物合成 
	EPCT_FoodGod1 = 20,	// 食神1，	我对策划心悦诚服					(食材加工)
	EPCT_FoodGod2 = 21,	// 食神2，	我对策划五体投地					(食物制作)
	EPCT_FoodGod3 = 22,	// 食神3		我对策划惊为天人					(圣诞武器)
	EPCT_Perfume =  23,	// 香水制作  我对策划无话可说，照做就行了 (灵魂融合)
	EPCT_Crystal = 24,	// 水晶合成
	EPCT_Stole_52 = 25, // 52等级圣衣
	EPCT_Stole_67 = 26, // 67等级圣衣
	EPCT_Stole_82 = 27, // 82等级圣衣
	EPCT_Stole_Blade = 28, // 神兵圣衣
	EPCT_Stole_Bow = 29, // 玄翎圣衣
	EPCT_Stole_Valor = 30, // 乾坤圣衣
	EPCT_Stole_Swing = 31, // 仙音圣衣
	EPCT_SoulLinker = 32, // 灵魂链
	EPCT_MoonCake = 33, // 月饼
	EPCT_HallowmasGift = 34, // 万圣节礼包
	EPCT_HallowmasName = 35, // 万圣节称号
	EPCT_MoShenEquip = 38,	//台湾魔神装备
	EPCT_FairySoulStren=39,// 妖精灵魂强化 add by xp
	EPCT_FairySkillbookStren=40,//妖精技能书强化 add by xp
	EPCT_BattleFieldCompose=41,//战场物品合成  add by xp
	EPCT_ReplaceFabao=42,//妖精魂核 add by xp
};
//合成类型(生成类型的子类)
enum EComposeType
{
	ECOMT_NULL = 0,
	ECOMT_StuffRefine = 10,			//(食材,布)精炼	10
	ECOMT_StuffProcess,				//(食材,布)加工	11
	ECOMT_StuffDecompose1,			//降分			13(add xtian 2009-10-13)
	ECOMT_RefineDan = 20,			//炼丹			20
	ECOMT_Cooking,					//烹饪			21
	ECOMT_WeaponMake = 30,			//武器制造		30
	ECOMT_MillstoneMake,			//消耗品			31
	ECIMT_WeaponAlbum,				// 武器图谱		32
	ECOMT_LoricaeMake = 40,			//铠甲制造		40
	ECOMT_RollMake,					//鉴定卷制造		41
	ECOMT_ArmorAlbum,				// 铠甲图谱		42
	ECOMT_ClothMake = 50,			//布甲制造		50
	ECOMT_FashionMake,				//时装制造		51
	ECOMT_ShaZhiMake,				//砂纸制造		52
	ECOMT_DecorateMake = 60,		//饰品制造		60
	ECOMT_ChiselMake,				//凿具制造		61(add xtian 12-3)
	ECOMT_GemProcess,				//宝石加工		62(add xtian 2009-2-12)
	ECOMT_DecorationAlbum,			// 饰品图谱		63
	ECOMT_Refine = 70,				//(矿石,草药)精练 70
	//ECOMT_Process = 71,					//(矿石,草药)加工 71
	//ECOMT_StuffDecompose2,			//降分2			72(add xtian 2009-10-13)
	ECOMT_EquipdDecompose = 80,		//装备分解		80
	ECOMT_PointUp,					//点化			81
	ECOMT_Rebuild = 90,				//印记改造		90	add xtian 2009-4-13
	ECOMT_LoongSoulCom,				//龙魂合成		91
	ECOMT_LoongSoulReb,				//龙魂改造		92
	ECOMT_GemCompose,				//宝石合成		93
	ECOMT_GemReBuild,				//宝石改造		94
	ECOMT_GemCarve,					//宝石雕琢		95
	ECOMT_PetMake = 100,			//灵魄合成		100 add xtian 2009-8-21
	ECOMT_WeaponProduce = 110,		// 武器制造		110
	ECOMT_ArmorProdeuce = 111,		// 盔甲制造		111
	ECOMT_ClothArmorProduce = 112,	// 布甲制造		112
	ECOMT_DecorationProduce = 113,	// 饰品制造		113
	ECOMT_WeaponRefine = 120,		// 武器萃取		120
	ECOMT_ArmorRefine = 121,		// 防具萃取		121
	ECOMT_DecorationRefine = 122,	// 饰品萃取		122
	ECOMT_OriStoneRefine = 130,		// 原石凝练		130
	//ECOMT_JingHuaRefine = 131,		// 精华凝练		131
	ECOMT_TimeLimitWeapon = 140,	// 时限武器		140
	ECOMT_TimeLimitArmor = 141,		// 时限防具		141
	ECOMT_legendDecoration = 142,		// 传奇饰品		142

	ECOMT_FoodProduce = 200,				// 食材加工		200
	ECOMT_SpriteFoodProduce = 210,	// 妖精食物		210

	ECOMT_ChristmasWeapon = 220,		// 圣诞武器		220
	ECOMT_SpriteSoul = 230,					// 妖精灵魂		230
	ECOMT_HighSoul = 231,					// 高级灵魂		231
	ECOMT_WashSoul = 232,					// 灵魂洗练		232

	ECOMT_CryStal = 240,					// 水晶合成

	ECOMT_Stole_52 = 250, // 52等级圣衣
	ECOMT_Stole_67 = 260, // 67等级圣衣
	ECOMT_Stole_82 = 270, // 82等级圣衣
	ECOMT_Stole_Blade = 280, // 神兵圣衣
	ECOMT_Stole_Bow = 290, // 玄翎圣衣
	ECOMT_Stole_Valor = 300, // 乾坤圣衣
	ECOMT_Stole_Swing = 310, // 仙音圣衣

	ECOMT_SoulLinker = 320, // 灵魂链

	ECOMT_MoonCake = 330, //月饼合成

	ECOMT_HallowmasGift = 340, // 万圣节礼包
	ECOMT_HallowmasName = 350, // 万圣节称号

	ECOMT_MoshenEquip = 380,	//魔神装备

	ECOMT_FairySoulStren=390,//妖精灵魂强化 add by xp
	ECOMT_FairySkillbookStren=400,//妖精技能书强化 add by xp
	ECOMT_BattleFieldCompose=410,//战场合成 add by xp
	ECOMT_ReplaceFabao=420,//妖精魂核 add by xp

	//ECOMT_TimeLimitDecoration = 142,// 时限饰品		142
	ECPOT_TreasureCompose = 900,	// 宝物合成		900 
	ECOMT_SpritFood1 = 901,			// 新春美食	策划牛逼
	ECOMT_SpritFood2 = 902,			// 新春美食	策划威武
	ECOMT_SpritFood3 = 903,			// 新春美食	策划无敌
	ECOMT_GoodPerfume = 904,		// 精美香水	策划大哥
};	

#define IsAdvanceComposeTypeValid(type) (((type) > EADVCOMT_NULL) && ((type) < EADVCOMT_MAX))

//每项生产包含的合成子类数量
#define ArtisanSubTypeQuan		ECOMT_StuffDecompose1 - ECOMT_StuffRefine + 1
#define DanTrainingSubTypeQuan	ECOMT_Cooking -  ECOMT_RefineDan + 1
#define SmithSubTypeQuan		ECIMT_WeaponAlbum - ECOMT_WeaponMake + 1
#define CastingSubTypeQuan		ECOMT_ArmorAlbum - ECOMT_LoricaeMake + 1
#define DressmakerSubTypeQuan	ECOMT_ShaZhiMake - ECOMT_ClothMake + 1
#define AechnicsSubTypeQuan		ECOMT_DecorationAlbum - ECOMT_DecorateMake + 1
#define SmiltSubTypeQuan		ECOMT_Refine - ECOMT_Refine + 1
#define PointUpSubTypeQuan		ECOMT_PointUp - ECOMT_EquipdDecompose + 1
#define GodArtisanSubTypeQuan	ECOMT_GemCarve - ECOMT_Rebuild + 1
#define PetSubTypeQuan			ECOMT_PetMake - ECOMT_PetMake + 1
#define EquipSubTypeQuan		ECOMT_DecorationProduce - ECOMT_WeaponProduce + 1
#define EquipRefineTypeQuan		ECOMT_DecorationRefine - ECOMT_WeaponRefine + 1
#define StuffRefineTypeQuan		ECOMT_OriStoneRefine - ECOMT_OriStoneRefine + 1
#define TimeLimitEquipTypeQuan		ECOMT_legendDecoration - ECOMT_TimeLimitWeapon + 1
#define FoodProduceTypeQuan		ECOMT_FoodProduce - ECOMT_FoodProduce + 1
#define SpriteFoodProduceTypeQuan		ECOMT_SpriteFoodProduce - ECOMT_SpriteFoodProduce + 1
#define ChristmasWeaponTypeQuan		ECOMT_ChristmasWeapon - ECOMT_ChristmasWeapon + 1
#define SpriteSoulTypeQuan		ECOMT_WashSoul - ECOMT_SpriteSoul + 1
#define CryStalTypeQuan		ECOMT_CryStal - ECOMT_CryStal + 1
#define Stole_52TypeQuan		ECOMT_Stole_52 - ECOMT_Stole_52 + 1
#define Stole_67TypeQuan		ECOMT_Stole_67 - ECOMT_Stole_67 + 1
#define Stole_82TypeQuan		ECOMT_Stole_82 - ECOMT_Stole_82 + 1
#define Stole_BladeTypeQuan		ECOMT_Stole_Blade - ECOMT_Stole_Blade + 1
#define Stole_BowTypeQuan		ECOMT_Stole_Bow - ECOMT_Stole_Bow + 1
#define Stole_ValorTypeQuan		ECOMT_Stole_Valor - ECOMT_Stole_Valor + 1
#define Stole_SwingTypeQuan		ECOMT_Stole_Swing - ECOMT_Stole_Swing + 1
#define Stole_SoulLinker		ECOMT_SoulLinker - ECOMT_SoulLinker +1
#define Stole_MoonCake		ECOMT_MoonCake - ECOMT_MoonCake +1
#define HallowmasGift		ECOMT_HallowmasGift - ECOMT_HallowmasGift +1
#define HallowmasName	ECOMT_HallowmasName - ECOMT_HallowmasName +1
#define TaiWamMoshenEquip ECOMT_MoshenEquip - ECOMT_MoshenEquip + 1
#define  FairySoulStren  ECOMT_FairySoulStren-ECOMT_FairySoulStren+1//add by xp
#define  FairySkillbookStren ECOMT_FairySkillbookStren-ECOMT_FairySkillbookStren+1//add by xp
#define  BattleFieldCompose ECOMT_BattleFieldCompose-ECOMT_BattleFieldCompose+1//add by xp
#define  ReplaceFabao ECOMT_ReplaceFabao-ECOMT_ReplaceFabao+1//add by xp

//合成配方类型
enum EFormulaType
{
	EFORMT_NULL	= 0,
	EFORMT_OreRefine,		//矿石提纯	1
	EFORMT_HerbalRefine,	//草药提纯	2
	EFORMT_ClothRefine,		//布料精纺	3
	EFORMT_FoodRefine,		//食材精制	4
	EFORMT_OreProcess,		//矿石加工	5
	EFORMT_HerbalProcess,	//草药加工	6
	EFORMT_ClothProcess,	//布料加工	7
	EFORMT_FoodProcess,		//食材加工	8
	EFORMT_ThewDrug,		//体力药		9
	EFORMT_EnergyDrug,		//真气药		10
	EFORMT_RestoreDrug,		//全回药		11
	EFORMT_GodDrug,			//神药		12
	EFORMT_SecondReDrug,	//秒回药		13
	EFORMT_InterinjuryDrug,	//内伤药		14
	EFORMT_NormalDrug,		//普通药		15
	EFORMT_Wine,			//酒			16
	EFORMT_Cate,			//美食		17
	EFORMT_Glave,			//剑			18
	EFORMT_Sword,			//刀			19
	EFORMT_Halberd,			//长兵		20
	EFORMT_Bow,				//弓			21
	EFORMT_SmallOrgan,		//小型法器	22
	EFORMT_BigOrgan,		//大型法器	23
	EFORMT_Instrument,		//琴			24
	EFORMT_Millstone,		//魔石		25
	EFORMT_Armet,			//头盔		26
	EFORMT_UpperLoricae,	//上身盔甲	27
	EFORMT_LowerLoricae,	//下身盔甲	28
	EFORMT_Armguard,		//护手		29
	EFORMT_BattleShoe,		//战靴		30
	EFORMT_Roll,			//鉴定卷		31
	EFORMT_Cap,				//帽子		32
	EFORMT_Jupe,			//上衣		33	
	EFORMT_Pants,			//下衣		34
	EFORMT_Glove,			//手套		35
	EFORMT_Shoe,			//鞋子		36
	EFORMT_Mantle,			//披风		37
	EFORMT_FashionCap,		//时装帽子	38
	EFORMT_FashionJupe,		//时装上衣	39
	EFORMT_FashionPants,	//时装下衣	40
	EFORMT_FashionGloave,	//时装手套	41
	EFORMT_FashionShoe,		//时装鞋子	42
	EFORMT_FashionMantle,	//时装披风	43
	EFORMT_FingerRing,		//戒指		44
	EFORMT_Caestus,			//腰饰		45
	EFORMT_Necklace,		//项链		46
	EFORMT_Chisel,			//凿具		47
	EFORMT_YuanRockIncise,	//原石切割	48
	EFORMT_GeBlade,			//割刀		49
	EFORMT_GangRebuild,		//钢锻印改造 50 add xtian 2009-4-13
	EFORMT_YuRebuild,		//玉磨印改造
	EFORMT_YinRebuild,		//银锻印改造
	EFORMT_JinRebuild,		//金琢印改造
	EFORMT_JinLoongSoul,	//金龙魂
	EFORMT_MuLoongSoul,		//木龙魂
	EFORMT_ShuLoongSoul,	//水龙魂
	EFORMT_HuoLoongSoul,	//火龙魂
	EFORMT_TuLoongSoul,		//土龙魂
	EFORMT_JinLoongSoulReb,	//金龙魂改造
	EFORMT_MuLoongSoulReb,	//木龙魂改造
	EFORMT_ShuLoongSoulReb,	//水龙魂改造
	EFORMT_HuoLoongSoulReb,	//火龙魂改造
	EFORMT_TuLoongSoulReb,	//土龙魂改造
	EFORMT_2Gem,			//二级宝石
	EFORMT_3Gem,			//三级宝石
	EFORMT_4Gem,			//四级宝石
	EFORMT_2GemRebuild,		//二级宝石改造
	EFORMT_3GemRebuild,		//三级宝石改造
	EFORMT_4GemRebuild,		//四级宝石改造
	EFORMT_RefineDanOther,	//炼丹其他
	EFORMT_WeaponMakeOther, //武器制造其他
	EFORMT_LoricaeMakeOther,//铸甲制造其他
	EFORMT_ClothMakeOther,	//布甲制造其他
	EFORMT_DecorateMakeOther,//饰品制造其他
	EFORMT_ShangSoul,			//上品灵魄
	EFORMT_LingSoul,			//灵品灵魄
	EFORMT_XianSoul,			//仙品灵魄
	EFORMT_ShengSoul,			//圣品灵魄
	EFORMT_MakeJade,			//炼玉
	EFORMT_PetFood,				//灵兽口粮
	EFORMT_GemBlade,			//宝石雕刀
	EFORMT_GemCarve,			//宝石雕琢
	EFORMT_ClothDecompose,		//布料降分
	EFORMT_FoodDecompose,		//食材降分
	EFORMT_OreDecompose,		//矿石降分
	EFORMT_HerbalDecompose,		//草药降分
	EFORMT_Album,				// 图谱
	EFORMT_5Gem,			// 五级宝石
	EFORMT_6Gem,			// 六级宝石
	EFORMT_7Gem,			// 七级宝石
	EFORMT_5GemRebuild,		// 五级宝石改造
	EFORMT_6GemRebuild,		// 六级宝石改造
	EFORMT_7GemRebuild,		// 七级宝石改造

	EFORMT_WeaponRefine = 120,		// 武器萃取		120
	EFORMT_ArmorRefine = 121,		// 防具萃取		121
	EFORMT_DecorationRefine = 122,	// 饰品萃取		122
	EFORMT_OriStoneRefine = 130,	// 原石凝练		130
	EFORMT_JingHuaRefine = 131,		// 精华凝练		131

	EFORMT_TimeLimitWeapon_40 = 140,		// 40级时限武器		141
	EFORMT_TimeLimitWeapon_60 = 141,		// 60级时限武器		141
	EFORMT_TimeLimitWeapon_80 = 142,		// 80级时限武器		142
	EFORMT_TimeLimitWeapon_100 = 143,		// 100级时限武器		143
	EFORMT_TimeLimitArmor_40 = 144,			// 40级时限防具		144
	EFORMT_TimeLimitArmor_60 = 145,			// 60级时限防具		145
	EFORMT_TimeLimitArmor_80 = 146,			// 80级时限防具		146
	EFORMT_TimeLimitArmor_100 = 147,		// 100级时限防具		147
	EFORMT_TimeLimitDecoration_40 = 148,	// 40级时限饰品		148
	EFORMT_TimeLimitDecoration_60 = 149,	// 60级时限饰品		149
	EFORMT_TimeLimitDecoration_80 = 150,	// 80级时限饰品		150
	EFORMT_TimeLimitDecoration_100 = 151,	// 100级时限饰品		151

	EFORMT_2LevelGemUpGrade = 161,	// 二级宝石雕琢		161
	EFORMT_3LevelGemUpGrade = 162,	// 3级宝石雕琢		162
	EFORMT_4LevelGemUpGrade = 163,	// 4级宝石雕琢		163
	EFORMT_5LevelGemUpGrade = 164,	// 5级宝石雕琢		164
	EFORMT_6LevelGemUpGrade = 165,	// 6级宝石雕琢		165
	EFORMT_7LevelGemUpGrade = 166,	// 7级宝石雕琢		166
	EFORMT_8LevelGemUpGrade = 167,	// 8级宝石雕琢		167
	EFORMT_9LevelGemUpGrade = 168,	// 9级宝石雕琢		168

	EFORMT_GodWeapon_60 = 171,		// 60级伪魔神武器	171
	EFORMT_GodWeapon_80 = 172,		// 80级伪魔神武器	172
	EFORMT_GodArmor_60 = 173,		// 60级伪魔神防具	173
	EFORMT_GodArmor_80 = 174,		// 80级伪魔神防具	174
	EFORMT_GodDecoration_60 = 175,	// 60级伪魔神饰品	175
	EFORMT_GodDecoration_80 = 176,	// 80级伪魔神饰品	176

	EFORMT_WaterProduct = 200,	// 水产加工	200
	EFORMT_EarthProduct = 201,		// 田产加工	201

	EFORMT_NormalFood= 210,		// 普通食物	210
	EFORMT_SpecFood = 211,			// 特殊食物	211

	EFORMT_OrangeChristmasWeapon = 220,			// 橙色圣诞武器	220
	EFORMT_PurpleChristmasWeapon = 221,				// 紫色圣诞武器	221

	EFORMT_SpriteSoulLevel_2 = 230,			// 2级妖精灵魂	230
	EFORMT_SpriteSoulLevel_3 = 231,			// 3级妖精灵魂	231
	EFORMT_SpriteSoulLevel_4 = 232,			// 4级妖精灵魂	232
	EFORMT_SpriteSoulLevel_5 = 233,			// 5级妖精灵魂	233
	EFORMT_SpriteSoulLevel_6 = 234,			// 6级妖精灵魂	234
	EFORMT_SpriteSoulLevel_7 = 235,			// 7级妖精灵魂	235
	EFORMT_SpriteSoulLevel_8 = 236,			// 8级妖精灵魂	236
	EFORMT_SpriteSoulLevel_9 = 237,			// 9级妖精灵魂	237
	EFORMT_SpriteSoulLevel_10= 238,			// 10级妖精灵魂	238

	EFORMT_CryStal_Sky= 240,						// 天之水晶合成
	EFORMT_CryStal_Earth = 241,						// 地之水晶合成
	
	EFORMT_Stole_Body_2 = 250,						// 武器2
	EFORMT_Stole_Legs_2 = 251,						// 圣衣头2
	EFORMT_Stole_Wrist_2 = 252,						// 圣衣上衣2
	EFORMT_Stole_Feet_2 = 253,						// 圣衣下一2
	EFORMT_Stole_Back_2 = 254,						// 圣衣手套2
	EFORMT_Stole_Finger_2 = 255,						// 鞋子2
	EFORMT_Stole_Waist_2 = 256,						// 披风2
	EFORMT_Stole_Neck_2 = 257,						// 饰品2

	EFORMT_Stole_Body_3 = 260,						// 武器3
	EFORMT_Stole_Legs_3 = 261,						// 圣衣头3
	EFORMT_Stole_Wrist_3 = 262,						// 圣衣上衣3
	EFORMT_Stole_Feet_3 = 263,						// 圣衣下衣3
	EFORMT_Stole_Back_3 = 264,						// 圣衣手套3
	EFORMT_Stole_Finger_3 = 265,						// 鞋子3
	EFORMT_Stole_Waist_3 = 266,						// 披风3
	EFORMT_Stole_Neck_3 = 267,						// 饰品3

	EFORMT_Stole_Body_4 = 270,						// 武器4
	EFORMT_Stole_Legs_4 = 271,						// 圣衣头4
	EFORMT_Stole_Wrist_4 = 272,						// 圣衣上衣4
	EFORMT_Stole_Feet_4 = 273,						// 圣衣下衣4
	EFORMT_Stole_Back_4 = 274,						// 圣衣手套4
	EFORMT_Stole_Finger_4 = 275,						// 鞋子4
	EFORMT_Stole_Waist_4 = 276,						// 披风4
	EFORMT_Stole_Neck_4 = 277,						// 饰品4

	EFORMT_Stole_Blade_85 = 280,						// 85级神兵神圣衣
	EFORMT_Stole_Blade_95 = 281,						// 95级神兵神圣衣

	EFORMT_Stole_Bow_85 = 290,						// 85级玄翎神圣衣
	EFORMT_Stole_Bow_95 = 291,						// 95级玄翎神圣衣

	EFORMT_Stole_Valor_85 = 300,						// 85级乾坤神圣衣
	EFORMT_Stole_Valor_95 = 301,						// 95级乾坤神圣衣

	EFORMT_Stole_Swing_85 = 310,						// 85级仙音神圣衣
	EFORMT_Stole_Swing_95 = 311,						// 95级仙音神圣衣

	EFORMT_SoulLinker = 320,						// 灵魂链合成

	EFORMT_MoonCake = 330,						// 月饼合成

	EFORMT_HallowmasGift = 340,						// 万圣节礼包
	EFORMT_HallowmasName = 350,						// 万圣节称号

	EFORMT_CryStalMake = 360,						// 水晶合成
	EFORMT_EquipGrow = 370,						// 装备成长

	EFORMT_Level8StrenSoul=391,               //八级强化灵魂 add by xp
	EFORMT_Level9StrenSoul=392,               //九级强化灵魂 add by xp
	EFORMT_Level10StrenSoul=393,            //十级强化灵魂 add by xp
	EFORMT_FairyStrenSkillbook=401,          //妖精强化技能书 add by xp
	EFORMT_BattleFieldCompose=411,       //战场合成 add by xp
	EFORMT_ReplaceFabao80=421,			  //妖精魂核[80]  add by xp
	EFORMT_ReplaceFabao85=422,			  //妖精魂核[85]  add by xp
	EFORMT_ReplaceFabao90=423,			  //妖精魂核[90]  add by xp
	EFORMT_ReplaceFabao95=424,			  //妖精魂核[95]  add by xp
	EFORMT_ReplaceFabao99=425,			  //妖精魂核[99]  add by xp
	EFORMT_ReplaceFabao100=426,			  //妖精魂核[100]  add by xp

	EFORMT_SpriteFood1 = 901,		// 新春美食 策划牛逼
	EFORMT_SpriteFood2 = 902,		// 新春美食 策划威武
	EFORMT_SpriteFood3 = 903,		// 新春美食 策划无敌
	EFORMT_GoodFerfume = 904,		// 精美香水 策划大哥

	EFORMT_SpriteSoul_2 = 2301,		// 2级妖精灵魂
	EFORMT_SpriteSoul_3 = 2302,		// 3级妖精灵魂
	EFORMT_SpriteSoul_4 = 2303,		// 4级妖精灵魂
	EFORMT_SpriteSoul_5 = 2304,		// 5级妖精灵魂
	EFORMT_SpriteSoul_6 = 2305,		// 6级妖精灵魂
	EFORMT_SpriteSoul_7 = 2306,		// 7级妖精灵魂
	EFORMT_SpriteSoul_8 = 2307,		// 8级妖精灵魂
	EFORMT_SpriteSoul_9 = 2308,		// 9级妖精灵魂
	EFORMT_SpriteSoul_10 = 2309,		// 10级妖精灵魂

	EFORMT_HighSoul_5 = 2311,		// 5级高级灵魂
	EFORMT_HighSoul_6 = 2312,		// 6级高级灵魂
	EFORMT_HighSoul_7 = 2313,		// 7级高级灵魂
	EFORMT_HighSoul_8 = 2314,		// 8级高级灵魂
	EFORMT_HighSoul_9 = 2315,		// 9级高级灵魂
	EFORMT_HighSoul_10 = 2316,		// 10级高级灵魂

	EFORMT_WashSoul_1 = 2321,		// 1级灵魂洗练
	EFORMT_WashSoul_2 = 2322,		// 2级灵魂洗练
	EFORMT_WashSoul_3 = 2323,		// 3级灵魂洗练
	EFORMT_WashSoul_4 = 2324,		// 4级灵魂洗练
	EFORMT_WashSoul_5 = 2325,		// 5级灵魂洗练
	EFORMT_WashSoul_6 = 2326,		// 6级灵魂洗练
	EFORMT_WashSoul_7 = 2327,		// 7级灵魂洗练
	EFORMT_WashSoul_8 = 2328,		// 8级灵魂洗练
	EFORMT_WashSoul_9 = 2329,		// 9级灵魂洗练
	EFORMT_WashSoul_10 = 2330,		// 10级灵魂洗练

};

//配方来源
enum EFormulaFrom
{
	EFormulaFrom_NULL = 0,
	EFormulaFrom_Skill,					//技能
	EFormulaFrom_NPC,					//NPC
	EFormulaFrom_Item,					//物品
	EFormulaFrom_SkillorNPC,			//技能及NPC
	EFormulaFrom_SkillorItem,			//技能及物品
	EFormulaFrom_NPCorItem,				//NPC及物品
};

//高级合成类型(生成类型的子类，支持多个材料合成多个结果里面的随机一个)
enum EAdvanceComposeType
{
	EADVCOMT_NULL = 0,
	EADVCOMT_Fashion = 1,				// 时装合成
	EADVCOMT_Gem = 2,					// 宝石合成
	EADVCOMT_GemBluePrint = 3,			// 宝石图纸
	EADVCOMT_YuanGu = 4,				// 远古装备图纸	
	EADVCOMT_ShengWen = 5,				// 圣纹合成
	EADVCOMT_MAX = 6,
};


//装备强化类型
enum EConsolidateType
{
	ECliDateT_NULL 		= 0,		//无
	ECliDateT_Posy		= 1,		//铭纹
	ECliDateT_Engrave	= 2,		//镌刻
	ECliDateT_Quench	= 3,		//淬火
	ECliDateT_Casting	= 4,		//熔铸
};

//所有强化类型
enum EAllConsolidateType
{
	EAllCliDateT_NULL 		= 0,		//无
	EAllCliDateT_Posy		= 1,		//铭纹
	EAllCliDateT_Engrave	= 2,		//镌刻
	EAllCliDateT_Quench		= 3,		//淬火
	EAllCliDateT_Chisel		= 4,		//开凿
	EAllCliDateT_HoleGem	= 5,		//镶嵌
	EAllCliDateT_Brand		= 6,		//烙印
	EAllCliDateT_LongFu		= 7,		//龙附
};

//强化铭纹属性
enum EPosyAtt
{
	EPosyAtt_NULL		= 0,	//无	
	EPosyAtt_MaxHP		= 1,    //最大体力
	EPosyAtt_MaxMP		= 2,	//最大真气
	EPosyAtt_ExAttack	= 3,	//外功攻击
	EPosyAtt_InAttack	= 4,	//内功攻击
	EPosyAtt_ExDefense	= 5,	//外功防御
	EPosyAtt_InDefense	= 6,	//内功防御

	EPosyAtt_End,
};
//强化镌刻属性
enum EEngraveAtt
{
	EEngraveAtt_NULL		= 0,	//无
	EEngraveAtt_WeaponDmg	= 1,	//武器伤害
	EEngraveAtt_WeaponSoul  = 2,	//武魂
	EEngraveAtt_Armor		= 3,	//防具护甲(防)
	EEngraveAtt_Deration	= 4,	//防具护甲(饰)
	EEngraveAtt_End,
};

//淬火类型
enum EQuenchType
{
	EEQuenchType_Metal		= 0,	//金淬
	EEQuenchType_Wood,				//木淬
	EEQuenchType_Water,				//水淬
	EEQuenchType_Fire,				//火淬
	EEQuenchType_Soil,				//土淬
};

//精纺阶段
enum ESpinStep
{
	ESpinStep_Null		= 0,	//未精纺
	ESpinStep_Basic,			//初级
	ESpinStep_Middle,			//中级
	ESpinStep_Senior,			//高级
	ESpinStep_End,
};

//精纺阶段对应等级
enum ESpinStepEquipLevel
{
	ESSEL_Null		= 1,			//未精纺
	ESSEL_Basic		= 30,			//初级
	ESSEL_Middle	= 60,			//中级
	ESSEL_Senior	= 90,			//高级
	ESSEL_End,
};

enum ESoarCastingType
{
	ESCT_Weapon			= 0,		//武器
	ESCT_Defence		= 1,		//防具
	ESCT_Decoration		= 2			//饰品
};

//精纺成功率原型
struct tagSpinSuccessRateProto
{
	DWORD				dwSpinItemNum;	//金丝线的数

	DWORD				dwSuccessRate[ESpinStep_End]; //成功率

	tagSpinSuccessRateProto()
	{
		memset(this,0,sizeof(*this));
	}

};

//map key
struct tagSpinKey
{
	DWORD				dwSpinFashionTypeID;
	ESuitAttType		eAttType; //强化套装属性类型

	tagSpinKey()
	{
		dwSpinFashionTypeID = GT_INVALID;
		eAttType = ESSAT_Null;
	}

	tagSpinKey(DWORD dwSpinFashionTypeID,ESuitAttType eAttType)
	{
		this->dwSpinFashionTypeID = dwSpinFashionTypeID;
		this->eAttType = eAttType;
	}
	bool operator<(const tagSpinKey& ver) const
	{
		if(dwSpinFashionTypeID<ver.dwSpinFashionTypeID)
			return true;
		else if(dwSpinFashionTypeID>ver.dwSpinFashionTypeID)
			return false;

		if( eAttType<ver.eAttType)
			return true;
		else if(eAttType>ver.eAttType)
			return false;

		return false;
	}

};


//精纺原型
struct tagSpinProto
{
	DWORD				dwSpinFashionTypeID[ESpinStep_End];		//精纺装备的typeid

//	TCHAR				szName[ESpinType_End][X_SHORT_NAME];	//装备名称

//	DWORD				dwSuitID[ESpinType_End];				//套装id

	ESuitAttType		eAttType; //强化套装属性类型

	tagSpinProto()
	{
		memset(this,0,sizeof(*this));
	}
};

//可强化装备部位
struct tagConsolidatePos
{
	EEquipPos ePos;
	BYTE	  byConsolidate;
};

//强化材料
struct tagConsolidateStuff
{
	DWORD				dwStuffID;			//材料ID
	EStuffType			eStuffType;			//材料类型
	DWORD				dwStuffNum;			//材料数量
};

//生产合成材料
struct tagProduceSutff
{
	DWORD				dwStuffID;			//材料ID
	EStuffType			eStuffType;			//材料类型
	DWORD				dwStuffNum;			//材料数量	
	DWORD				dwFailDeductionNum;		// 失败时扣除原料数量
};

//铭纹结构体
struct tagPosyProto
{

	DWORD				dwID;					//配方ID
	TCHAR				szName[X_SHORT_NAME];	//配方名称
	EPosyAtt			ePosyAtt;				//铭纹属性
	BYTE				byPosyTimes;			//铭纹次数
	INT					nSuccessRate;			//成功率
	DWORD				dwProduceTime;			//生产计时
	DWORD				dwPotValConsume;		//潜力值消耗
	DWORD				dwMoneyConsume;			//金钱消耗
	FLOAT 				fcoefficientA; 			//属性价值公式参数A
	FLOAT 				fcoefficientB; 			//属性价值公式参数B
	FLOAT 				fcoefficientC; 			//属性价值公式参数C
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY]; //强化材料
	tagConsolidatePos   ConsolidatePos[MAX_CONSOLIDATE_POS_QUANTITY];	//可强化装备部位
};

//镌刻结构体
struct tagEngraveProto
{

	DWORD				dwID;					//配方ID
	TCHAR				szName[X_SHORT_NAME];	//配方名称
	EEngraveAtt			eEngraveAtt;			//镌刻属性
	BYTE				byEngraveTimes;			//镌刻次数
	INT					nSuccessRate;			//成功率
	DWORD				dwProduceTime;			//生产计时
	DWORD				dwPotValConsume;		//潜力值消耗
	DWORD				dwMoneyConsume;			//金钱消耗
	FLOAT 				fcoefficientA; 			//属性价值公式参数A
	FLOAT 				fcoefficientB; 			//属性价值公式参数B
	FLOAT 				fcoefficientC; 			//属性价值公式参数C
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY]; //强化材料
	tagConsolidatePos   ConsolidatePos[MAX_CONSOLIDATE_POS_QUANTITY];	//可强化装备部位
};

//淬火属性
struct tagQuenchAtt
{
	EWuXing				eWuXing;
	INT					nWuXingValue;
};

//淬火结构体
struct tagQuenchProto
{
	DWORD				dwID;					//配方ID
	TCHAR				szName[X_SHORT_NAME];	//配方名称
	EQuenchType			eQuenchType;			//淬火类型
	tagQuenchAtt		srcQuenchAtt;			//原属性及属性值
	tagQuenchAtt		dstQuenchAtt;			//目标属性及属性值
	DWORD				dwPotValConsume;		//潜力值消耗
	DWORD				dwMoneyConsume;			//金钱消耗
	INT					nSuccessRate;			//成功率
	DWORD				dwProduceTime;			//生产计时
	tagConsolidateStuff	ConsolidateStuff[MAX_CONSOLIDATE_STUFF_QUANTITY]; //强化材料
};

//生产合成
struct tagProduceProto
{
	DWORD				dwID;					//配方ID
	TCHAR				szName[X_SHORT_NAME];	//配方名称
	EProduceType		eProType;				//生产类型
	EComposeType		eComType;				//合成类型
	EFormulaType		eFormType;				//配方类型
	EFormulaFrom		eFormFrom;				//配方来源
	DWORD				dwFormLvl;				//配方等级
	DWORD				dwProItemTypeID;		//合成的物品
	DWORD				dwProQuantity;			//合成的数量
	INT					nSuccessRate;			//成功率
	DWORD				dwProduceTime;			//生产计时
	DWORD				dwVitalityConsume;		//活力消耗
	DWORD				dwMasterIncre;			//熟练度增加
	DWORD				dwMoneyConsume;			//金钱消耗
	tagProduceSutff		ProduceStuff[MAX_PRODUCE_STUFF_QUANTITY];//生产材料
};

#define MAX_RANDOMPRODUCE_STUFF 2			// 需要的材料
#define MAX_RANDOMPRODUCE_NUM 20			// 最多可生产多少种物品

//时装合成
struct tagRandomProduceProto
{
	DWORD				dwID;									// 配方ID
	EAdvanceComposeType		eRProType;								// 生产类型
	DWORD				dwDesigner;								// 制作图
	DWORD				dwStuff[MAX_RANDOMPRODUCE_STUFF];		// 所需材料
	INT16				n16StuffNum[MAX_RANDOMPRODUCE_STUFF];	// 所需材料数目
	DWORD				dwVitalityConsume;						// 活力消耗
	DWORD				dwMoneyConsume;							// 金钱消耗
	DWORD				dwItemTypeID[MAX_RANDOMPRODUCE_NUM];	// 可生成的物品typeID
};

// 神铸
struct tagGodStrengthen
{
	DWORD dwID;					// 神铸等级
	DWORD dwExp;				// 当前等级所需经验
	DWORD dwGreenExp;
	DWORD dwBlueExp;
	DWORD dwOrangeExp;
	DWORD dwPurpleExp;
	DWORD dwPinkExp;
	DWORD dwCost;				// 金钱消耗
};

//强化道具强化的属性
struct tagConsolidateRoleAtt
{
	ERoleAttribute		eRoleAtt;
	INT					nAttVal;
};

//镶嵌,烙印,龙附(强化道具)
struct tagConsolidateItem
{
	DWORD					dwID;						//道具的ID
	DWORD					dwPotValConsume;			//潜力值消耗
	INT						nSuccessRate;				//成功率
	EWuXing					eWuXing;					//龙魂属性
	tagConsolidateRoleAtt	tagRoleAtt[MAX_CONSOLIDATE_ROLEATT];		//强化属性	
	tagConsolidatePos   ConsolidatePos[MAX_CONSOLIDATE_POS_QUANTITY + MAX_CONSOLIDATE_POS_HOLYEQUIP];	//可强化装备部位
};

//  分解点化输出物品
struct tagOutputStuff
{
	DWORD				dwStuffTypeID;			//输出材料TypeID
	INT					nPerMinVal;				//完美最小数量
	INT					nPerMaxVal;				//完美最大数量
	INT					nSucMinVal;				//成功最小数量
	INT					nSucMaxVal;				//成功最大数量
	INT					nRate;					//几率
};

// 分解点化
struct tagDeComposeProto
{
	DWORD				dwID;					//配方ID
	TCHAR				szName[X_SHORT_NAME];	//配方名称
	EComposeType		eComType;				//合成类型
	BYTE				byLevel;				//装备等级
	BYTE				byQuality;				//装备品级
	EItemType			eType;					//装备类型
	EItemTypeEx			eTypeEx;				//武器类型
	EEquipPos			ePos;					//装备位置
	DWORD				dwFormLvl;				//配方等级
	EFormulaFrom		eFormFrom;				//配方来源
	INT					nSuccessRate;			//成功率
	DWORD				dwProduceTime;			//生产计时
	DWORD				dwVitalityConsume;		//活力消耗
	DWORD				dwMasterIncre;			//熟练度增加
	DWORD				dwMoneyConsume;			//金钱消耗
	tagOutputStuff		OutputStuff[MAX_OUTPUT_QUANTITY];			//输出材料		
};

// 萃取
struct tagExtractProto
{
	DWORD				dwEquipID;
	DWORD				dwProduceTime;							//生产计时
	DWORD				dwVitalityConsume;						//活力消耗
	DWORD				dwMoneyConsume;							//金钱消耗
	BYTE				byQuality;								//装备品级
	DWORD				dwItemID[X_EXTRACT_MAX_PRODUCTION];		//分解产物的ID
	INT					nItemMaxNum[X_EXTRACT_MAX_PRODUCTION];	//分解产物最大数量
	INT					nItemMinNum[X_EXTRACT_MAX_PRODUCTION];	//分解产物最小数量
	DWORD				dwRate[X_EXTRACT_MAX_PRODUCTION];		//产物生成几率
};

//	熔铸材料
struct tagCastingStuff
{
	DWORD					dwID;				//材料ID
	DWORD					dwStuffNum;			//材料数量	

};
// 飞升装备熔铸
struct tagSoarCastingProto
{
	DWORD					dwID;					//配方ID
	TCHAR					szName[X_SHORT_NAME];	//配方名称
	BYTE					byLevel;				//装备等级
	ESoarCastingType		eType;					//熔铸类型
	DWORD					dwPotvalConsume;		//潜力值消耗
	INT						nSuccessRate;			//成功率
	tagConsolidateRoleAtt	CastingRoleAtt[MAX_CASTING_ROLEATT];		//强化属性	
	tagCastingStuff			CastingStuff[MAX_OUTPUT_QUANTITY];		//材料
};

//元神装备强化属性结构

//--------------------------------------------------------------------------------------------
//元神装备冲星
//--------------------------------------------------------------------------------------------
struct tagStarEnhanceProto
{
	BYTE	byID;			//元神装备的星级
	DWORD	dwCostItemId;	//强化星级消耗的道具的ID
	UINT16	nCostItemNum;	//强化星级消耗的道具的数量
	UINT16	nRate;			//强化对应星级的成功几率
	BYTE	byFailLevel;	//强化星级失败后退回的等级
	DWORD	dwCost;			//强化星级消耗的金钱数
	UINT	nStunt;			//绝技伤害加成
	UINT	nHitRate;		//命中加成
	UINT	nCritRate;		//致命加成
	UINT	nCritaMount;	//致命量加成
	UINT	nExAttack;		//外功攻击加成
	UINT	nInAttack;		//内功攻击加成
	UINT	nAccuRate;		//精准加成
	UINT	nExDamage;		//额外伤害加成
	UINT	nNegToughness;	//忽视韧性加成
	UINT	nMorale;		//士气加成
	tagStarEnhanceProto()
	{
		memset(this,0,sizeof(*this));
	}
};

#pragma pack(pop)
