local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5800084);
	Quest.SetCompleteOptions(5800084);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>妖精芙希希：<color=0xffffffff>\\n辛勤的劳作是可以得到丰盛的回报的。");

end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40051)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40051,"<font=龙字体,0,19,20><color=0xfffcf157>妖精芙希希：<color=0xffffffff>\\n我感觉最近这几天全身无力，可能是到了那个时期了吧。好想吃妖精庄园的火桃酱面包啊，你能不能帮我去一趟妖精农场，找到那里的庄园主艾欧里奥。要前往农场的话，可以去斗战天城找到妖精农场传送人。");
       
        end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 40056)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(40056,"<font=龙字体,0,19,20><color=0xfffcf157>妖精芙希希：<color=0xffffffff><font=龙字体,0,17,20>\\n这……真的是妖精农场的火桃酱面包，真是想死我了。太感谢你了，我觉得看到火桃酱面包我的力气就恢复了一半了，这些算是我对你的报答吧。");
	end
end

Rfuxx = {}
Rfuxx["OnTalk"] = OnTalk
Rfuxx["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rfuxx["OnCompleteQuestTalk"] = OnCompleteQuestTalk