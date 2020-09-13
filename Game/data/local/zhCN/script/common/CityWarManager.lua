local function OnTalk()
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"城战报名");
		Quest.SetAcceptOptions(4900001);			
		Quest.SetCompleteOptions(4900001);
		
		Quest.NPCTalk(0,0,"城战报名官员：您好，请问您需要什么帮助。");
end

local function OnScenarioTalk(id,step)
	
	if( id == 0 )then --查看城市信息				
		City.OpenCityWarApply()	
	end
	
end

CityWarManager = {}
CityWarManager["OnTalk"] = OnTalk
CityWarManager["OnScenarioTalk"] = OnScenarioTalk

