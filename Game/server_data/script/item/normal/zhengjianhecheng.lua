--可否使用函数
function chuanshuozhuangbei_CanUse(MapID, InstanceID, TypeID, TargetID)
    local tbl = {}
	tbl[4090004] = {id = 4090001,num = 5}
	tbl[4090005] = {id = 4090002,num = 5}
	tbl[4090006] = {id = 4090003,num = 10}
	tbl[4090012] = {id = 4090007,num = 5}
	tbl[4090013] = {id = 4090008,num = 5}
	tbl[4090014] = {id = 4090009,num = 5}
	tbl[4090015] = {id = 4090010,num = 10}
	tbl[4090016] = {id = 4090011,num = 20}

    local bRet, bIgnore = 0, false
    local k = role.GetRoleItemNum(TargetID, TypeID)
	local FreeSize = role.GetBagFreeSize(TargetID)
	if FreeSize < 1 then
           bRet = 40
	end
       if k < tbl[TypeID].num then
	    bRet = 32
	end
	--返回
	return bRet, bIgnore

end

--使用效果
function chuanshuozhuangbei_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local tbl = {}
	tbl[4090004] = {id = 4090001,num = 5}
	tbl[4090005] = {id = 4090002,num = 5}
	tbl[4090006] = {id = 4090003,num = 10}
	tbl[4090012] = {id = 4090007,num = 5}
	tbl[4090013] = {id = 4090008,num = 5}
	tbl[4090014] = {id = 4090009,num = 5}
	tbl[4090015] = {id = 4090010,num = 10}
	tbl[4090016] = {id = 4090011,num = 20}

	role.RemoveFromRole(MapID, InstanceID, TargetID, TypeID, tbl[TypeID].num, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[TypeID].id, 1, -1, 8, 420)

	return 1
end

--注册
aux.RegisterItemEvent(4090004, 1, "chuanshuozhuangbei_QuestUsable")
aux.RegisterItemEvent(4090004, 0, "chuanshuozhuangbei_CanUse")
aux.RegisterItemEvent(4090005, 1, "chuanshuozhuangbei_QuestUsable")
aux.RegisterItemEvent(4090005, 0, "chuanshuozhuangbei_CanUse")
aux.RegisterItemEvent(4090006, 1, "chuanshuozhuangbei_QuestUsable")
aux.RegisterItemEvent(4090006, 0, "chuanshuozhuangbei_CanUse")
aux.RegisterItemEvent(4090012, 1, "chuanshuozhuangbei_QuestUsable")
aux.RegisterItemEvent(4090012, 0, "chuanshuozhuangbei_CanUse")
aux.RegisterItemEvent(4090013, 1, "chuanshuozhuangbei_QuestUsable")
aux.RegisterItemEvent(4090013, 0, "chuanshuozhuangbei_CanUse")
aux.RegisterItemEvent(4090014, 1, "chuanshuozhuangbei_QuestUsable")
aux.RegisterItemEvent(4090014, 0, "chuanshuozhuangbei_CanUse")
aux.RegisterItemEvent(4090015, 1, "chuanshuozhuangbei_QuestUsable")
aux.RegisterItemEvent(4090015, 0, "chuanshuozhuangbei_CanUse")
aux.RegisterItemEvent(4090016, 1, "chuanshuozhuangbei_QuestUsable")
aux.RegisterItemEvent(4090016, 0, "chuanshuozhuangbei_CanUse")


