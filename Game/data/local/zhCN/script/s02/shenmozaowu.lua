
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5011017);
	Quest.SetCompleteOptions(5011017);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神魔造物主：<color=0xffffffff><font=龙字体,0,17,20>\\n那些魔罐就是我创造的");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30118)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30118,"<font=龙字体,0,19,20><color=0xfffcf157>神魔造物主：<color=0xffffffff><font=龙字体,0,17,20>\\n快去完成百鬼试炼任务吧，之后你会得到一个我创造的神奇魔罐。");
        elseif( id == 30119)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30119,"<font=龙字体,0,19,20><color=0xfffcf157>神魔造物主：<color=0xffffffff><font=龙字体,0,17,20>\\n快去完成百鬼试炼任务吧，之后你会得到一个我创造的神奇魔罐。");
        elseif( id == 30120)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30120,"<font=龙字体,0,19,20><color=0xfffcf157>神魔造物主：<color=0xffffffff><font=龙字体,0,17,20>\\n快去完成百鬼试炼任务吧，之后你会得到一个我创造的神奇魔罐。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30118)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30118,"<font=龙字体,0,19,20><color=0xfffcf157>神魔造物主：<color=0xffffffff><font=龙字体,0,17,20>\\n你要相信我伟大的创造力不仅仅如此！");
        elseif( id == 30119)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30119,"<font=龙字体,0,19,20><color=0xfffcf157>神魔造物主：<color=0xffffffff><font=龙字体,0,17,20>\\n你要相信我伟大的创造力不仅仅如此！");
        elseif( id == 30120)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30120,"<font=龙字体,0,19,20><color=0xfffcf157>神魔造物主：<color=0xffffffff><font=龙字体,0,17,20>\\n你要相信我伟大的创造力不仅仅如此！");
        
       end


end

shenmozaowu = {}
shenmozaowu["OnTalk"] = OnTalk
shenmozaowu["OnAcceptQuestTalk"] = OnAcceptQuestTalk
shenmozaowu["OnCompleteQuestTalk"] = OnCompleteQuestTalk