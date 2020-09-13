local function OnTalk()
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
	  --Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"结婚说明");					--设置对话选项
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"申请结婚");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,1,0,"申请离婚");
		--Quest.SetNPCTalkOption(EOT_ScenarioTalk,2,0,"举行婚礼(未开放)");
		--Quest.SetNPCTalkOption(EOT_ScenarioTalk,3,0,"申请强行离婚");

		Quest.NPCTalk(0,0,"阿芙罗狄：\\n        您好。\\n        想和你的爱人白头偕老吗？带一个丘比特之箭过来，我将会为你们举办婚礼。\\n        如果你需要丘比特之箭的话，可以去斗战天城找丘比特谈一谈。");
end

local function OnScenarioTalk(id,step)

	--if( id == 0 )then --结婚说明
	--	Quest.MarriageNote();

	if( id == 0 )then --申请结婚
		Quest.MarriageApply();
		Quest.CloseNPCTalk();

	elseif( id == 1 )then --申请离婚
		Quest.ForceDivorceApply();
		Quest.CloseNPCTalk();

	--elseif( id == 2 )then --举行婚礼
	--	Quest.WeddingCeremony();

	--elseif( id == 3 )then --申请强行离婚
	--	Quest.ForceDivorceApply();

	end

end

Marriage = {}
Marriage["OnTalk"] = OnTalk
Marriage["OnScenarioTalk"] = OnScenarioTalk
