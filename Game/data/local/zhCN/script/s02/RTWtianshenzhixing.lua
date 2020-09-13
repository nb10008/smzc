
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(7000005);
	Quest.SetCompleteOptions(7000005);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神執政官：<color=0xffffffff><font=龙字体,0,17,20>\\n魔物最近越來越猖狂了，主宰下令讓我們廣發魔神裝的材料，讓你們這些小小天神能夠有能力抗衡強大的魔物。\\n每日任務重置時間為晚上00:00");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 55001)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(55001,"<font=龙字体,0,19,20><color=0xfffcf157>天神執政官：<color=0xffffffff><font=龙字体,0,17,20>\\n給我40個三等精鐵及50金遊戲幣，我就把魔神之魂給你！");
        elseif( id == 55002)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(55002,"<font=龙字体,0,19,20><color=0xfffcf157>天神執政官：<color=0xffffffff><font=龙字体,0,17,20>\\n給我20個三等玄鐵及200金遊戲幣，我就把魔神之魄給你！");
        elseif( id == 55003)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(55003,"<font=龙字体,0,19,20><color=0xfffcf157>天神執政官：<color=0xffffffff><font=龙字体,0,17,20>\\n給我10個三等幻鋼及800金遊戲幣，我就把魔神之靈給你！");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 55001)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(55001,"<font=龙字体,0,19,20><color=0xfffcf157>天神執政官：<color=0xffffffff><font=龙字体,0,17,20>\\n很好，拿著這些材料去鬥戰天城(936,645)處找兌換商領取魔神裝吧！這樣一來，你的實力就會大大的提升！");
        elseif( id == 55002)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(55002,"<font=龙字体,0,19,20><color=0xfffcf157>天神執政官：<color=0xffffffff><font=龙字体,0,17,20>\\n很好，拿著這些材料去鬥戰天城(936,645)處找兌換商領取魔神裝吧！這樣一來，你的實力就會大大的提升！");
        elseif( id == 55003)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(55003,"<font=龙字体,0,19,20><color=0xfffcf157>天神執政官：<color=0xffffffff><font=龙字体,0,17,20>\\n很好，拿著這些材料去鬥戰天城(936,645)處找兌換商領取魔神裝吧！這樣一來，你的實力就會大大的提升！");
       end


end

RTWtianshenzhixing = {}
RTWtianshenzhixing["OnTalk"] = OnTalk
RTWtianshenzhixing["OnAcceptQuestTalk"] = OnAcceptQuestTalk
RTWtianshenzhixing["OnCompleteQuestTalk"] = OnCompleteQuestTalk
