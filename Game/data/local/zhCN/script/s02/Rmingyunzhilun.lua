
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5511015);
	Quest.SetCompleteOptions(5511015);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>命运之轮守护者：<color=0xffffffff>\\n如果你找到了<I>圣魔珠</I>，可以再加上<I>命运卷</I>在我这里兑换<color=0xffff80ff>命运之轮<color=0xffffffff>。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30113)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30113,"<font=龙字体,0,19,20><color=0xfffcf157>命运之轮守护者：<color=0xffffffff>\\n    你要兑换命运之轮吗？那么就去收集<I>圣魔珠</I>和<I>命运卷</I>，就可兑换<C>1</C>个<color=0xffff80ff>命运之轮<color=0xffffffff>。");
        elseif( id == 30114)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30114,"<font=龙字体,0,19,20><color=0xfffcf157>命运之轮守护者：<color=0xffffffff>\\n    你要兑换命运之轮吗？那么就去收集<I>圣魔珠</I>和<I>命运卷</I>，就可兑换<C>5</C>个<color=0xffff80ff>命运之轮<color=0xffffffff>。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30113)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30113,"<font=龙字体,0,19,20><color=0xfffcf157>命运之轮守护者：<color=0xffffffff>\\n    那么祝你好运，看看你能得到什么好运吧。");
        elseif( id == 30114)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30114,"<font=龙字体,0,19,20><color=0xfffcf157>命运之轮守护者：<color=0xffffffff>\\n    那么祝你好运，看看你能得到什么好运吧。");
	
       end

end

Rmingyunzhilun = {}
Rmingyunzhilun["OnTalk"] = OnTalk
Rmingyunzhilun["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rmingyunzhilun["OnCompleteQuestTalk"] = OnCompleteQuestTalk