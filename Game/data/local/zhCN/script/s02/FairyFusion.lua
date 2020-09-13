local function OnTalk()
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"确认熔炼");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,1,0,"离开");

		Quest.NPCTalk(0,0,"熔炼介绍： \\n 嗨~！你想拥有一个新妖精么？想让你的妖精与众不同么，那么就交给我吧，只需2个妖精就可以熔炼出一个新的妖精，你还等什么赶快行动吧，偷偷告诉你，妖精等级越高，资质越高那么熔炼出来极品妖精的几率就越高哦~！对了还有一点要注意，熔炼有几率失败，失败的话会扣除其中一只妖精，失败会扣除第一只妖精，等等不对，是第二只，不对应该扣除第一只吧….扣得太多了，我都记乱了，反正记住妖精越垃圾失败率越高或者是你点击熔炼的姿势不正确都会熔炼失败~，记住这点就行了~亲，看你运气~拜拜");
end

local function OnScenarioTalk(id,step)

	if( id == 0 )then --确认熔炼
		Quest.OpenFairyFusion();
		Quest.CloseNPCTalk();

	elseif( id == 1 )then --离开
		Quest.CloseNPCTalk();

	end

end

FairyFusion = {}
FairyFusion ["OnTalk"] = OnTalk
FairyFusion ["OnScenarioTalk"] = OnScenarioTalk
