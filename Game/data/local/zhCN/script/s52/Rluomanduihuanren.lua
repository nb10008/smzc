local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600035);
	Quest.SetCompleteOptions(5600035);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>罗曼称号兑换人：<color=0xffffffff><font=龙字体,0,17,20>\\n欢迎来到博博罗曼村~~这里到处都充满了爱~~");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30775)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30775,"<font=龙字体,0,19,20><color=0xfffcf157>罗曼称号兑换人：<color=0xffffffff>\\n若你能收集<C>50</C>个<I>同志的祝福</I>，我就将这个<I>同志</I>的称号送给你。");
	elseif( id == 30776)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30776,"<font=龙字体,0,19,20><color=0xfffcf157>罗曼称号兑换人：<color=0xffffffff>\\n若你能收集<C>50</C>个<I>真爱情书</I>，我就将这<I>情圣</I>或<I>花魁</I>称号送给你。");
	elseif( id == 30777)then
		 Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30777,"<font=龙字体,0,19,20><color=0xfffcf157>罗曼称号兑换人：<color=0xffffffff>\\n若你能收集<C>50</C>个<I>蕾丝边</I>，我就将这<I>蕾丝边</I>的称号送给你。");
	
        end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30775)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30775,"<font=龙字体,0,19,20><color=0xfffcf157>罗曼称号兑换人：<color=0xffffffff><font=龙字体,0,17,20>\\n你果然是个好同志。");
	elseif( id == 30776)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30776,"<font=龙字体,0,19,20><color=0xfffcf157>罗曼称号兑换人：<color=0xffffffff><font=龙字体,0,17,20>\\n看来你收获不少啊。");
	elseif( id == 30777)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30777,"<font=龙字体,0,19,20><color=0xfffcf157>罗曼称号兑换人：<color=0xffffffff><font=龙字体,0,17,20>\\n蕾丝边，你懂的。");

	end
end

Rluomanduihuanren = {}
Rluomanduihuanren["OnTalk"] = OnTalk
Rluomanduihuanren["OnScenarioTalk"] = OnScenarioTalk
Rluomanduihuanren["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rluomanduihuanren["OnCompleteQuestTalk"] = OnCompleteQuestTalk