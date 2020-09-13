local function OnTalk()
	Produce.ClearNPCTalkOptions();

	Quest.ClearNPCTalkOptions();                    			
	Quest.SetCompleteOptions(4001055);

	Produce.SetProduceOptions(0, ProduceSkillTalk, "学习生产技能");
	Produce.SetProduceOptions(1, ProduceSkillTalk, "遗忘生产技能");

	if (Quest.QuestIsCompleted(10015)) then
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin, "了解生产技能" );
	end

	Produce.NPCTalk(0,0,"您可以在此处学习各种生产技能，也可将已学习的技能进行遗忘。");
end

local function OnScenarioTalk(id,step)
	if( step == ScenarioTalkBegin )then
		Quest.ClearNPCTalkOptions();
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+1,"开采与收获" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+2,"冶炼与巧匠" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+3,"锻造与工艺" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+4,"铸甲与裁缝" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+5,"炼丹" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+6,"点化" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+7,"活力值" );
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+8,"返回" );
		Quest.NPCTalk(0,0,"生产大师：\\n    少侠可以在本人处<color=0xff00a2ff>学习<color=0xffffffff>和<color=0xff00a2ff>遗忘<color=0xffffffff>生产技能。\\n    龙里面的生产技能共分为<color=0xffff0000>10<color=0xffffffff>种，其中每个人可同时学习<color=0xffff0000>3<color=0xffffffff>种生产技能。少侠可以在下面的选项里进行详细的了解。");
	elseif( step == ScenarioTalkBegin+1 )then
		Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>开采与收获：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>开采<color=0xffffffff>技能可从<color=0xffff80ff>矿石与草药资源点<color=0xffffffff>中获得<color=0xff00ddfe>矿石材料<color=0xffffffff>与<color=0xff00ddfe>草药材料<color=0xffffffff>\\n    <color=0xff00a2ff>收获<color=0xffffffff>技能可从<color=0xffff80ff>食材与布料资源点<color=0xffffffff>中获得<color=0xff00ddfe>食材材料<color=0xffffffff>或<color=0xff00ddfe>布料材料<color=0xffffffff>\\n    开采和收获所获得的物品为<color=0xff00a2ff>其他生产技能所需的原材料<color=0xffffffff>\\n    \\n    每一次<color=0xff00a2ff>使用生产技能都会增加技能熟练度<color=0xffffffff>，<color=0xff00a2ff>技能熟练度满后技能自动升级<color=0xffffffff>\\n    技能等级越高，开发或收获出的高品质材料几率越高");
	elseif( step == ScenarioTalkBegin+2 )then
		Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>冶炼与巧匠：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>冶炼<color=0xffffffff>技能可将各种<color=0xff00ddfe>矿石<color=0xffffffff>与<color=0xff00ddfe>草药<color=0xffffffff>材料进行<color=0xff00a2ff>加工<color=0xffffffff>或<color=0xff00a2ff>提纯<color=0xffffffff>\\n    <color=0xff00a2ff>巧匠<color=0xffffffff>技能可将各种<color=0xff00ddfe>布料<color=0xffffffff>和<color=0xff00ddfe>食材<color=0xffffffff>材料进行<color=0xff00a2ff>加工<color=0xffffffff>或<color=0xff00a2ff>精炼<color=0xffffffff>\\n    可将<color=0xff00a2ff>多个低品级材料加工成为高品级材料<color=0xffffffff>，或者提纯或精炼为稀有材料\\n    \\n    每一次<color=0xff00a2ff>使用生产技能都会增加技能熟练度<color=0xffffffff>，<color=0xff00a2ff>技能熟练度满后技能自动升级<color=0xffffffff>\\n    技能等级越高，所学会的生产配方越多");
	elseif( step == ScenarioTalkBegin+3 )then
		Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>锻造与工艺：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>锻造<color=0xffffffff>技能可以制作各种<color=0xff00ddfe>武器<color=0xffffffff>与<color=0xff00ddfe>磨石<color=0xffffffff>\\n    <color=0xff00a2ff>工艺<color=0xffffffff>技能可以制作各种<color=0xff00ddfe>饰品<color=0xffffffff>与进行<color=0xff00a2ff>宝石切割<color=0xffffffff>、<color=0xff00a2ff>宝石雕琢<color=0xffffffff>\\n    在制作武器与饰品时，若使用的<color=0xff00a2ff>原材料品质越高<color=0xffffffff>，制作出的装备<color=0xff00a2ff>出现高品质几率越大<color=0xffffffff>\\n    \\n    每一次制作出各种装备或物品都会增加技能熟练度，技能熟练度满后技能自动升级\\n    技能等级越高，所学会的生产配方越多");
	elseif( step == ScenarioTalkBegin+4 )then
		Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>铸甲与裁缝：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>铸甲<color=0xffffffff>技能可以制作各种<color=0xff00ddfe>铠甲防具<color=0xffffffff>与<color=0xff00ddfe>鉴定卷<color=0xffffffff>\\n    <color=0xff00a2ff>裁缝<color=0xffffffff>技能可以制作各种<color=0xff00ddfe>布装防具<color=0xffffffff>与<color=0xff00ddfe>时装<color=0xffffffff>\\n    在制作武器与饰品时，若使用的<color=0xff00a2ff>原材料品质越高<color=0xffffffff>，制作出的装备<color=0xff00a2ff>出现高品质几率越大<color=0xffffffff>\\n    \\n    每一次制作出各种装备或物品都会增加技能熟练度，技能熟练度满后技能自动升级\\n    技能等级越高，所学会的生产配方越多");
	elseif( step == ScenarioTalkBegin+5 )then
		Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>炼丹：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>炼丹<color=0xffffffff>技能可以制作各种<color=0xff00ddfe>药品<color=0xffffffff>与<color=0xff00ddfe>食品<color=0xffffffff>，同时还可以制作出<color=0xff00a2ff>其他生产技能需要的炼制物品<color=0xffffffff>\\n    \\n    每一次制作出各种恢复品，都会增加技能熟练度，技能熟练度满后技能自动升级\\n    技能等级越高，所学会的生产配方越多");
	elseif( step == ScenarioTalkBegin+6 )then
		Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>点化：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>点化<color=0xffffffff>技能可以<color=0xff00a2ff>将装备进行点化和分解<color=0xffffffff>\\n    点化可将装备转化成为游戏中的<color=0xff00a2ff>各种重要道具<color=0xffffffff>\\n    分解可将装备分解成为<color=0xff00a2ff>各种原材料<color=0xffffffff>\\n    \\n    每一次使用生产技能，都会增加技能熟练度，技能熟练度满后技能自动升级\\n    技能等级越高，可点化和分解更高级的装备");
	elseif( step == ScenarioTalkBegin+7 )then
		Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>活力值：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    无论使用<color=0xff00a2ff>任何生产技能<color=0xffffffff>，均会消耗角色的<color=0xff00a2ff>活力值<color=0xffffffff>。\\n    活力值可以在<color=0xff00a2ff>角色界面<color=0xffffffff>内查看。\\n    角色的<color=0xff00a2ff>等级越高<color=0xffffffff>，<color=0xff00a2ff>活力值越高<color=0xffffffff>。\\n    只要不断<color=0xff00a2ff>在线<color=0xffffffff>，角色的活力值就会<color=0xff00a2ff>缓慢恢复<color=0xffffffff>。\\n    同时游戏内有<color=0xff00a2ff>恢复活力值的药品<color=0xffffffff>。");
	elseif( step == ScenarioTalkBegin+8 )then
		OnTalk();
	end
end

local function OnProduceSkillTalk(id, step)
	if( id == 0 )then
		Produce.ClearNPCTalkOptions();
		Produce.LearnProduceSkill(0, 90000, "开采");
		Produce.LearnProduceSkill(1, 90001, "收获");
		Produce.LearnProduceSkill(2, 90002, "冶炼");
		Produce.LearnProduceSkill(3, 90003, "巧匠");
		Produce.LearnProduceSkill(4, 90004, "炼丹");
		Produce.LearnProduceSkill(5, 90005, "锻造");
		Produce.LearnProduceSkill(6, 90006, "铸甲");
		Produce.LearnProduceSkill(7, 90007, "裁缝");
		Produce.LearnProduceSkill(8, 90008, "工艺");
		Produce.LearnProduceSkill(9, 90009, "点化");
		Produce.SetCancelProduceSkill(10, -1, "返回");
		Produce.NPCTalk(0,0,"您可从以下数种生产技能中选择<A>三种</A>进行学习。\\n    <S>开采</S>：可采集<A>矿石材料</A>或<A>药材材料</A>\\n    <S>收获</S>：可获得<A>布料材料</A>或<A>食材材料</A>\\n    <P>冶炼</P>：可将<A>矿石材料</A>或<A>药材材料</A>进行<A>加工</A>或<A>精炼</A>\\n    <P>巧匠</P>：可将<A>布料材料</A>或<A>食材材料</A>进行<A>加工</A>或<A>精炼</A>\\n    <N>炼丹</N>：可制作<A>药品</A>和<A>食品</A>\\n    <N>锻造</N>：可制作各种<A>武器</A>和<A>磨石</A>\\n    <D>铸甲</D>：可制作<A>铠甲防具</A>和<A>鉴定卷</A>\\n    <D>裁缝</D>：可制作各种<A>防具</A>、<A>部分时装</A>以及<A>染料</A>\\n    <C>工艺</C>：可制作<A>饰品</A>、<A>加工宝石</A>以及<A>凿具</A>\\n    <C>点化</C>：可<A>点化</A>或<A>分解</A>装备\\n    \\n    您可自由进行选择");
	elseif( id == 1 )then
		Produce.ClearNPCTalkOptions();
		Produce.LeaveProduceSkill(0, 90000, "开采");
		Produce.LeaveProduceSkill(1, 90001, "收获");
		Produce.LeaveProduceSkill(2, 90002, "冶炼");
		Produce.LeaveProduceSkill(3, 90003, "巧匠");
		Produce.LeaveProduceSkill(4, 90004, "炼丹");
		Produce.LeaveProduceSkill(5, 90005, "锻造");
		Produce.LeaveProduceSkill(6, 90006, "铸甲");
		Produce.LeaveProduceSkill(7, 90007, "裁缝");
		Produce.LeaveProduceSkill(8, 90008, "工艺");
		Produce.LeaveProduceSkill(9, 90009, "点化");
		Produce.SetCancelProduceSkill(10, -1, "返回");
		Produce.NPCTalk(0,0,"遗忘生产技能后，该生产技能的<P>熟练度会清空<P>，您需要遗忘哪种生活技能？");
	end
end
			
local function OnCompleteQuestTalk(id,step)
	if( id == 10015)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,10015,QuestTalkBegin+1,"我已了解！[选择此项可完成任务]" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,10015,QuestTalkBegin+2,"开采与收获" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,10015,QuestTalkBegin+3,"冶炼与巧匠" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,10015,QuestTalkBegin+4,"锻造与工艺" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,10015,QuestTalkBegin+5,"铸甲与裁缝" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,10015,QuestTalkBegin+6,"炼丹" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,10015,QuestTalkBegin+7,"点化" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,10015,QuestTalkBegin+8,"活力值" );
			Quest.NPCTalk(0,0,"生产大师：\\n    少侠可以在本人处<color=0xff00a2ff>学习<color=0xffffffff>和<color=0xff00a2ff>遗忘<color=0xffffffff>生产技能。\\n    龙里面的生产技能共分为<color=0xffff0000>10<color=0xffffffff>种，其中每个人可同时学习<color=0xffff0000>3<color=0xffffffff>种生产技能。少侠可以在下面的选项里进行详细的了解。\\n    \\n    <color=0xffffae00>[小提示：若您对生产方面的内容全部有所了解之后，可点击“我已了解”选项继续任务流程。]<color=0xffffffff>");	        
		elseif( step == QuestTalkBegin+1 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,10015,QuestTalkBegin+9,"确定" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,10015,QuestTalkBegin,"返回" );
			Quest.NPCTalk(0,0,"生产大师：\\n    少侠既已全部了解，那我也不再啰嗦。日后若有任何不明，均可再我处询问。");
		elseif( step == QuestTalkBegin+2 )then
			Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>开采与收获：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>开采<color=0xffffffff>技能可从<color=0xffff80ff>矿石与草药资源点<color=0xffffffff>中获得<color=0xff00ddfe>矿石材料<color=0xffffffff>与<color=0xff00ddfe>草药材料<color=0xffffffff>\\n    <color=0xff00a2ff>收获<color=0xffffffff>技能可从<color=0xffff80ff>食材与布料资源点<color=0xffffffff>中获得<color=0xff00ddfe>食材材料<color=0xffffffff>或<color=0xff00ddfe>布料材料<color=0xffffffff>\\n    开采和收获所获得的物品为<color=0xff00a2ff>其他生产技能所需的原材料<color=0xffffffff>\\n    \\n    每一次<color=0xff00a2ff>使用生产技能都会增加技能熟练度<color=0xffffffff>，<color=0xff00a2ff>技能熟练度满后技能自动升级<color=0xffffffff>\\n    技能等级越高，开发或收获出的高品质材料几率越高\\n    \\n    <color=0xffffae00>[小提示：若您对生产方面的内容全部有所了解之后，可点击“我已了解”选项继续任务流程。]<color=0xffffffff>");
		elseif( step == QuestTalkBegin+3 )then
			Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>冶炼与巧匠：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>冶炼<color=0xffffffff>技能可将各种<color=0xff00ddfe>矿石<color=0xffffffff>与<color=0xff00ddfe>草药<color=0xffffffff>材料进行<color=0xff00a2ff>加工<color=0xffffffff>或<color=0xff00a2ff>提纯<color=0xffffffff>\\n    <color=0xff00a2ff>巧匠<color=0xffffffff>技能可将各种<color=0xff00ddfe>布料<color=0xffffffff>和<color=0xff00ddfe>食材<color=0xffffffff>材料进行<color=0xff00a2ff>加工<color=0xffffffff>或<color=0xff00a2ff>精炼<color=0xffffffff>\\n    可将<color=0xff00a2ff>多个低品级材料加工成为高品级材料<color=0xffffffff>，或者提纯或精炼为稀有材料\\n    \\n    每一次<color=0xff00a2ff>使用生产技能都会增加技能熟练度<color=0xffffffff>，<color=0xff00a2ff>技能熟练度满后技能自动升级<color=0xffffffff>\\n    技能等级越高，所学会的生产配方越多\\n    \\n    <color=0xffffae00>[小提示：若您对生产方面的内容全部有所了解之后，可点击“我已了解”选项继续任务流程。]<color=0xffffffff>");
		elseif( step == QuestTalkBegin+4 )then
			Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>锻造与工艺：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>锻造<color=0xffffffff>技能可以制作各种<color=0xff00ddfe>武器<color=0xffffffff>与<color=0xff00ddfe>磨石<color=0xffffffff>\\n    <color=0xff00a2ff>工艺<color=0xffffffff>技能可以制作各种<color=0xff00ddfe>饰品<color=0xffffffff>与进行<color=0xff00a2ff>宝石切割<color=0xffffffff>、<color=0xff00a2ff>宝石雕琢<color=0xffffffff>\\n    在制作武器与饰品时，若使用的<color=0xff00a2ff>原材料品质越高<color=0xffffffff>，制作出的装备<color=0xff00a2ff>出现高品质几率越大<color=0xffffffff>\\n    \\n    每一次制作出各种装备或物品都会增加技能熟练度，技能熟练度满后技能自动升级\\n    技能等级越高，所学会的生产配方越多\\n    \\n    <color=0xffffae00>[小提示：若您对生产方面的内容全部有所了解之后，可点击“我已了解”选项继续任务流程。]<color=0xffffffff>");
		elseif( step == QuestTalkBegin+5 )then
			Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>铸甲与裁缝：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>铸甲<color=0xffffffff>技能可以制作各种<color=0xff00ddfe>铠甲防具<color=0xffffffff>与<color=0xff00ddfe>鉴定卷<color=0xffffffff>\\n    <color=0xff00a2ff>裁缝<color=0xffffffff>技能可以制作各种<color=0xff00ddfe>布装防具<color=0xffffffff>与<color=0xff00ddfe>时装<color=0xffffffff>\\n    在制作武器与饰品时，若使用的<color=0xff00a2ff>原材料品质越高<color=0xffffffff>，制作出的装备<color=0xff00a2ff>出现高品质几率越大<color=0xffffffff>\\n    \\n    每一次制作出各种装备或物品都会增加技能熟练度，技能熟练度满后技能自动升级\\n    技能等级越高，所学会的生产配方越多\\n    \\n    <color=0xffffae00>[小提示：若您对生产方面的内容全部有所了解之后，可点击“我已了解”选项继续任务流程。]<color=0xffffffff>");
		elseif( step == QuestTalkBegin+6 )then
			Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>炼丹：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>炼丹<color=0xffffffff>技能可以制作各种<color=0xff00ddfe>药品<color=0xffffffff>与<color=0xff00ddfe>食品<color=0xffffffff>，同时还可以制作出<color=0xff00a2ff>其他生产技能需要的炼制物品<color=0xffffffff>\\n    \\n    每一次制作出各种恢复品，都会增加技能熟练度，技能熟练度满后技能自动升级\\n    技能等级越高，所学会的生产配方越多\\n    \\n    <color=0xffffae00>[小提示：若您对生产方面的内容全部有所了解之后，可点击“我已了解”选项继续任务流程。]<color=0xffffffff>");
		elseif( step == QuestTalkBegin+7 )then
			Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>点化：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    <color=0xff00a2ff>点化<color=0xffffffff>技能可以<color=0xff00a2ff>将装备进行点化和分解<color=0xffffffff>\\n    点化可将装备转化成为游戏中的<color=0xff00a2ff>各种重要道具<color=0xffffffff>\\n    分解可将装备分解成为<color=0xff00a2ff>各种原材料<color=0xffffffff>\\n    \\n    每一次使用生产技能，都会增加技能熟练度，技能熟练度满后技能自动升级\\n    技能等级越高，可点化和分解更高级的装备\\n    \\n    <color=0xffffae00>[小提示：若您对生产方面的内容全部有所了解之后，可点击“我已了解”选项继续任务流程。]<color=0xffffffff>");
		elseif( step == QuestTalkBegin+8 )then
			Quest.NPCTalk(0,0,"<font=小白雅黑,0,20,20><color=0xffff00ff>活力值：<color=0xffffffff><font=小白雅黑,0,17,20>\\n    \\n    无论使用<color=0xff00a2ff>任何生产技能<color=0xffffffff>，均会消耗角色的<color=0xff00a2ff>活力值<color=0xffffffff>。\\n    活力值可以在<color=0xff00a2ff>角色界面<color=0xffffffff>内查看。\\n    角色的<color=0xff00a2ff>等级越高<color=0xffffffff>，<color=0xff00a2ff>活力值越高<color=0xffffffff>。\\n    只要不断<color=0xff00a2ff>在线<color=0xffffffff>，角色的活力值就会<color=0xff00a2ff>缓慢恢复<color=0xffffffff>。\\n    同时游戏内有<color=0xff00a2ff>恢复活力值的药品<color=0xffffffff>。\\n    \\n    <color=0xffffae00>[小提示：若您对生产方面的内容全部有所了解之后，可点击“我已了解”选项继续任务流程。]<color=0xffffffff>");
		elseif( step == QuestTalkBegin+9 )then
			Quest.UpdateQuest(10015);
		end
	end
end

ProduceMaster = {}
ProduceMaster["OnTalk"] = OnTalk
ProduceMaster["OnProduceSkillTalk"] = OnProduceSkillTalk
ProduceMaster["OnCompleteQuestTalk"] = OnCompleteQuestTalk
ProduceMaster["OnScenarioTalk"] = OnScenarioTalk
