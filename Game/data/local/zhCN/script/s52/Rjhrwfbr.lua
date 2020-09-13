
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600063);
	Quest.SetCompleteOptions(5600063);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>结婚任务发布人<color=0xffffffff><font=龙字体,0,17,20>\\n        请接受我的祝福，希望你们在以后的日子里能够不离不弃，永结同心。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30377)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30377,"<font=龙字体,0,19,20><color=0xfffcf157>结婚任务发布人<color=0xffffffff><font=龙字体,0,17,20>\\n        是不是觉得自己很幸福呢？如果是的话，就拥抱你的爱人吧，不要害羞，大家都会祝福你们的。");
        elseif( id == 30378)then
                Quest.CloseNPCTalk();
								Quest.AcceptQuestTalk(30378,"<font=龙字体,0,19,20><color=0xfffcf157>结婚任务发布人<color=0xffffffff><font=龙字体,0,17,20>\\n        以后的生活中，你们要相互信任，互相扶持，祝你们白头偕老，幸福美满。嘿，我要是你，就亲自己的爱人一下！");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30377)then
				      Quest.CloseNPCTalk();
				      Quest.CompleteQuestTalk(30377,"<font=龙字体,0,19,20><color=0xfffcf157>结婚任务发布人<color=0xffffffff><font=龙字体,0,17,20>\\n        把这些东西拿去分给你的亲朋好友吧。");
       elseif( id == 30378)then
				      Quest.CloseNPCTalk();
				      Quest.CompleteQuestTalk(30378,"<font=龙字体,0,19,20><color=0xfffcf157>结婚任务发布人<color=0xffffffff><font=龙字体,0,17,20>\\n        来，这些东西估计你们以后会用得到的。");

       end


end

Rjhrwfbr = {}
Rjhrwfbr["OnTalk"] = OnTalk
Rjhrwfbr["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rjhrwfbr["OnCompleteQuestTalk"] = OnCompleteQuestTalk
