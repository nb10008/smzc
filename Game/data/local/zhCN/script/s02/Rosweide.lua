
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010227);
	Quest.SetCompleteOptions(5010227);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n每日都有审判，等待最终的审判之日。"  );

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 29006)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29006,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  黑暗学院里面的恶魔，准备接受我的审判吧！到黑暗学院去，只要完成其中一项任务，我就认可你的能力，将审判者的奖励送给你。 ");
	elseif( id == 29007)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29007,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  白银魔窟里面的恶魔，准备接受我的审判吧！到白银魔窟去，只要完成其中一项任务，我就认可你的能力，将审判者的奖励送给你。 ");
	elseif( id == 29008)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29008,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  冰寒魔域里面的恶魔，准备接受我的审判吧！到冰寒魔域去，只要完成其中一项任务，我就认可你的能力，将审判者的奖励送给你。 ");
	elseif( id == 29009)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29009,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  裁决之所里面的恶魔，准备接受我的审判吧！到裁决之所去，只要完成其中一项任务，我就认可你的能力，将审判者的奖励送给你。 ");
	elseif( id == 29010)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29010,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  黑暗学院里面的恶魔，准备接受我的审判吧！到黑暗学院去，只要完成其中一项任务，我就认可你的能力，将审判者的奖励送给你。 ");
	elseif( id == 29011)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29011,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  迷幻仙宫里面的恶魔，准备接受我的审判吧！到迷幻仙宫去，只要完成其中一项任务，我就认可你的能力，将审判者的奖励送给你。 ");
	elseif( id == 29012)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29012,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  幽冥神殿里面的恶魔，准备接受我的审判吧！到幽冥神殿去，只要完成其中一项任务，我就认可你的能力，将审判者的奖励送给你。 ");
	elseif( id == 29013)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29013,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  失忆回廊里面的恶魔，准备接受我的审判吧！到失忆回廊去，只要完成其中一项任务，我就认可你的能力，将审判者的奖励送给你。 ");
       end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 29006)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(29006,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  每天审判一次就可以了，一定要持之以恒，不能让魔族有喘息的机会。 ");
        elseif( id == 29007)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(29007,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  每天审判一次就可以了，一定要持之以恒，不能让魔族有喘息的机会。 ");
	elseif( id == 29008)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(29008,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  每天审判一次就可以了，一定要持之以恒，不能让魔族有喘息的机会。 ");
	elseif( id == 29009)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(29009,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  每天审判一次就可以了，一定要持之以恒，不能让魔族有喘息的机会。 ");
	elseif( id == 29010)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(29010,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  每天审判一次就可以了，一定要持之以恒，不能让魔族有喘息的机会。 ");
	elseif( id == 29011)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(29011,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  每天审判一次就可以了，一定要持之以恒，不能让魔族有喘息的机会。 ");
	elseif( id == 29012)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(29012,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  每天审判一次就可以了，一定要持之以恒，不能让魔族有喘息的机会。 ");
	elseif( id == 29013)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(29013,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n  每天审判一次就可以了，一定要持之以恒，不能让魔族有喘息的机会。 ");
	elseif( id == 30867)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30867,QuestTalkBegin+1,"确定" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,30867,QuestTalkBegin+2,"返回" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>审判者奥斯威德：<color=0xffffffff>\\n    我正念叨着今年怎么没有人给我拜年来着呢，你的到来让我很开心，回去替我好好谢谢春节使者米莲妮。");	        
		elseif( step == QuestTalkBegin+1 )then
			Quest.UpdateQuest(30867);
		elseif( step == QuestTalkBegin+2 )then
			Quest.CloseNPCTalk();
		end
	end

end

Rosweide = {}
Rosweide["OnTalk"] = OnTalk
Rosweide["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rosweide["OnCompleteQuestTalk"] = OnCompleteQuestTalk