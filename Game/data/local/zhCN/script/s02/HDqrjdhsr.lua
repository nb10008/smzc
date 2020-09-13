
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600037);
	Quest.SetCompleteOptions(5600037);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>情人节兑换券使者：<color=0xffffffff>\\n情人节到了，我这里有好东西哦！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30918)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30918,"<font=龙字体,0,19,20><color=0xfffcf157>情人节兑换券使者：<color=0xffffffff>\\n心动了吗？心动不如行动，快带着情人节超值兑换券来换礼物吧！");
				elseif( id == 30919)then
								Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30919,"<font=龙字体,0,19,20><color=0xfffcf157>情人节兑换券使者：<color=0xffffffff>\\n心动了吗？心动不如行动，快带着情人节超值兑换券来换礼物吧！");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30918)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30918,"<font=龙字体,0,19,20><color=0xfffcf157>情人节兑换券使者：<color=0xffffffff>\\n礼物收到了吗？情人节快乐！");
				elseif( id == 30919)then
							Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30919,"<font=龙字体,0,19,20><color=0xfffcf157>情人节兑换券使者：<color=0xffffffff>\\n礼物收到了吗？情人节快乐！");
       end

end

HDqrjdhsr = {}
HDqrjdhsr["OnTalk"] = OnTalk
HDqrjdhsr["OnAcceptQuestTalk"] = OnAcceptQuestTalk
HDqrjdhsr["OnCompleteQuestTalk"] = OnCompleteQuestTalk