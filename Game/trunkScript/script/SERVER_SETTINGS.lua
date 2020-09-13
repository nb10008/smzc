--台湾代言人植入活动开关(1为开启，0为关闭)

TW_Endorsor_On = 0

--!\ 神迹争夺开发(1:开启 0:关闭)
OpenGodMiracleFlag = 1
TianShenZhong_OpenFlag = 1		--天神冢战场开关
QiHuaZhiHai_OpenFlag = 1			--七花之海战场开关
BenLeiLieGu_OpenFlag = 1			--奔雷裂谷战场开关

--竞技场开关(1:开启 0:关闭)
Open_Arena = 1

--新疆选秀开关(1:开启 0:关闭)
Open_XJ_XiaoLongNv = 0

--薪酬系统开关及薪酬额度设置(1为开启，0为关闭)
Salary_NPC_On = 1

--情人节使用个性动作获得真爱唇印(1为开启，0为关闭)
Kiss_Motion = 0

Salary_RoleLevel_Tabel = {}
Salary_RoleLevel_Tabel[50] = 500	--50-59级玩家领取额度
Salary_RoleLevel_Tabel[60] = 1000	--60-69级玩家领取额度
Salary_RoleLevel_Tabel[70] = 1500	--70-79级玩家领取额度
Salary_RoleLevel_Tabel[80] = 2000	--80-89级玩家领取额度
Salary_RoleLevel_Tabel[90] = 2000	--90-99级玩家领取额度
Salary_RoleLevel_Tabel[100] = 3000	--100级玩家领取额度

--世博会活动开关(1=开，0=关)
EXPO_trigger = 0

--劳动节开关(1=开，0=关)
Labor_trigger = 0

--英雄乱舞和群雄逐鹿开关开关(1=开，0=关)
PVPactivity_trigger = 1

--端午节活动模式（0=大陆，1=台湾）

Duanwu_Mode = 0

--飞升开关（1=开，0=关）
FeiSheng_trigger = 1

-- 夺宝奇兵活动开关（1=开，0=关）
DuoBaoQiBing_trigger	= 1


--万圣节活动开关
WanShengJie_Open = 1

--腊八粥开关（1=开，0=关）
LaBaZhou_Open = 1

--老服信仰兑换人开关
GetFaith_Open = 1

--藏经阁开关（1=开，0=关）
CangJingGe_Open = 1

--信仰之怒兑换人（1=开，0=关）
XinYangZhiNu_Open = 1

--劳动节兑换人（1=开，0=关）
LaoDongJie_Open = 1

--悬赏榜倍率开关
XuanShangBUFF_Open = 1

--感恩节活动（1=开，0=关）
Ganenxianling_open = 1
Ganenshizhe_open = 1

--中秋活动开关
MidautumnDay = 1

--宝石兑换商
Baoshiduihuan = 1


--资料片：通灵前夕
ZLP_start = 1  	--0关闭，1开启
EVE_last = 6	--前夕持续天数
EVE_hour = 20	--每天刷怪时间点
ziliaopianlibao = 0 --0关闭，1开启

--5倍悬赏任务奖励
xuanshangrenwu5 = 1  	--0关闭，1开启

--声望加倍
shengwangjiabei = 0    --0关闭，2代表2倍声望开启，3代表3倍声望开启

--勇士回归
BravemanBack = 1    --0关闭，1开启

--欢乐侍者
huanleshizhe= 0 --0关闭，1开启
huanlebiwupin = 1 --0关闭，1开启
huanlebijinqian = 1 --0关闭，1开启

--初冬礼包魔神之魂奖励
chudonglibao = 0 --0关闭，1开启

--阳光大使开关
yangguangdashi = 0 --0关闭，1开启

--上线提示
online_tip = 1    --0关闭，1开启

--幸福传递
happiness = 0 --0关闭，1开启

--幸运之神（点名活动）
lucky = 0 --0关闭，1开启

--水晶心领取
crystal = 1 --0关闭，1开启

--金币总动员
JBZDY_Open = 1            --0关闭，1开启
JBZDY_SystemGold = 5000   --每天系统添加进奖池的金币
JBZDY_Limit = 362         --如果大于1，则在该天之前不允许开出大奖（需要填写os.data(%j)的值）
JBZDY_SetDay = 362         --如果大于1，则在该天之后(当日不会有任何效果，从后一天开始生效)每加一天，开出大奖几率提高x%（需要填写os.data(%j)的值）
JBZDY_SetDay_Step = 10    --满足JBZDY_SetDay之后每天增加的概率
JBZDY_Nomal = 10          --平常的大奖开出几率

--特权使者
tequanshizhe = 1  --0关闭，1开启

--shenmozaowuzhu
shenmozaowuzhu = 1  --0关闭，1开启


--s妖精秘药兑换商
yaojingmiyaoduihuan = 1  --0关闭，1开启

--新开服活动
xinkaifuhuodong = 1  --0关闭，1开启

--幻彩商人
huancaishangren = 1  --0关闭，1开启
