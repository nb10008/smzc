local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610063);
	Quest.SetCompleteOptions(5610063);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>祝福灵童：<color=0xffffffff><font=龙字体,0,17,20>\\n      五星红旗迎风飘扬，胜利歌声多么响亮……");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30831)then
			Quest.CloseNPCTalk();
		    Quest.AcceptQuestTalk(30831,"<font=龙字体,0,19,20><color=0xfffcf157>祝福灵童：<color=0xffffffff><font=龙字体,0,17,20>\\n      难得来三界一次，路上我只顾着欣赏山水，不小心把国庆礼花给弄丢了，怎么办？师父脾气不好，知道了肯定会责罚我的。看起来你挺面善的，能帮我找回礼花吗？我会感谢你们全家的！");
        elseif( id == 30832)then
			Quest.CloseNPCTalk();
			Quest.AcceptQuestTalk(30832,"<font=龙字体,0,19,20><color=0xfffcf157>祝福灵童：<color=0xffffffff><font=龙字体,0,17,20>\\n      国庆期间，30级以上的玩家每天都可以来找我领取礼包，只有国庆期间哦");
		end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30831)then
			Quest.CloseNPCTalk();
	        Quest.CompleteQuestTalk(30831,"<font=龙字体,0,19,20><color=0xfffcf157>祝福灵童：<color=0xffffffff><font=龙字体,0,17,20>\\n      就这么点吗？算了，看你也不容易，就给你点奖励吧。明天要带来更多的礼花哦。");
        elseif( id == 30832)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30832,"<font=龙字体,0,19,20><color=0xfffcf157>祝福灵童：<color=0xffffffff><font=龙字体,0,17,20>\\n      今天的礼包给你了，想要的话明天再来吧！");
        end
end

Rzhufulingtong = {}
Rzhufulingtong["OnTalk"] = OnTalk
Rzhufulingtong["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rzhufulingtong["OnCompleteQuestTalk"] = OnCompleteQuestTalk
