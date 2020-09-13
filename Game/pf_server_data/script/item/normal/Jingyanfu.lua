--服务器开双倍后，玩家不能使用经验符道具

function ShuangBeiFu_canUse(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local GM_Exp_Eff = aux.GetGMExpRate()
	if(GM_Exp_Eff >= 2) then
		--服务器已经开启多倍活动，不能使用此类物品
		bRet = 71
		return bRet, bIgnore
	end
end

aux.RegisterItemEvent(3070024, 0, "ShuangBeiFu_canUse")--3070024	小经验符                  
aux.RegisterItemEvent(3070025, 0, "ShuangBeiFu_canUse")--3070025	小经验符                  
aux.RegisterItemEvent(3070026, 0, "ShuangBeiFu_canUse")--3070026	经验符                      
aux.RegisterItemEvent(3070027, 0, "ShuangBeiFu_canUse")--3070027	经验符                      
aux.RegisterItemEvent(3070028, 0, "ShuangBeiFu_canUse")--3070028	超级经验符               
aux.RegisterItemEvent(3070029, 0, "ShuangBeiFu_canUse")--3070029	超级经验符               
aux.RegisterItemEvent(3070061, 0, "ShuangBeiFu_canUse")--3070061	经验符                      
aux.RegisterItemEvent(5600001, 0, "ShuangBeiFu_canUse")--5600001	超级经验符[2小时]    
aux.RegisterItemEvent(5600002, 0, "ShuangBeiFu_canUse")--5600002	超级经验符[2小时]    
aux.RegisterItemEvent(5600003, 0, "ShuangBeiFu_canUse")--5600003	超级经验符[6小时]    
aux.RegisterItemEvent(5600004, 0, "ShuangBeiFu_canUse")--5600004	超级经验符[6小时]    
aux.RegisterItemEvent(5600005, 0, "ShuangBeiFu_canUse")--5600005	超级经验符[12小时]  
aux.RegisterItemEvent(5600006, 0, "ShuangBeiFu_canUse")--5600006	超级经验符[12小时]  
aux.RegisterItemEvent(5600007, 0, "ShuangBeiFu_canUse")--5600007	超级经验符[包天]      
aux.RegisterItemEvent(5600008, 0, "ShuangBeiFu_canUse")--5600008	超级经验符[包天]




