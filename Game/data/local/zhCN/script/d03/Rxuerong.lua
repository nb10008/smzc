
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010091);
	Quest.SetCompleteOptions(5010091);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>雪荣：<color=0xffffffff>\\n        这么危险的地方，还是少来的好。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20310)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20310,"<font=龙字体,0,19,20><color=0xfffcf157>雪荣：<color=0xffffffff>\\n    这里的野兽都被魔化了，尤其是那些可怕的寒冷掠食者。");
        elseif( id == 20311)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20311,"<font=龙字体,0,19,20><color=0xfffcf157>雪荣：<color=0xffffffff>\\n    其实我来到这里是要找一件东西，一颗冰晶一样透明的狼牙。这颗牙属于一只叫做<color=0xffff6000>寒狼之王<color=0xffffffff>的大家伙，天神帮帮忙好不好呀。");
        elseif( id == 20312)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20312,"<font=龙字体,0,19,20><color=0xfffcf157>雪荣：<color=0xffffffff>\\n    现在就回去吧，雪吉在洞外已经等了很久了。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20309)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20309,"<font=龙字体,0,19,20><color=0xfffcf157>雪荣：<color=0xffffffff>\\n        我还以为我死定了，没想到有天神来救我。");
        elseif( id == 20310)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20310,"<font=龙字体,0,19,20><color=0xfffcf157>雪荣：<color=0xffffffff>\\n        天神就是厉害。");
        elseif( id == 20311)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20311,"<font=龙字体,0,19,20><color=0xfffcf157>雪荣：<color=0xffffffff>\\n        真是太感谢了，我想我也该回去了。");
       end

end	

Rxuerong = {}
Rxuerong["OnTalk"] = OnTalk
Rxuerong["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxuerong["OnCompleteQuestTalk"] = OnCompleteQuestTalk