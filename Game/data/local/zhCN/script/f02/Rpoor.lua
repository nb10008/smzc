
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5100001);
	Quest.SetCompleteOptions(5100001);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>被囚禁的平民：<color=0xffffffff>\\n我被他们抓来这里，他们把我关在这里，派了人看守，我会被杀死的。");
	
end



local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 29000)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(29000,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff>\\n在坦伯特，你还是可以看到七花之海的美丽，但那些生物却已经被魔物侵蚀了。我需要一些材料做出抵御魔族的药水，那些幻蝶就是最好的药粉。");
        end

end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 29000)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(29000,"<font=龙字体,0,19,20><color=0xfffcf157>被囚禁的平民：<color=0xffffffff>\\n伟大的人，请你救我出去吧，我会给你报酬的。");
	end


end

Rpoor = {}
Rpoor["OnTalk"] = OnTalk
Rpoor["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rpoor["OnCompleteQuestTalk"] = OnCompleteQuestTalk