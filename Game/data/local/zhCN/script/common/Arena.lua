local function OnTalk()
		Quest.ClearNPCTalkOptions();                    --清除对话框选项					--设置对话选项
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,ScenarioTalkBegin,"3v3竞技场");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,1,ScenarioTalkBegin,"竞技场规则介绍");

		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xffffe289>竞技场管事：<color=0xffffffff><font=龙字体,0,17,20>\\n        吾奉帝瑞之命来此挑选勇士抵御魔患，若是你觉得自己可以胜任的话不妨进来一试！");
end

local function OnScenarioTalk(id,step)


	if( id == 0 )then --3v3竞技场
	        Quest.CloseNPCTalk();
		PvP.PvPEnterHall(1);

	elseif( id == 1 )then --竞技场规则介绍
		if( step == ScenarioTalkBegin ) then
			Quest.ClearNPCTalkOptions();                    --清除对话框选项
	                Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xffffe289>竞技场管事：<color=0xffffffff><font=龙字体,0,17,20>\\n        你想知道哪些关于竞技场的事情呢？");
                        Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin+1, "如何进入竞技场大厅" );
                        Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin+2, "如何进入竞技场房间" );
                        Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin+3, "竞技场战斗的流程" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin+4, "武勋值和武技值" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin+5, "返回" );
		elseif( step == ScenarioTalkBegin+1 ) then
                        Quest.ClearNPCTalkOptions();
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xffffe289>竞技场管事：<color=0xffffffff><font=龙字体,0,17,20>\\n        只要你选择选项<color=0xff00a2ff>3v3竞技场<color=0xffffffff>，就能进入竞技场的大厅。在大厅里可以看见竞技场的房间列表，你可以根据自己的实际情况选择一个合适自己的房间进入。另外，如果你觉得房间太多不好挑选的话，请将<color=0xff05ff00>大厅界面左下角<color=0xffffffff>的<color=0xff00a2ff>房间过滤<color=0xffffffff>功能打开，这样就能够屏蔽掉所有你无法进入的房间了。");
                        Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin,"确定" );
                elseif( step == ScenarioTalkBegin+2 ) then
                        Quest.ClearNPCTalkOptions();
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xffffe289>竞技场管事：<color=0xffffffff><font=龙字体,0,17,20>\\n        竞技场一共有两种类型：<color=0xff05ff00>随机竞技场<color=0xffffffff>和<color=0xff05ff00>普通竞技场<color=0xffffffff>。随机竞技场只有在每天的16:00-17:00才会开放，而普通竞技场则会全天开放。每个竞技场房间都有自己的<color=0xff00a2ff>进入限制<color=0xffffffff>（显示在大厅房间列表中房间名的下方），你只能选择合适自己的房间参与战斗。如果你想要自己挑选对手，那么请使用<color=0xff00a2ff>创建房间<color=0xffffffff>的功能，这样你就可以拥有自己的房间了。");
                        Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin,"确定" );
                elseif( step == ScenarioTalkBegin+3 ) then
                        Quest.ClearNPCTalkOptions();
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xffffe289>竞技场管事：<color=0xffffffff><font=龙字体,0,17,20>\\n        战斗开始以后，参战双方的所有成员都将获得<color=0xff05ff00>一分钟的准备时间<color=0xffffffff>（期间将获得回满体力真气并重置所有技能的恢复时间），在此期间，如果其中一方的队员全部离开竞技场，则剩余的一方不会获得任何奖励。战斗开始后，双方将进行公平的比试，队员全部阵亡（或者离开竞技场）的一方将被判负。如果战斗进行到第<color=0xffff0000>10<color=0xffffffff>分钟双方还没有分出胜负，则系统会自动判断<color=0xff00a2ff>存活人数多<color=0xffffffff>的一方获胜（如果存活人数相等则判断为平局）。战斗结束以后，系统将会按照每个人的武勋等级来相应增加他们的武勋及武技值（<color=0xffff0000>所有提前离开竞技场的玩家都不会获得任何分数<color=0xffffffff>）。");
                        Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin,"确定" );
		elseif( step == ScenarioTalkBegin+4 ) then
                        Quest.ClearNPCTalkOptions();
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xffffe289>竞技场管事：<color=0xffffffff><font=龙字体,0,17,20>\\n        每个人每天可以获得的武勋值是有上限的，平时的上限是<color=0xff05ff00>100<color=0xffffffff>点，而周末的上限是<color=0xff05ff00>200<color=0xffffffff>点。每个玩家能够保存的武技值也是有上限的，它是由玩家的武勋等级来决定。如果达到这个上限，则玩家必须将武技值用掉一部分才能继续获得武技值。这两个值可以在人物属性界面的<color=0xff05ff00>“排行”<color=0xffffffff>分页中查看。<color=0xff05ff00>武技值可以用来购买各种职业装备和强化材料。<color=0xffffffff>");
                        Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin,"确定" );
		elseif( step == ScenarioTalkBegin+5 ) then
                        OnTalk();
		end

	end

end

Arena = {}
Arena["OnTalk"] = OnTalk
Arena["OnScenarioTalk"] = OnScenarioTalk
