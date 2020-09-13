
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010100);
	Quest.SetCompleteOptions(5010100);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天兵伊恩：<color=0xffffffff>\\n兄弟们！安息吧！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20341)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20341,"<font=龙字体,0,19,20><color=0xfffcf157>天兵伊恩：<color=0xffffffff>\\n    我们被魔族袭击，那些天兵全都被魔族控制了，我很幸运的逃了出来。那些天兵已经变成魔族了，不能让他们再危害到其他人了。");
        elseif( id == 20342)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20342,"<font=龙字体,0,19,20><color=0xfffcf157>天兵伊恩：<color=0xffffffff>\\n    在这里的南部，因为魔族的关系出现了很多魔灵，我们快去清理一下吧。");
        elseif( id == 20343)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20343,"<font=龙字体,0,19,20><color=0xfffcf157>天兵伊恩：<color=0xffffffff>\\n    我们本来是打算去南边的北风岗，消灭那里的魔族，可还没到那里就被魔族袭击了，这件事只好交给天神来做了。");
        elseif( id == 20344)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20344,"<font=龙字体,0,19,20><color=0xfffcf157>天兵伊恩：<color=0xffffffff>\\n    可以回去复命了，你也一起回去吧，不过要等你等级到达50级的时候。");     
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20340)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20340,"<font=龙字体,0,19,20><color=0xfffcf157>天兵伊恩：<color=0xffffffff>\\n        我确实需要你的帮助。");
        elseif( id == 20341)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20341,"<font=龙字体,0,19,20><color=0xfffcf157>天兵伊恩：<color=0xffffffff>\\n        真是得救了。");   
        elseif( id == 20342)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20342,"<font=龙字体,0,19,20><color=0xfffcf157>天兵伊恩：<color=0xffffffff>\\n        天神真是太厉害了。"); 
        elseif( id == 20343)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20343,"<font=龙字体,0,19,20><color=0xfffcf157>天兵伊恩：<color=0xffffffff>\\n        总算圆满完成了任务……"); 	      
       end

end

Ryien = {}
Ryien["OnTalk"] = OnTalk
Ryien["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryien["OnCompleteQuestTalk"] = OnCompleteQuestTalk