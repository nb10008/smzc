
local function OnTalk()
	Quest.ClearNPCTalkOptions();  --清除对话框选项
	Quest.SetAcceptOptions(5511012);
	Quest.SetCompleteOptions(5511012);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>游客：<color=0xffffffff>\\n旅游的步伐是不能停止的；但是当遇到危险的时候，还是要停下来看一看。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	 if( id == 50032)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(50032,"<font=龙字体,0,19,20><color=0xfffcf157>游客：<color=0xffffffff>\\n我初次来到这个森林，感觉有很多危险，而我又不了解这里的状况，你能帮我打探下这里的状况么？会有酬劳给你的。");
	 elseif( id == 50033)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(50033,"<font=龙字体,0,19,20><color=0xfffcf157>游客：<color=0xffffffff>\\我还是感觉有很多危险，你能再帮我打探下这里的状况么？会有酬劳给你的。");
	 elseif( id == 50034)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(50034,"<font=龙字体,0,19,20><color=0xfffcf157>游客：<color=0xffffffff>\\n我听说这里还有很多地方你没有去过，你再去帮我打探下这里的状况吧。同样会有酬劳给你的。");
	 end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	 if( id == 50032)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(50032,"<font=龙字体,0,19,20><color=0xfffcf157>游客：<color=0xffffffff>\\n恩~我知道了些情况。谢谢你，这是你的酬劳。");
	 elseif( id == 50033)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(50033,"<font=龙字体,0,19,20><color=0xfffcf157>游客：<color=0xffffffff>\\n我对这里有了进一步的了解，这都是你的功劳啊。");
	 elseif( id == 50034)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(50034,"<font=龙字体,0,19,20><color=0xfffcf157>游客：<color=0xffffffff>\\n我对这里终于有了大致的了解，可以放心游玩了。");
	end

end

Ryouke = {}
Ryouke["OnTalk"] = OnTalk
Ryouke["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryouke["OnCompleteQuestTalk"] = OnCompleteQuestTalk