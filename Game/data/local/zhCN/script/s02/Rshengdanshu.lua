
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600019);
	Quest.SetCompleteOptions(5600019);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞树：<color=0xffffffff>\\n圣诞快乐~~礼物多多~~");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30711)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30711,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞树：<color=0xffffffff>\\n你可以从圣诞树领取一份圣诞时装礼包。");
	elseif( id == 30712)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30712,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞树：<color=0xffffffff>\\n你可以从圣诞树领取一份礼物。");
	elseif( id == 30713)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30713,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞树：<color=0xffffffff>\\n你可以从圣诞树领取一份圣诞的祝福。");
		elseif( id == 30903)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30903,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞树：<color=0xffffffff>\\n快来领取你的圣诞装备吧，每个人只能领取一次哦。");
		elseif( id == 30904)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30904,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞树：<color=0xffffffff>\\n给我50金，我就把5个圣诞强化券给你。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30711)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30711,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞树：<color=0xffffffff>\\n请收下这套时装，赶快换上吧！");
	elseif( id == 30712)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30712,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞树：<color=0xffffffff>\\n请收下这份圣诞礼物。圣诞快乐！");
	elseif( id == 30713)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30713,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞树：<color=0xffffffff>\\n在这份祝福下，你会拥有一个快乐的圣诞的。");
	      elseif( id == 30903)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30903,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞树：<color=0xffffffff>\\n千万不要把圣诞装备弄丢了，否则会很麻烦的。");
	      elseif( id == 30904)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30904,"<font=龙字体,0,19,20><color=0xfffcf157>圣诞树：<color=0xffffffff>\\n如果你还想要更多的圣诞强化券，那么你就只能明天再来找我了。");
	end

end

Rshengdanshu = {}
Rshengdanshu["OnTalk"] = OnTalk
Rshengdanshu["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rshengdanshu["OnCompleteQuestTalk"] = OnCompleteQuestTalk
