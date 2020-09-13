

local function OnTalk()
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"查看公会仓库");					--设置对话选项
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,1,0,"设置使用权限");

		Quest.NPCTalk(0,0,"公会仓库管理员：您好，请问您需要什么帮助。");
end

local function OnScenarioTalk(id,step)

	if( id == 0 )then --查看仓库
		Quest.GuildStorage();

	elseif( id == 1 )then --设置权限
	  Quest.GuildStorPermit();

	end

end

GuildStorage = {}
GuildStorage["OnTalk"] = OnTalk
GuildStorage["OnScenarioTalk"] = OnScenarioTalk
