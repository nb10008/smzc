
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010092);
	Quest.SetCompleteOptions(5010092);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n白龙谷的魔族好像越来越多了，这里有点支持不住了。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20315)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20315,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n    在白龙谷的南边有一群被恶魔控制的天兵，没有别的办法，只好将它们全部消灭。");
        elseif( id == 20316)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20316,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n    白龙谷的西部，被一群疯狂的狼人占据着，我们需要将它们赶走。");
        elseif( id == 20317)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20317,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n    在白龙谷的狼人中，你可能会看到一些个头很大的家伙，那是狼人的头头，见到这些家伙一定不要手软。");
        elseif( id == 20318)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20318,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n    那些被魔族控制的天兵中，还有一些精英，曾经都是天兵将领，如今也一起堕落了。请将它们净化掉吧。");	
        elseif( id == 20319)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20319,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n    白龙谷已经平安无事了，天神若将等级提升至<C>38</C>级，即可以前往<N>福临镇</N>了。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20314)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20314,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n        我是这次任务的头领，天神也来助我们一臂之力了。");
        elseif( id == 20315)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20315,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n        天神出马果然不同凡响啊。");
        elseif( id == 20316)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20316,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n        这些狼人数量还真是惊人啊。");
        elseif( id == 20317)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20317,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n        真是辛苦了，这些家伙很厉害吧。");
        elseif( id == 20318)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20318,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n        虽然不忍下手，可还是做了。这都是魔族的错。");
        elseif( id == 30337)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30337,"<font=龙字体,0,19,20><color=0xfffcf157>元寒：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");
  
       end

end

Ryuanhan = {}
Ryuanhan["OnTalk"] = OnTalk
Ryuanhan["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryuanhan["OnCompleteQuestTalk"] = OnCompleteQuestTalk