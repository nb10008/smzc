local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(6040055);
	Quest.SetCompleteOptions(6040055);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天使呼啦啦：<color=0xffffffff><font=龙字体,0,17,20>\\n      升级对我来说是一件神速的事情。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30797)then
			Quest.CloseNPCTalk();
		    Quest.AcceptQuestTalk(30797,"<font=龙字体,0,19,20><color=0xfffcf157>天使呼啦啦：<color=0xffffffff><font=龙字体,0,17,20>\\n      这是千载难逢的机会啊，快去完成悬赏任务吧！注意：<T>等级超过85将不能给你提升等级</T>！");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30797)then
			Quest.CloseNPCTalk();
	        Quest.CompleteQuestTalk(30797,"<font=龙字体,0,19,20><color=0xfffcf157>天使呼啦啦：<color=0xffffffff><font=龙字体,0,17,20>\\n      马上为你提升等级。注意：<T>等级超过85将不能给你提升等级</T>。");
        end
end

tianshihulala = {}
tianshihulala["OnTalk"] = OnTalk
tianshihulala["OnAcceptQuestTalk"] = OnAcceptQuestTalk
tianshihulala["OnCompleteQuestTalk"] = OnCompleteQuestTalk
