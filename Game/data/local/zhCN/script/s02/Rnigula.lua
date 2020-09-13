
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5600011);
	Quest.SetCompleteOptions(5600011);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n银装素裹的雪国圣迹，那里拥有很多传说！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30679)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30679,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n快去寻找3个槲寄生嫩叶和3个雪人之心吧。");
	elseif( id == 30680)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30680,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n雪之金币可以用于参加各种圣诞活动，祝你玩得开心。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30658)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30658,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30659)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30659,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30660)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30660,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30661)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30661,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30662)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30662,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30663)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30663,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30664)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30664,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30665)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30665,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30666)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30666,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30667)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30667,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30668)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30668,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30669)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30669,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30670)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30670,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30671)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30671,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30672)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30672,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30673)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30673,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30674)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30674,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30675)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30675,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30676)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30676,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30677)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30677,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30678)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30678,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30679)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30679,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n每天都可以完成一次哦。");
	elseif( id == 30680)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30680,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n快去参加圣诞活动吧！");
	elseif( id == 30690)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30690,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30691)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30691,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30692)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30692,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30693)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30693,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30694)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30694,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30695)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30695,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30696)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30696,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30697)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30697,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30698)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30698,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30699)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30699,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30700)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30700,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30701)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30701,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30702)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30702,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30703)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30703,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30704)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30704,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30705)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30705,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30706)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30706,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30707)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30707,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30708)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30708,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30709)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30709,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
	elseif( id == 30710)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30710,"<font=龙字体,0,19,20><color=0xfffcf157>雪国长者尼古拉：<color=0xffffffff>\\n这次保护雪国的作战十分成功，那些家伙其实也只是想得到些圣诞礼物而已，不过要用正当的方法才行。这是给你的奖励，十分感谢你的帮助。");
       end

end

Rnigula= {}
Rnigula["OnTalk"] = OnTalk
Rnigula["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rnigula["OnCompleteQuestTalk"] = OnCompleteQuestTalk