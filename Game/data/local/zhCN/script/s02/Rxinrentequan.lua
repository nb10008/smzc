
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5810101);
	Quest.SetCompleteOptions(5810101);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        只有59级以下的新创建的角色才能享有新玩家特权。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
    if( id == 30725)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30725,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        并不是所有的人都能在我这里领取新人特权奖励的。你先看一下自己身上是否有“新玩家特权”的状态，如果有的话，我就把这些奖励给你。");
    elseif( id == 30726)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30726,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        和妖精缔结契约以后就可以将其收为己用了。我这里正好有一些契约用的材料，如果你能带几个“北洲之雪”回来，我就把它们都送给你。");
    elseif( id == 30727)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30727,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        如果你想要获得一整套的传说装备的话，那么你就应该多参加一些各种活动。去完成一次百鬼炼狱活动，我会给你额外的勇者之证作为奖励。");
    elseif( id == 30728)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30728,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        你可以用<C>2</C>个七彩宝玉跟我兑换蓝色品质的20级装备。七彩宝玉是通过使用天火之匣炼化各场景中怪物掉落的灵石获得的。");
    elseif( id == 30729)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30729,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        你可以用<C>2</C>个七彩宝玉跟我兑换蓝色品质的20级装备。七彩宝玉是通过使用天火之匣炼化各场景中怪物掉落的灵石获得的。");
		elseif( id == 30730)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30730,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        你可以用<C>4</C>个七彩宝玉跟我兑换蓝色品质的30级装备。七彩宝玉是通过使用天火之匣炼化各场景中怪物掉落的灵石获得的。");
		elseif( id == 30731)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30731,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        你可以用<C>4</C>个七彩宝玉跟我兑换蓝色品质的30级装备。七彩宝玉是通过使用天火之匣炼化各场景中怪物掉落的灵石获得的。");
		elseif( id == 30732)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30732,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        你可以用<C>7</C>个七彩宝玉跟我兑换蓝色品质的40级装备。七彩宝玉是通过使用天火之匣炼化各场景中怪物掉落的灵石获得的。");
		elseif( id == 30733)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30733,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        你可以用<C>7</C>个七彩宝玉跟我兑换蓝色品质的40级装备。七彩宝玉是通过使用天火之匣炼化各场景中怪物掉落的灵石获得的。");
		elseif( id == 30734)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30734,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        你可以用<C>15</C>个七彩宝玉跟我兑换蓝色品质的50级装备。七彩宝玉是通过使用天火之匣炼化各场景中怪物掉落的灵石获得的。");
		elseif( id == 30735)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30735,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        你可以用<C>15</C>个七彩宝玉跟我兑换蓝色品质的50级装备。七彩宝玉是通过使用天火之匣炼化各场景中怪物掉落的灵石获得的。");
		elseif( id == 30736)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30736,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        你可以用<C>25</C>个七彩宝玉跟我兑换蓝色品质的60级装备。七彩宝玉是通过使用天火之匣炼化各场景中怪物掉落的灵石获得的。");
    elseif( id == 30737)then
            Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(30737,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        你可以用<C>25</C>个七彩宝玉跟我兑换蓝色品质的60级装备。七彩宝玉是通过使用天火之匣炼化各场景中怪物掉落的灵石获得的。");

    end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
    if( id == 30725)then
          Quest.CloseNPCTalk();
    			Quest.CompleteQuestTalk(30725,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        这个天火之匣可是好东西，要好好保管，千万不要弄丢了。");
    elseif( id == 30726)then
          Quest.CloseNPCTalk();
    			Quest.CompleteQuestTalk(30726,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        用这些材料去寻找妖精并缔结契约吧。");
    elseif( id == 30727)then
          Quest.CloseNPCTalk();
   			  Quest.CompleteQuestTalk(30727,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        有了勇者之证你就可以获得更多的40级传说装备了。");
    elseif( id == 30728)then
          Quest.CloseNPCTalk();
    			Quest.CompleteQuestTalk(30728,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        等你有了更多的七彩宝玉记得来找我兑换哦。");
    elseif( id == 30729)then
          Quest.CloseNPCTalk();
   				Quest.CompleteQuestTalk(30729,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        等你有了更多的七彩宝玉记得来找我兑换哦。");
    elseif( id == 30730)then
          Quest.CloseNPCTalk();
   				Quest.CompleteQuestTalk(30730,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        等你有了更多的七彩宝玉记得来找我兑换哦。");
   	elseif( id == 30731)then
          Quest.CloseNPCTalk();
   				Quest.CompleteQuestTalk(30731,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        等你有了更多的七彩宝玉记得来找我兑换哦。");
   	elseif( id == 30732)then
          Quest.CloseNPCTalk();
   				Quest.CompleteQuestTalk(30732,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        等你有了更多的七彩宝玉记得来找我兑换哦。");
   elseif( id == 30733)then
          Quest.CloseNPCTalk();
   				Quest.CompleteQuestTalk(30733,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        等你有了更多的七彩宝玉记得来找我兑换哦。");
   elseif( id == 30734)then
          Quest.CloseNPCTalk();
   				Quest.CompleteQuestTalk(30734,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        等你有了更多的七彩宝玉记得来找我兑换哦。");
   	elseif( id == 30735)then
          Quest.CloseNPCTalk();
   				Quest.CompleteQuestTalk(30735,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        等你有了更多的七彩宝玉记得来找我兑换哦。");
   	elseif( id == 30736)then
          Quest.CloseNPCTalk();
   				Quest.CompleteQuestTalk(30736,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        等你有了更多的七彩宝玉记得来找我兑换哦。");
   	elseif( id == 30737)then
          Quest.CloseNPCTalk();
   				Quest.CompleteQuestTalk(30737,"<font=龙字体,0,19,20><color=0xfffcf157>新人特权使者：<color=0xffffffff><font=龙字体,0,17,20>\\n        等你有了更多的七彩宝玉记得来找我兑换哦。");
    end
end

Rxinrentequan = {}
Rxinrentequan["OnTalk"] = OnTalk
Rxinrentequan["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxinrentequan["OnCompleteQuestTalk"] = OnCompleteQuestTalk