
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010198);
	Quest.SetCompleteOptions(5010198);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>僵尸博士：<color=0xffffffff><font=龙字体,0,17,20>\\n僵尸和植物不共戴天~！！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 50005)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50005,"<font=龙字体,0,19,20><color=0xfffcf157>僵尸博士：<color=0xffffffff><font=龙字体,0,17,20>\\n我超级憎恨毁灭菇，这臭蘑菇曾经给我带来了巨大的伤害，你只要帮我拿<C>3</C>个<I>毁灭菇</I>来，我愿意用我的僵尸药水来和你交换！不过我制造这僵尸药水的能力有限，所以<T>每天你只能完成1次</T>。");
        elseif( id == 50006)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50006,"<font=龙字体,0,19,20><color=0xfffcf157>僵尸博士：<color=0xffffffff><font=龙字体,0,17,20>\\n怎么？感觉<I>低级僵尸药水</I>效果不够？那就用5个来和我换1个<I>高级僵尸药水</I>！");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 50005)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50005,"<font=龙字体,0,19,20><color=0xfffcf157>僵尸博士：<color=0xffffffff><font=龙字体,0,17,20>\\n哈哈哈哈~煮着吃？炖着吃？生吃？炒着吃？我得好好考虑考虑！这瓶<I>僵尸药水</I>你拿去吧！");
        elseif( id == 50006)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50006,"<font=龙字体,0,19,20><color=0xfffcf157>僵尸博士：<color=0xffffffff><font=龙字体,0,17,20>\\n希望你早日加入我们僵尸军团！本团目前只有我1个成员……很寂寞啊！");
       end

end

Rjiangshiboshi = {}
Rjiangshiboshi["OnTalk"] = OnTalk
Rjiangshiboshi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rjiangshiboshi["OnCompleteQuestTalk"] = OnCompleteQuestTalk