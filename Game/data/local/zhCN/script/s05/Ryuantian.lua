
local function OnTalk()
	Quest.ClearNPCTalkOptions();--清除对话框选项
	Quest.SetAcceptOptions(5010089);
	Quest.SetCompleteOptions(5010089);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n天雪关前面就是白龙谷，如今这里十分危险。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 20305)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20305,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n白龙谷现在已经被魔族侵染了，看看那些雪狐的眼神，我们需要清理一下。");

	elseif( id == 20306)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20306,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n看看那些强盗，如果它们继续被魔族影响，对天雪镇的威胁真是太大了。");
	elseif( id == 20307)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20307,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n这附近有一种叫做<color=0xffff6000>雪狐<color=0xffffffff>的狐狸，好像对魔族的影响很小，我很想研究一下这其中的原因。能帮我弄一些银狐皮来么？");
	elseif( id == 20308)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20308,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n天神也是需要磨练的，否则也可能被魔族打败，这附近有个叫做<N>白龙洞</N>的地方，不过等级太低的天神进去是很危险的，若等级到达<C>33</C>级的时候，差不多就可以去那里了。");
	elseif( id == 20314)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20314,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n<N>白龙谷</N>已经遍布着被魔族控制的怪物，其中还有曾经下凡与此的天界兵将，天神请找到我身旁的<D>元寒</D>，他需要您的帮助。");
	elseif( id == 20350)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20350,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n听说在踏雪峰有特别强的怪物出现，不知道是否是真的，你快去看看吧。");
		end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
		if( id == 20304)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(20304,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n天神果然来帮助我们了。");
		elseif( id == 20305)then
			  Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(20305,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n真是感谢天神。");
		elseif( id == 20306)then
			Quest.CloseNPCTalk();
			  Quest.CompleteQuestTalk(20306,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n真是感谢天神。");
		elseif( id == 20307)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(20307,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n到底是为什么呢，还是到时候拿回镇子里让他们看看吧。");
		elseif( id == 20313)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(20313,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n天神终于回来了，我们在此等候多时了。");
		elseif( id == 20350)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(20350,"<font=龙字体,0,19,20><color=0xfffcf157>元天：<color=0xffffffff><font=龙字体,0,17,20>\\n哦~原来是这样，谢谢你。");

		   end

end

Ryuantian = {}
Ryuantian["OnTalk"] = OnTalk
Ryuantian["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryuantian["OnCompleteQuestTalk"] = OnCompleteQuestTalk