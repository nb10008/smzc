
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600101);
	Quest.SetCompleteOptions(5600101);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n你知道腊八粥的起源么？");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30738)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30738,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n快去帮我收集腊八粥的材料吧，要不就没有腊八粥喝了。");
	elseif( id == 30739)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30739,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n这么多人，当然需要更多的材料了！");
	elseif( id == 30740)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30740,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n继续努力收集腊八粥材料吧。");
	elseif( id == 30741)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30741,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n腊八粥要换一种做法");
	elseif( id == 30742)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30742,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n想要得到更多，就需求更多");
	elseif( id == 30743)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30743,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n这是最后的需求了。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30738)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30738,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n材料来了，这就去煮粥。");
	elseif( id == 30739)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30739,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n恩~还是不够啊！");
	elseif( id == 30740)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30740,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n你的实力真强，可以收集这么多的材料。");
	elseif( id == 30741)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30741,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n煮出来的粥一定比传统的好喝。");
	elseif( id == 30742)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30742,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n这样一来喝粥的人就可以多一些了。");
	elseif( id == 30743)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30743,"<font=龙字体,0,19,20><color=0xfffcf157>腊八粥厨师：<color=0xffffffff>\\n这个腊八粥从此扬名于世了。");
       end

end

Rlabachushi = {}
Rlabachushi["OnTalk"] = OnTalk
Rlabachushi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rlabachushi["OnCompleteQuestTalk"] = OnCompleteQuestTalk