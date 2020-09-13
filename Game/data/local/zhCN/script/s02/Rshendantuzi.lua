
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600020);
	Quest.SetCompleteOptions(5600020);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞兔子：<color=0xffffffff>\\n圣诞要过得愉快啊~~");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30723)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30723,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞兔子：<color=0xffffffff>\\n快来领取经验加成的奖励吧！");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30723)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30723,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞兔子：<color=0xffffffff>\\n完成任务即可获得1.5倍的经验奖励");

       end

end

Rshendantuzi = {}
Rshendantuzi["OnTalk"] = OnTalk
Rshendantuzi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rshendantuzi["OnCompleteQuestTalk"] = OnCompleteQuestTalk