--任务描述
local ShiMen_QuestNum = 5 --师门任务最大步骤，步骤大于此数则重置步骤为0
Quest_ShiMen = {}
Quest_ShiMen[1]={TargetType=QuestTargetType["Kill"],TargetID1=1040013,Num1=50,RewMoney=17400,RewXP=25200,RewItemID1=17400,MsgID=13001,}--白熊【北洲天雪】
Quest_ShiMen[2]={TargetType=QuestTargetType["Kill"],TargetID1=1040014,Num1=50,RewMoney=17400,RewXP=25200,RewItemID1=18600,MsgID=13002,}--雪地熊【北洲天雪】
Quest_ShiMen[3]={TargetType=QuestTargetType["Kill"],TargetID1=1040024,Num1=50,RewMoney=17400,RewXP=25200,RewItemID1=19700,MsgID=13003,}--冰洞蛇妖【北洲天雪】的【冷心洞】
Quest_ShiMen[4]={TargetType=QuestTargetType["Kill"],TargetID1=1040019,Num1=50,RewMoney=18600,RewXP=30100,RewItemID1=20800,MsgID=13004,}--雪地蓝魔【北洲天雪】
Quest_ShiMen[5]={TargetType=QuestTargetType["Kill"],TargetID1=1040026,Num1=50,RewMoney=18600,RewXP=30100,RewItemID1=32850,MsgID=13005,}--魔冥鬼师【北洲天雪】
Quest_ShiMen[6]={TargetType=QuestTargetType["Kill"],TargetID1=1040020,Num1=50,RewMoney=18600,RewXP=30100,RewItemID1=34350,MsgID=13006,}--天魔妖女【北洲天雪】
Quest_ShiMen[7]={TargetType=QuestTargetType["Kill"],TargetID1=1050004,Num1=50,RewMoney=19700,RewXP=44100,RewItemID1=43200,MsgID=13007,}--三鬼信徒【西洲幻林】
Quest_ShiMen[8]={TargetType=QuestTargetType["Kill"],TargetID1=1050005,Num1=50,RewMoney=19700,RewXP=44100,RewItemID1=45000,MsgID=13008,}--三鬼门徒【西洲幻林】
Quest_ShiMen[9]={TargetType=QuestTargetType["Kill"],TargetID1=1050006,Num1=50,RewMoney=19700,RewXP=44100,RewItemID1=52000,MsgID=13009,}--三鬼魔灵【西洲幻林】
Quest_ShiMen[10]={TargetType=QuestTargetType["Kill"],TargetID1=1050014,Num1=50,RewMoney=20800,RewXP=51450,RewItemID1=54000,MsgID=13010,}--幻林绿速龙【西洲幻林】
Quest_ShiMen[11]={TargetType=QuestTargetType["Kill"],TargetID1=1050015,Num1=50,RewMoney=20800,RewXP=51450,RewItemID1=69750,MsgID=13011,}--幻林红速龙【西洲幻林】
Quest_ShiMen[12]={TargetType=QuestTargetType["Kill"],TargetID1=1050016,Num1=50,RewMoney=20800,RewXP=51450,RewItemID1=72000,MsgID=13012,}--五鬼苦修者【西洲幻林】
Quest_ShiMen[13]={TargetType=QuestTargetType["Kill"],TargetID1=1050021,Num1=70,RewMoney=32850,RewXP=59850,MsgID=13013,}--三鬼先知【西洲幻林】
Quest_ShiMen[14]={TargetType=QuestTargetType["Kill"],TargetID1=1050022,Num1=70,RewMoney=32850,RewXP=59850,MsgID=13014,}--三鬼刽子手【西洲幻林】
Quest_ShiMen[15]={TargetType=QuestTargetType["Kill"],TargetID1=1050023,Num1=70,RewMoney=32850,RewXP=59850,MsgID=13015,}--三鬼野蛮人【西洲幻林】的【武岩洞】
Quest_ShiMen[16]={TargetType=QuestTargetType["Kill"],TargetID1=1060006,Num1=70,RewMoney=34350,RewXP=77700,MsgID=13016,}--丛林突袭者【南洲迷沼】
Quest_ShiMen[17]={TargetType=QuestTargetType["Kill"],TargetID1=1060005,Num1=70,RewMoney=34350,RewXP=77700,MsgID=13017,}--丛林袭击者【南洲迷沼】
Quest_ShiMen[18]={TargetType=QuestTargetType["Kill"],TargetID1=1060007,Num1=70,RewMoney=34350,RewXP=77700,MsgID=13018,}--痛苦野人【南洲迷沼】
Quest_ShiMen[19]={TargetType=QuestTargetType["Kill"],TargetID1=1060014,Num1=90,RewMoney=43200,RewXP=107100,MsgID=13019,}--沼泽巨鳄【南洲迷沼】
Quest_ShiMen[20]={TargetType=QuestTargetType["Kill"],TargetID1=1060015,Num1=90,RewMoney=43200,RewXP=107100,MsgID=13020,}--沼泽食人花【南洲迷沼】
Quest_ShiMen[21]={TargetType=QuestTargetType["Kill"],TargetID1=1060013,Num1=90,RewMoney=43200,RewXP=107100,MsgID=13021,}--饥饿蛇妖【南洲迷沼】
Quest_ShiMen[22]={TargetType=QuestTargetType["Kill"],TargetID1=1060022,Num1=90,RewMoney=45000,RewXP=140700,MsgID=13022,}--残忍蛇妖【南洲迷沼】
Quest_ShiMen[23]={TargetType=QuestTargetType["Kill"],TargetID1=1060023,Num1=90,RewMoney=45000,RewXP=140700,MsgID=13023,}--嗜血蛇妖【南洲迷沼】
Quest_ShiMen[24]={TargetType=QuestTargetType["Kill"],TargetID1=1060024,Num1=90,RewMoney=45000,RewXP=140700,MsgID=13024,}--死亡凝视者【南洲迷沼】
Quest_ShiMen[25]={TargetType=QuestTargetType["Kill"],TargetID1=1070002,Num1=100,RewMoney=52000,RewXP=182000,MsgID=13025,}--石皮野熊【东洲冥山】
Quest_ShiMen[26]={TargetType=QuestTargetType["Kill"],TargetID1=1070004,Num1=100,RewMoney=52000,RewXP=182000,MsgID=13026,}--狂风劫匪【东洲冥山】
Quest_ShiMen[27]={TargetType=QuestTargetType["Kill"],TargetID1=1070005,Num1=100,RewMoney=52000,RewXP=182000,MsgID=13027,}--狂风强盗【东洲冥山】
Quest_ShiMen[28]={TargetType=QuestTargetType["Kill"],TargetID1=1070008,Num1=100,RewMoney=52000,RewXP=182000,MsgID=13028,}--良田破坏者【东洲冥山】
Quest_ShiMen[29]={TargetType=QuestTargetType["Kill"],TargetID1=1070014,Num1=100,RewMoney=54000,RewXP=225750,MsgID=13029,}--原神教刺客【东洲冥山】
Quest_ShiMen[30]={TargetType=QuestTargetType["Kill"],TargetID1=1070015,Num1=100,RewMoney=54000,RewXP=225750,MsgID=13030,}--狂风刀手【东洲冥山】
Quest_ShiMen[31]={TargetType=QuestTargetType["Kill"],TargetID1=1070016,Num1=100,RewMoney=54000,RewXP=225750,MsgID=13031,}--狂风喽啰【东洲冥山】
Quest_ShiMen[32]={TargetType=QuestTargetType["Kill"],TargetID1=1070017,Num1=100,RewMoney=54000,RewXP=225750,MsgID=13032,}--狂风放蛊者【东洲冥山】
Quest_ShiMen[33]={TargetType=QuestTargetType["Kill"],TargetID1=1070023,Num1=120,RewMoney=69750,RewXP=275450,MsgID=13033,}--冥山龙【东洲冥山】
Quest_ShiMen[34]={TargetType=QuestTargetType["Kill"],TargetID1=1070024,Num1=120,RewMoney=69750,RewXP=275450,MsgID=13034,}--嗜月血狼【东洲冥山】
Quest_ShiMen[35]={TargetType=QuestTargetType["Kill"],TargetID1=1070025,Num1=120,RewMoney=69750,RewXP=275450,MsgID=13035,}--狂风魔盗【东洲冥山】
Quest_ShiMen[36]={TargetType=QuestTargetType["Kill"],TargetID1=1070026,Num1=120,RewMoney=69750,RewXP=275450,MsgID=13036,}--发疯的恶徒【东洲冥山】
Quest_ShiMen[37]={TargetType=QuestTargetType["Kill"],TargetID1=1070037,Num1=120,RewMoney=72000,RewXP=331100,MsgID=13037,}--狂风贵族【东洲冥山】
Quest_ShiMen[38]={TargetType=QuestTargetType["Kill"],TargetID1=1070039,Num1=120,RewMoney=72000,RewXP=331100,MsgID=13038,}--原神教护法【东洲冥山】的【原神圣殿】
Quest_ShiMen[39]={TargetType=QuestTargetType["Kill"],TargetID1=1070046,Num1=120,RewMoney=72000,RewXP=331100,MsgID=13039,}--地狱守卫【东洲冥山】
Quest_ShiMen[40]={TargetType=QuestTargetType["Kill"],TargetID1=1070047,Num1=120,RewMoney=72000,RewXP=331100,MsgID=13040,}--地狱石魔【东洲冥山】
Quest_ShiMen[41]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=5,TargetID2=3020001,Num2=5,TargetID3=3050010,Num3=2,TargetID4=3060001,Num4=3,RewMoney=17400,RewXP=25200,MsgID=13101,}--结晶、强化石、二等精铁、妖精之粉1级
Quest_ShiMen[42]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=5,TargetID2=3020001,Num2=5,TargetID3=3050010,Num3=2,TargetID4=3060001,Num4=3,RewMoney=18600,RewXP=30100,MsgID=13102,}--结晶、强化石、二等精铁、妖精之粉1级
Quest_ShiMen[43]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=5,TargetID2=3020001,Num2=5,TargetID3=3050011,Num3=2,TargetID4=3060001,Num4=3,RewMoney=19700,RewXP=44100,MsgID=13103,}--结晶、强化石、三等精铁、妖精之粉1级
Quest_ShiMen[44]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=5,TargetID2=3020001,Num2=5,TargetID3=3050011,Num3=2,TargetID4=3060001,Num4=3,RewMoney=20800,RewXP=51450,MsgID=13104,}--结晶、强化石、三等精铁、妖精之粉1级
Quest_ShiMen[45]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=8,TargetID2=3020001,Num2=8,TargetID3=3050014,Num3=2,TargetID4=3060002,Num4=3,RewMoney=32850,RewXP=59850,MsgID=13105,}--结晶、强化石、一等玄铁、妖精之粉2级
Quest_ShiMen[46]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=8,TargetID2=3020001,Num2=8,TargetID3=3050014,Num3=2,TargetID4=3060002,Num4=3,RewMoney=34350,RewXP=77700,MsgID=13106,}--结晶、强化石、一等玄铁、妖精之粉2级
Quest_ShiMen[47]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=10,TargetID2=3020001,Num2=10,TargetID3=3050015,Num3=2,TargetID4=3060002,Num4=3,RewMoney=43200,RewXP=107100,MsgID=13107,}--结晶、强化石、二等玄铁、妖精之粉2级
Quest_ShiMen[48]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=10,TargetID2=3020001,Num2=10,TargetID3=3050015,Num3=2,TargetID4=3060002,Num4=3,RewMoney=45000,RewXP=140700,MsgID=13108,}--结晶、强化石、二等玄铁、妖精之粉2级
Quest_ShiMen[49]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=15,TargetID2=3020001,Num2=15,TargetID3=3050016,Num3=2,TargetID4=3060003,Num4=3,RewMoney=52000,RewXP=182000,MsgID=13109,}--结晶、强化石、三等玄铁、妖精之粉3级
Quest_ShiMen[50]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=15,TargetID2=3020001,Num2=15,TargetID3=3050016,Num3=2,TargetID4=3060003,Num4=3,RewMoney=54000,RewXP=225750,MsgID=13110,}--结晶、强化石、三等玄铁、妖精之粉3级
Quest_ShiMen[51]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=20,TargetID2=3020001,Num2=20,TargetID3=3050019,Num3=2,TargetID4=3060003,Num4=3,RewMoney=69750,RewXP=275450,MsgID=13111,}--结晶、强化石、二等幻钢、妖精之粉3级
Quest_ShiMen[52]={TargetType=QuestTargetType["Collect"],TargetID1=3050003,Num1=20,TargetID2=3020001,Num2=20,TargetID3=3050019,Num3=2,TargetID4=3060003,Num4=3,RewMoney=72000,RewXP=331100,MsgID=13112,}--结晶、强化石、二等幻钢、妖精之粉3级
Quest_ShiMen[61]={TargetType=QuestTargetType["Collect"],TargetID1=4010007,Num1=30,RewMoney=17400,RewXP=25200,MsgID=13201,}--雪峰金钻【北洲天雪】
Quest_ShiMen[62]={TargetType=QuestTargetType["Collect"],TargetID1=4010008,Num1=35,RewMoney=18600,RewXP=30100,MsgID=13202,}--雪峰宝玉【北洲天雪】
Quest_ShiMen[63]={TargetType=QuestTargetType["Collect"],TargetID1=4010009,Num1=40,RewMoney=19700,RewXP=44100,MsgID=13203,}--幻林灵石【西洲幻林】
Quest_ShiMen[64]={TargetType=QuestTargetType["Collect"],TargetID1=4010010,Num1=45,RewMoney=20800,RewXP=51450,MsgID=13204,}--幻林宝玉【西洲幻林】
Quest_ShiMen[65]={TargetType=QuestTargetType["Collect"],TargetID1=4010011,Num1=50,RewMoney=32850,RewXP=59850,MsgID=13205,}--幻林金钻【西洲幻林】
Quest_ShiMen[66]={TargetType=QuestTargetType["Collect"],TargetID1=4010012,Num1=55,RewMoney=34350,RewXP=77700,MsgID=13206,}--迷沼宝玉【南洲迷沼】
Quest_ShiMen[67]={TargetType=QuestTargetType["Collect"],TargetID1=4010013,Num1=60,RewMoney=43200,RewXP=107100,MsgID=13207,}--迷沼天石【南洲迷沼】
Quest_ShiMen[68]={TargetType=QuestTargetType["Collect"],TargetID1=4010014,Num1=65,RewMoney=45000,RewXP=140700,MsgID=13208,}--玉龙山天钻【南洲迷沼】
Quest_ShiMen[69]={TargetType=QuestTargetType["Collect"],TargetID1=4010015,Num1=70,RewMoney=52000,RewXP=182000,MsgID=13209,}--平粱宝钻【东洲冥山】
Quest_ShiMen[70]={TargetType=QuestTargetType["Collect"],TargetID1=4010016,Num1=75,RewMoney=54000,RewXP=225750,MsgID=13210,}--冥山红玉【东洲冥山】
Quest_ShiMen[71]={TargetType=QuestTargetType["Collect"],TargetID1=4010017,Num1=80,RewMoney=69750,RewXP=275450,MsgID=13211,}--云蒙白金【东洲冥山】
Quest_ShiMen[72]={TargetType=QuestTargetType["Collect"],TargetID1=4010018,Num1=85,RewMoney=72000,RewXP=331100,MsgID=13212,}--黄龙晶玉【东洲冥山】
Quest_ShiMen[81]={TargetType=QuestTargetType["Collect"],TargetID1=4080017,Num1=5,RewMoney=18600,RewXP=30100,MsgID=13301,}--雪妖帽【北洲天雪】
Quest_ShiMen[82]={TargetType=QuestTargetType["Collect"],TargetID1=4080018,Num1=5,RewMoney=18600,RewXP=30100,MsgID=13302,}--雪麦【北洲天雪】
Quest_ShiMen[83]={TargetType=QuestTargetType["Collect"],TargetID1=4080019,Num1=5,RewMoney=18600,RewXP=30100,MsgID=13303,}--寒水鱼【北洲天雪】
Quest_ShiMen[84]={TargetType=QuestTargetType["Collect"],TargetID1=4080020,Num1=5,RewMoney=18600,RewXP=30100,MsgID=13304,}--冰封石【北洲天雪】
Quest_ShiMen[85]={TargetType=QuestTargetType["Collect"],TargetID1=4080017,Num1=5,RewMoney=18600,RewXP=30100,MsgID=13305,}--雪妖帽【北洲天雪】
Quest_ShiMen[86]={TargetType=QuestTargetType["Collect"],TargetID1=4080018,Num1=5,RewMoney=18600,RewXP=30100,MsgID=13306,}--雪麦【北洲天雪】
Quest_ShiMen[87]={TargetType=QuestTargetType["Collect"],TargetID1=4080019,Num1=5,RewMoney=18600,RewXP=30100,MsgID=13307,}--寒水鱼【北洲天雪】
Quest_ShiMen[88]={TargetType=QuestTargetType["Collect"],TargetID1=4080020,Num1=5,RewMoney=18600,RewXP=30100,MsgID=13308,}--冰封石【北洲天雪】
Quest_ShiMen[89]={TargetType=QuestTargetType["Collect"],TargetID1=4080025,Num1=10,RewMoney=20800,RewXP=51450,MsgID=13309,}--野猪牙【西洲幻林】
Quest_ShiMen[90]={TargetType=QuestTargetType["Collect"],TargetID1=4080026,Num1=10,RewMoney=20800,RewXP=51450,MsgID=13310,}--蝶精粉尘【西洲幻林】
Quest_ShiMen[91]={TargetType=QuestTargetType["Collect"],TargetID1=4080027,Num1=10,RewMoney=20800,RewXP=51450,MsgID=13311,}--速龙之牙【西洲幻林】
Quest_ShiMen[92]={TargetType=QuestTargetType["Collect"],TargetID1=4080028,Num1=10,RewMoney=20800,RewXP=51450,MsgID=13312,}--速龙之爪【西洲幻林】
Quest_ShiMen[93]={TargetType=QuestTargetType["Collect"],TargetID1=4080025,Num1=10,RewMoney=20800,RewXP=51450,MsgID=13313,}--野猪牙【西洲幻林】
Quest_ShiMen[94]={TargetType=QuestTargetType["Collect"],TargetID1=4080026,Num1=10,RewMoney=20800,RewXP=51450,MsgID=13314,}--蝶精粉尘【西洲幻林】
Quest_ShiMen[95]={TargetType=QuestTargetType["Collect"],TargetID1=4080027,Num1=10,RewMoney=20800,RewXP=51450,MsgID=13315,}--速龙之牙【西洲幻林】
Quest_ShiMen[96]={TargetType=QuestTargetType["Collect"],TargetID1=4080028,Num1=10,RewMoney=20800,RewXP=51450,MsgID=13316,}--速龙之爪【西洲幻林】
Quest_ShiMen[97]={TargetType=QuestTargetType["Collect"],TargetID1=4080033,Num1=15,RewMoney=43200,RewXP=107100,MsgID=13317,}--狼人之爪【南洲迷沼】
Quest_ShiMen[98]={TargetType=QuestTargetType["Collect"],TargetID1=4080034,Num1=15,RewMoney=43200,RewXP=107100,MsgID=13318,}--迷沼鳄的牙【南洲迷沼】
Quest_ShiMen[99]={TargetType=QuestTargetType["Collect"],TargetID1=4080035,Num1=15,RewMoney=43200,RewXP=107100,MsgID=13319,}--黑沼泥【南洲迷沼】
Quest_ShiMen[100]={TargetType=QuestTargetType["Collect"],TargetID1=4080036,Num1=15,RewMoney=43200,RewXP=107100,MsgID=13320,}--蛇妖皮【南洲迷沼】
Quest_ShiMen[101]={TargetType=QuestTargetType["Collect"],TargetID1=4080033,Num1=15,RewMoney=43200,RewXP=107100,MsgID=13321,}--狼人之爪【南洲迷沼】
Quest_ShiMen[102]={TargetType=QuestTargetType["Collect"],TargetID1=4080034,Num1=15,RewMoney=43200,RewXP=107100,MsgID=13322,}--迷沼鳄的牙【南洲迷沼】
Quest_ShiMen[103]={TargetType=QuestTargetType["Collect"],TargetID1=4080035,Num1=15,RewMoney=43200,RewXP=107100,MsgID=13323,}--黑沼泥【南洲迷沼】
Quest_ShiMen[104]={TargetType=QuestTargetType["Collect"],TargetID1=4080036,Num1=15,RewMoney=43200,RewXP=107100,MsgID=13324,}--蛇妖皮【南洲迷沼】
Quest_ShiMen[105]={TargetType=QuestTargetType["Collect"],TargetID1=4080041,Num1=20,RewMoney=54000,RewXP=225750,MsgID=13325,}--狂风符【东洲冥山】
Quest_ShiMen[106]={TargetType=QuestTargetType["Collect"],TargetID1=4080042,Num1=20,RewMoney=54000,RewXP=225750,MsgID=13326,}--冥山木【东洲冥山】
Quest_ShiMen[107]={TargetType=QuestTargetType["Collect"],TargetID1=4080043,Num1=20,RewMoney=54000,RewXP=225750,MsgID=13327,}--朱红线【东洲冥山】
Quest_ShiMen[108]={TargetType=QuestTargetType["Collect"],TargetID1=4080044,Num1=20,RewMoney=54000,RewXP=225750,MsgID=13328,}--大兽牙【东洲冥山】
Quest_ShiMen[109]={TargetType=QuestTargetType["Collect"],TargetID1=4080041,Num1=80,RewMoney=69750,RewXP=275450,MsgID=13329,}--狂风符【东洲冥山】
Quest_ShiMen[110]={TargetType=QuestTargetType["Collect"],TargetID1=4080042,Num1=80,RewMoney=69750,RewXP=275450,MsgID=13330,}--冥山木【东洲冥山】
Quest_ShiMen[111]={TargetType=QuestTargetType["Collect"],TargetID1=4080043,Num1=80,RewMoney=69750,RewXP=275450,MsgID=13331,}--朱红线【东洲冥山】
Quest_ShiMen[112]={TargetType=QuestTargetType["Collect"],TargetID1=4080044,Num1=80,RewMoney=69750,RewXP=275450,MsgID=13332,}--大兽牙【东洲冥山】
Quest_ShiMen[121]={TargetType=QuestTargetType["Kill"],TargetID1=1040047,Num1=1,RewMoney=17400,RewXP=25200,MsgID=13333,}--蛇妖杜莎梅【北洲天雪】的【冷心洞】
Quest_ShiMen[122]={TargetType=QuestTargetType["Kill"],TargetID1=1040045,Num1=1,RewMoney=18600,RewXP=30100,MsgID=13334,}--流窜匪首【北洲天雪】
Quest_ShiMen[123]={TargetType=QuestTargetType["Kill"],TargetID1=1050037,Num1=1,RewMoney=19700,RewXP=44100,MsgID=13335,}--幻林蝶王【西洲幻林】
Quest_ShiMen[124]={TargetType=QuestTargetType["Kill"],TargetID1=1050041,Num1=1,RewMoney=20800,RewXP=51450,MsgID=13336,}--痛苦之王【西洲幻林】
Quest_ShiMen[125]={TargetType=QuestTargetType["Kill"],TargetID1=1050042,Num1=1,RewMoney=32850,RewXP=59850,MsgID=13337,}--魔蝶紫罗兰【西洲幻林】
Quest_ShiMen[126]={TargetType=QuestTargetType["Kill"],TargetID1=1060048,Num1=1,RewMoney=34350,RewXP=77700,MsgID=13338,}--妖迷王【南洲迷沼】
Quest_ShiMen[127]={TargetType=QuestTargetType["Kill"],TargetID1=1060052,Num1=1,RewMoney=43200,RewXP=107100,MsgID=13339,}--九天石魔【南洲迷沼】的【石狼洞】
Quest_ShiMen[128]={TargetType=QuestTargetType["Kill"],TargetID1=1060053,Num1=1,RewMoney=45000,RewXP=140700,MsgID=13340,}--风影【南洲迷沼】的【石狼洞】
Quest_ShiMen[129]={TargetType=QuestTargetType["Kill"],TargetID1=1070074,Num1=1,RewMoney=52000,RewXP=182000,MsgID=13341,}--千人杀米雪【东洲冥山】的【山隐秘窟】
Quest_ShiMen[130]={TargetType=QuestTargetType["Kill"],TargetID1=1070073,Num1=1,RewMoney=54000,RewXP=225750,MsgID=13342,}--魔舞之花【东洲冥山】
Quest_ShiMen[131]={TargetType=QuestTargetType["Kill"],TargetID1=1070075,Num1=1,RewMoney=69750,RewXP=275450,MsgID=13343,}--护法雷芒【东洲冥山】
Quest_ShiMen[132]={TargetType=QuestTargetType["Kill"],TargetID1=1070082,Num1=1,RewMoney=72000,RewXP=331100,MsgID=13344,}--冥山山王【东洲冥山】

--接取任务
function q30401_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	local Index = 0
	local Step = role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Step"])
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel > 100 then
		rolelevel = 100
	end
	if Step == 0  then--接取第一步骤，小怪
		if rolelevel >= 40 and rolelevel<= 79 then
			local temp = math.floor((rolelevel-40)/5)
			Index = temp*3+ math.random(1,3)
		elseif rolelevel >=80 and rolelevel<=100 then
			local temp = math.floor((rolelevel-80)/5)
			Index = 24 + 4*(temp-1) + math.random(1,4)
		end
	elseif Step == 1 then --第二步，收生产材料
		local rand = math.random(100)
		local temp = math.floor((rolelevel-40)/5)
		Index = 41 + temp
		if rolelevel >= 100 then Index = 52 end
	elseif Step == 2 then --第三步，收小怪掉的材料
		local temp = math.floor((rolelevel-40)/5)
		Index = 61 + temp
		if rolelevel>= 100 then Index = 72 end
	elseif Step == 3 then --第四步，商店购买物品
		local temp = math.floor((rolelevel-40)/5)
		local temp1 = 0
		if temp >=2 and temp <= 4 then
			temp1 = 1
		elseif temp >= 5 and temp <= 7 then
			temp1 = 2
		elseif temp >= 8 and temp <= 12 then
			temp1 = 3
		end
		Index = 80 + temp1 * 8 + math.random(8)
	elseif Step == 4 then --第5步，杀BOSS
		local temp = math.floor((rolelevel-40)/5)
		Index = 121 + temp
		if Index > 132 then
		    Index = 132
		end
	end

	--初始化任务的动态数据
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_ShiMen, Index))
	--同步客户端总任务环数和任务说明
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_ShiMen[Index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

--完成任务
function q30401_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local Step = role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Step"])
	Step = Step + 1
	if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30792)  then
	local	Step2 = role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Step_qixu"])
	Step2 = Step2 + 1
	role.SetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Step_qixu"], Step2)
		if Step2 >=5 then
        role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 30792, 2)
		end
	end
	if Step >= ShiMen_QuestNum or Step < 0 then Step = 0 end
	role.SetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Step"], Step)
	local CurTime = tonumber(os.date("%j"))
	role.SetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Update"], CurTime)

	if role.GetTrialState(RoleID, 4016) == 1 then
		role.SetTrialCompleteNum(RoleID, 4016, role.GetTrialCompleteNum(RoleID,4016) + 1)
    end
	if role.GetTrialState(RoleID, 4016) == 1 and role.GetTrialCompleteNum(RoleID,4016) >= Trail_maxnum[4016] then
		role.TrialComplete(RoleID,4016)
	end

	if role.GetTrialState(RoleID, 4017) == 1 then
		role.SetTrialCompleteNum(RoleID, 4017, role.GetTrialCompleteNum(RoleID,4017) + 1)
    end
	if role.GetTrialState(RoleID, 4017) == 1 and role.GetTrialCompleteNum(RoleID,4017) >= Trail_maxnum[4017] then
		role.TrialComplete(RoleID,4017)
	end
end



function q30401_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local Step = role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Step"])
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Update"])
	if 0 == Step then
		local CurTime = tonumber(os.date("%j"))
		if CurTime~=UpdateTime then
			return 1
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 401001)--每天只能完成一次师门初始任务！
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	end
	if Step >= ShiMen_QuestNum then return 0 end
	return 1
end

--放弃任务
function q30401_OnCancel(MapID, InstanceID, QuestID, RoleID)
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Update"])
	local CurTime = tonumber(os.date("%j"))
	if CurTime~=UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Step"], 0)
	end
end

aux.RegisterQuestEvent(30401, 2, "q30401_OnCancel")
aux.RegisterQuestEvent(30401, 1, "q30401_OnComplete")
aux.RegisterQuestEvent(30401, 7, "q30401_OnQuestInit")
aux.RegisterQuestEvent(30401, 4, "q30401_OnCheckAccept")
--aux.RegisterQuestEvent(30401, 5, "q30401_OnCheckComplete")
