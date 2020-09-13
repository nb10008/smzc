
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010099);
	Quest.SetCompleteOptions(5010099);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>汉特：<color=0xffffffff>\\n如果不想碌碌无为，就要不断努力！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20339)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20339,"<font=龙字体,0,19,20><color=0xfffcf157>汉特：<color=0xffffffff>\\n    继续消灭这些魔族吧，当你的等级提升至48级的时候就可以从我后面的山路回到天雪镇了，有人在那里等着你呢。");
	elseif( id == 20347)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20347,"<font=龙字体,0,19,20><color=0xfffcf157>汉特：<color=0xffffffff>\\n    去看看那些雪地狂野人为什么变得这样。");

     
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20338)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20338,"<font=龙字体,0,19,20><color=0xfffcf157>汉特：<color=0xffffffff>\\n        表现的不错，可这还不够呢。");
	elseif( id == 20347)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20347,"<font=龙字体,0,19,20><color=0xfffcf157>汉特：<color=0xffffffff>\\n        都是魔族惹的祸。");
   

       end

end

Rhante = {}
Rhante["OnTalk"] = OnTalk
Rhante["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rhante["OnCompleteQuestTalk"] = OnCompleteQuestTalk