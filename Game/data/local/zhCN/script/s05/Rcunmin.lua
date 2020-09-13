
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510628);
	Quest.SetCompleteOptions(5510628);
	talk_ID=math.random(1,5)
	if talk_ID==1 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>饥寒交迫的平民<color=0xffffffff>\\n        我们被白银魔窟和寒冰魔域的恶人们欺压的可惨了。");
	elseif talk_ID==2 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>福临镇的平民<color=0xffffffff>\\n          欢迎您的到来，我们的镇长在村北头的高台上。");
	elseif talk_ID==3 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>饥寒交迫的平民<color=0xffffffff>\\n        阿鲁可是福临镇的名人啊。");
	elseif talk_ID==4 then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>饥寒交迫的平民<color=0xffffffff>\\n        北州天雪有一对漂亮的姐妹花。");
	else
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>饥寒交迫的平民<color=0xffffffff>\\n        悬赏榜可以接取各种任务，帮助我们铲除周围的歹人可以获得丰厚的奖励呢！");

	end
end






Rcunmin = {}
Rcunmin["OnTalk"] = OnTalk
