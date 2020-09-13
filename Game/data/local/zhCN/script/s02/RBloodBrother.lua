local function OnTalk()
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"签订契约【7天】");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,1,0,"签订契约【15天】");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,2,0,"取消");
		Quest.SetAcceptOptions(5610057);
	  Quest.SetCompleteOptions(5610057);
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>灵能引渡者：<color=0xffffffff>\\n        经验分享可以让低等级的玩家在与高等级玩家组队时额外获得高等级玩家的杀怪经验的30%。（高等级玩家的经验获得不会受到任何影响）。\\n想要使用此功能的话，需要满足如下条件：\\n两个人必须互为好友，并组成二人小队;低级玩家身上必须携带有一个相应时长的“经验分享契约”;每个高于80级的玩家最多只能将杀怪经验分享给5个低级玩家;每个低于80级的玩家最多只能分享1个高级玩家的杀怪经验。\\n当两个玩家不再互为好友时此功能失效。");
end

local function OnScenarioTalk(id,step)

	if( id == 0 )then  --签订契约【7天】
		Quest.BloodBrother(7);
		Quest.CloseNPCTalk();
	elseif( id == 1 )then --签订契约【15天】
		Quest.BloodBrother(15);
		Quest.CloseNPCTalk();
	elseif( id == 2 )then --关闭
		Quest.CloseNPCTalk();
	end

end


RBloodBrother = {}
RBloodBrother["OnTalk"] = OnTalk
RBloodBrother["OnScenarioTalk"] = OnScenarioTalk