
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010078);
	Quest.SetCompleteOptions(5010078);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>岩神奥利佛：<color=0xffffffff>\\n岩石愤怒了，魔族必须付出代价，为这里的焦土。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20207)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20207,"<font=龙字体,0,19,20><color=0xfffcf157>岩神奥利佛：<color=0xffffffff>\\n        因为魔族，这里变成了焦土，现在他们又来这里破坏，这些巨石已经愤怒了。他们需要安静的睡一会。");
        elseif( id == 20208)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20208,"<font=龙字体,0,19,20><color=0xfffcf157>岩神奥利佛：<color=0xffffffff>\\n        这些可恶的家伙，我是说那些讨厌的白龙，总是抓住岩石扔到熔岩中。我可不能让石元素受到威胁。");
        elseif( id == 20209)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20209,"<font=龙字体,0,19,20><color=0xfffcf157>岩神奥利佛：<color=0xffffffff>\\n        这附近有个魔女一直控制着这些石元素，我们必须找到她，并且制服她。");
        elseif( id == 20210)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20210,"<font=龙字体,0,19,20><color=0xfffcf157>岩神奥利佛：<color=0xffffffff>\\n        我们也要加快脚步，这附近的悬赏任务应该可以让你很快的到达<C>26</C>级，在那之后你就动身前往<N>夏尔绿谷</N>，去找那里的<D>夏尔</D>。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20206)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20206,"<font=龙字体,0,19,20><color=0xfffcf157>岩神奥利佛：<color=0xffffffff>\\n        这些可恶的魔族……你是来找我的么。");
        elseif( id == 20207)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20207,"<font=龙字体,0,19,20><color=0xfffcf157>岩神奥利佛：<color=0xffffffff>\\n        不能就这么看着这里被魔族毁掉。");
        elseif( id == 20208)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20208,"<font=龙字体,0,19,20><color=0xfffcf157>岩神奥利佛：<color=0xffffffff>\\n        谢谢你为岩石们做的事情。");
        elseif( id == 20209)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20209,"<font=龙字体,0,19,20><color=0xfffcf157>岩神奥利佛：<color=0xffffffff>\\n        这个魔女确是魔族的人，我想附近应该还有个大家伙。");
       end

end

Raolifo = {}
Raolifo["OnTalk"] = OnTalk
Raolifo["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Raolifo["OnCompleteQuestTalk"] = OnCompleteQuestTalk