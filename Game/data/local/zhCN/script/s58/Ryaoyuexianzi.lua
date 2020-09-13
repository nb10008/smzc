local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600071);
	Quest.SetCompleteOptions(5600071);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>邀月仙子：<color=0xffffffff><font=龙字体,0,17,20>\\n      曾经的此地是多么的祥和安宁……");
end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30829)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30829,"<font=龙字体,0,19,20><color=0xfffcf157>邀月仙子：<color=0xffffffff><font=龙字体,0,17,20>\\n      找到那些散落在此地的月灵石碎片，据说藏匿在雕像中的树妖身上有这种东西。");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30829)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30829,"<font=龙字体,0,19,20><color=0xfffcf157>邀月仙子：<color=0xffffffff><font=龙字体,0,17,20>\\n      你果然是不负众望！");
	end
end

Ryaoyuexianzi = {}
Ryaoyuexianzi["OnTalk"] = OnTalk
Ryaoyuexianzi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryaoyuexianzi["OnCompleteQuestTalk"] = OnCompleteQuestTalk