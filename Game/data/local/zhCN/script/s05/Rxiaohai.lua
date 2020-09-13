--[[
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510629);
	Quest.SetCompleteOptions(5510629);
	talk_ID=math.random(1,5)
	if talk_ID==1 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>顽皮的小孩<color=0xffffffff>\\n        你有棒棒糖吗？");
	elseif talk_ID==2 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>顽皮的小孩<color=0xffffffff>\\n        我们常和那些大坏人躲猫猫。");
	elseif talk_ID==3 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>顽皮的小孩<color=0xffffffff>\\n        我们一起来做游戏吧。");
	elseif talk_ID==4 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>顽皮的小孩<color=0xffffffff>\\n        我长大了也要像你们一样帅。");
	else
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>顽皮的小孩<color=0xffffffff>\\n        没人陪我玩。");

	end
end






Rxiaohai = {}
Rxiaohai["OnTalk"] = OnTalk

]]--