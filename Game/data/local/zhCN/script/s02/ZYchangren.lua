
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010192);
	Quest.SetCompleteOptions(5010192);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n如果你喜欢与敌人展开兵刃战，那选择长刃绝对错不了。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取	    
        if( id == 41013)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41013,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n看来你是想换个职业，如果你已经决定了。若要转换职业，首先要变为一转职业并需要一个一转印记，然后再去完成对应的二转任务即可。我会将你的职业改为神兵，如果你需要的话。");	
        elseif( id == 41017)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41017,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n你已经转职成为神兵，现在回去斗神之王处接取新的任务吧。");	
	elseif( id == 41021)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41021,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n你只要把二转印记交给我，就可以转换你的职业成为王者了。");
	elseif( id == 41022)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(41022,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n你只要把二转印记交给我，就可以转换你的职业成为神甲了。");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 41000)then
			if( step == QuestTalkBegin )then
				Quest.ClearNPCTalkOptions();
				Quest.SetNPCTalkOption( EOT_CompleteQuest,41000,QuestTalkBegin+1,"我知道了" );
				Quest.SetNPCTalkOption( EOT_CompleteQuest,41000,QuestTalkBegin+2,"取消" );
				Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff><font=龙字体,0,17,20>\\n    近身战斗无人能敌的职业，物理攻击的王者，拥有最高的物理防御力。");
			elseif( step == QuestTalkBegin+1 )then
				Quest.UpdateQuest(41000);
			elseif( step == QuestTalkBegin+2 )then
				OnTalk();
			end


	elseif( id == 41001)then
			if( not Quest.QuestCanComplete(41001) ) then

				if( step == QuestTalkBegin )then
					Quest.ClearNPCTalkOptions();
					Quest.SetNPCTalkOption( EOT_CompleteQuest,41001,QuestTalkBegin+1,"好的，我已经决定了" );
					Quest.SetNPCTalkOption( EOT_CompleteQuest,41001,QuestTalkBegin+2,"我要在想一想" );
					Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n    既然已经决定了，那就将武之契交给我吧，我会将神兵的力量赋予你！记住,神兵是近身战斗无人能敌的职业，物理攻击的王者，拥有最高的物理防御力。");
				elseif( step == QuestTalkBegin+1 )then
					Quest.ClearNPCTalkOptions();
					Quest.SetNPCTalkOption( EOT_CompleteQuest,41001,QuestTalkBegin+3,"确定" );
					Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n    那么请将武之契交给我吧。");
				elseif( step == QuestTalkBegin+3 )then
					Quest.UpdateQuest(41001);
				elseif( step == QuestTalkBegin+2 ) then
					Quest.CloseNPCTalk();
				end
			else

				Quest.CloseNPCTalk();
				Quest.CompleteQuestTalk(41001,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n    那么神兵的力量你马上就可以感受到了。 \\n    <color=0xffff00ff>提示：因只可拥有一种职业，若同时接取其他转职任务，在完成此任务后请删除即可<color=0xffffffff>。");

			end
        elseif( id == 41005)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41005,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n没想到你真的成功了，那么现在，我正式将<color=0xff80ff00>王者<color=0xffffffff>的能力赋予你，你一定要好好的运用自己的能力。");
        elseif( id == 41006)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41006,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n没想到你真的成功了，那么现在，我正式将<color=0xff80ff00>神甲<color=0xffffffff>的能力赋予你，你一定要好好的运用自己的能力。");
        elseif( id == 41013)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,41013,QuestTalkBegin+1,"交付一转印记" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,41013,QuestTalkBegin+2,"我再想想" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n    转职后所有的技能点数也会被重置，你真的决定转职为神兵的话，就把一转印记交给我吧。");
		elseif( step == QuestTalkBegin+2 ) then
			Quest.CloseNPCTalk();
		elseif( step == QuestTalkBegin+1 )then
			Quest.ClearNPCTalkOptions();
			Quest.CompleteQuestTalk(41013,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n    点击完成，你就转职为神兵了。");
		end
	elseif( id == 41021)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41021,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n那么请将二转印记交给我吧。");
	elseif( id == 41022)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(41022,"<font=龙字体,0,19,20><color=0xfffcf157>长刃之灵：<color=0xffffffff>\\n那么请将二转印记交给我吧。");
	end

end

ZYchangren = {}
ZYchangren["OnTalk"] = OnTalk
ZYchangren["OnAcceptQuestTalk"] = OnAcceptQuestTalk
ZYchangren["OnCompleteQuestTalk"] = OnCompleteQuestTalk