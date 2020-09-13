
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010098);
	Quest.SetCompleteOptions(5010098);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>力神戈登：<color=0xffffffff>\\n羡慕我的巨力吗？想借用我的力量就要信仰我！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20337)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20337,"<font=龙字体,0,19,20><color=0xfffcf157>力神戈登：<color=0xffffffff>\\n    这路上全都是魔族，看到那些天魔妖女了吧，这只是先锋部队而已。我们先来把道路清理一下吧。");
        elseif( id == 20338)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20338,"<font=龙字体,0,19,20><color=0xfffcf157>力神戈登：<color=0xffffffff>\\n    到魔冥岭去吧，杀掉那里的魔族，然后去找汉特，他会给你指清道路的。");
     
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20336)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20336,"<font=龙字体,0,19,20><color=0xfffcf157>力神戈登：<color=0xffffffff>\\n        在这里一定要小心！");
        elseif( id == 20337)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20337,"<font=龙字体,0,19,20><color=0xfffcf157>力神戈登：<color=0xffffffff>\\n        我还真是看好你啊。");

       

       end

end

Rgedeng = {}
Rgedeng["OnTalk"] = OnTalk
Rgedeng["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rgedeng["OnCompleteQuestTalk"] = OnCompleteQuestTalk