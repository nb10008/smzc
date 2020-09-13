local function OnTalk()
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"查看城市信息");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,1,0,"提升城市生产力");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,2,0,"设置税率");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,3,0,"查看税金");
		Quest.SetAcceptOptions(4900003);			
		Quest.SetCompleteOptions(4900003);
		
		Quest.NPCTalk(0,0,"禁城总管：您好，请问您需要什么帮助。");
end

local function OnScenarioTalk(id,step)
	
	if( id == 0 )then --查看城市信息				
		City.OpenCity()
	elseif (id == 1) then
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
		Quest.SetCityProductivityOption(0, 0, "使用城市发展铜令");
		Quest.SetCityProductivityOption(1, 0, "使用城市发展银令");
		Quest.SetCityProductivityOption(2, 0, "使用城市发展金令");
        Quest.SetCityProductivityOption(3, 0, "返回");
		Quest.NPCTalk(0,0,"禁城总管：您好，请选择使用城市发展令牌类型");
	elseif (id == 2) then
	  City.OpenSetRate()
	elseif (id == 3) then
	  City.OpenPickUpTax()	
	end
	
end

local function OnCityProductivityTalk(id,step)
	if (id == 0) then
		City.HandinToken(0)
	elseif (id == 1) then
		City.HandinToken(1)
	elseif (id == 2) then 
		City.HandinToken(2)
	elseif (id == 3) then
		OnTalk()
	end
end

CityManager = {}
CityManager["OnTalk"] = OnTalk
CityManager["OnScenarioTalk"] = OnScenarioTalk
CityManager["OnCityProductivityTalk"] = OnCityProductivityTalk

