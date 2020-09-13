

local function OnTalk()
		Quest.CloseNPCTalk();
		Quest.OpenShop();
end

Shop = {}
Shop["OnTalk"] = OnTalk