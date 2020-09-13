
function x_RoleGetItem(RoleID, ItemID)  --获得道具
    if role.IsFairyItem(ItemID) then
		role.NotifyFBBS(RoleID,3,ItemID)
	end
    if ItemID >= 3030004 and ItemID <= 3030101 then
		local baoshi_4 = {}
		baoshi_4[3030004] = true
		baoshi_4[3030012] = true
		baoshi_4[3030018] = true
		baoshi_4[3030024] = true
		baoshi_4[3030032] = true
		baoshi_4[3030040] = true
		baoshi_4[3030046] = true
		baoshi_4[3030052] = true
		baoshi_4[3030057] = true
		baoshi_4[3030062] = true
		baoshi_4[3030067] = true
		baoshi_4[3030075] = true
		baoshi_4[3030083] = true
		baoshi_4[3030088] = true
		baoshi_4[3030094] = true
		baoshi_4[3030097] = true
		baoshi_4[3030100] = true


		local baoshi_5 = {}
		baoshi_5[3030005] = true
		baoshi_5[3030013] = true
		baoshi_5[3030019] = true
		baoshi_5[3030025] = true
		baoshi_5[3030033] = true
		baoshi_5[3030041] = true
		baoshi_5[3030047] = true
		baoshi_5[3030053] = true
		baoshi_5[3030058] = true
		baoshi_5[3030063] = true
		baoshi_5[3030068] = true
		baoshi_5[3030076] = true
		baoshi_5[3030084] = true
		baoshi_5[3030089] = true
		baoshi_5[3030095] = true
		baoshi_5[3030098] = true
		baoshi_5[3030101] = true


		local baoshi_6 = {}
		baoshi_6[3030006] = true
		baoshi_6[3030014] = true
		baoshi_6[3030020] = true
		baoshi_6[3030026] = true
		baoshi_6[3030034] = true
		baoshi_6[3030042] = true
		baoshi_6[3030048] = true
		baoshi_6[3030054] = true
		baoshi_6[3030059] = true
		baoshi_6[3030064] = true
		baoshi_6[3030069] = true
		baoshi_6[3030077] = true
		baoshi_6[3030085] = true
		baoshi_6[3030090] = true

		if baoshi_4[ItemID] == true then
			if role.GetTrialState(RoleID, 5006) == 1 then
				role.SetTrialCompleteNum(RoleID, 5006, 1)
			end
			if role.GetTrialState(RoleID, 5006) == 1 and role.GetTrialCompleteNum(RoleID,5006) >= Trail_maxnum[5006] then
				role.TrialComplete(RoleID,5006)
			end
		elseif baoshi_5[ItemID] == true then
			if role.GetTrialState(RoleID, 6008) == 1 then
				role.SetTrialCompleteNum(RoleID, 6008, 1)
			end
			if role.GetTrialState(RoleID, 6008) == 1 and role.GetTrialCompleteNum(RoleID,6008) >= Trail_maxnum[6008] then
				role.TrialComplete(RoleID,6008)
			end
		elseif baoshi_6[ItemID] == true then
			if role.GetTrialState(RoleID, 7012) == 1 then
				role.SetTrialCompleteNum(RoleID, 7012, 1)
			end
			if role.GetTrialState(RoleID, 7012) == 1 and role.GetTrialCompleteNum(RoleID,7012) >= Trail_maxnum[7012] then
				role.TrialComplete(RoleID,7012)
			end
		end

	elseif ItemID >= 3040003 and ItemID <= 3040060 then
        if (ItemID - 3040003) % 5 == 0 then
		    if role.GetTrialState(RoleID, 7013) == 1 then
				role.SetTrialCompleteNum(RoleID, 7013, 1)
			end
			if role.GetTrialState(RoleID, 7013) == 1 and role.GetTrialCompleteNum(RoleID,7013) >= Trail_maxnum[7013] then
				role.TrialComplete(RoleID,7013)
			end
		elseif (ItemID - 3040004) % 5 == 0 then
            if role.GetTrialState(RoleID, 8009) == 1 then
				role.SetTrialCompleteNum(RoleID, 8009, 1)
			end
			if role.GetTrialState(RoleID, 8009) == 1 and role.GetTrialCompleteNum(RoleID,8009) >= Trail_maxnum[8009] then
				role.TrialComplete(RoleID,8009)
			end
        elseif (ItemID - 3040005) % 5 == 0 then
		    if role.GetTrialState(RoleID, 9028) == 1 then
				role.SetTrialCompleteNum(RoleID, 9028, 1)
			end
			if role.GetTrialState(RoleID, 9028) == 1 and role.GetTrialCompleteNum(RoleID,9028) >= Trail_maxnum[9028] then
				role.TrialComplete(RoleID,9028)
			end
		end
	elseif ItemID >= 3050001 and ItemID <= 3050026 then
	    if ItemID == 3050001 then
            local i = role.GetRoleItemNum(RoleID, 3050001)
			if role.GetTrialState(RoleID, 2011) == 1 and role.GetTrialCompleteNum(RoleID,2011) < i then
				role.SetTrialCompleteNum(RoleID, 2011, i)
			end
			if role.GetTrialState(RoleID, 2011) == 1 and role.GetTrialCompleteNum(RoleID,2011) >= Trail_maxnum[2011] then
				role.TrialComplete(RoleID,2011)
			end
        elseif ItemID == 3050003 then
            local i = role.GetRoleItemNum(RoleID, 3050003)
			if role.GetTrialState(RoleID, 2012) == 1 and role.GetTrialCompleteNum(RoleID,2012) < i then
				role.SetTrialCompleteNum(RoleID, 2012, i)
			end
			if role.GetTrialState(RoleID, 2012) == 1 and role.GetTrialCompleteNum(RoleID,2012) >= Trail_maxnum[2012] then
				role.TrialComplete(RoleID,2012)
			end
		elseif ItemID == 3050024 then
            local i = role.GetRoleItemNum(RoleID, 3050024)
			if role.GetTrialState(RoleID, 5012) == 1 and role.GetTrialCompleteNum(RoleID,5012) < i then
				role.SetTrialCompleteNum(RoleID, 5012, i)
			end
			if role.GetTrialState(RoleID, 5012) == 1 and role.GetTrialCompleteNum(RoleID,5012) >= Trail_maxnum[5012] then
				role.TrialComplete(RoleID,5012)
			end
		elseif ItemID == 3050025 then
            local i = role.GetRoleItemNum(RoleID, 3050025)
			if role.GetTrialState(RoleID, 6015) == 1 and role.GetTrialCompleteNum(RoleID,6015) < i then
				role.SetTrialCompleteNum(RoleID, 6015, i)
			end
			if role.GetTrialState(RoleID, 6015) == 1 and role.GetTrialCompleteNum(RoleID,6015) >= Trail_maxnum[6015] then
				role.TrialComplete(RoleID,6015)
			end
		elseif ItemID == 3050026 then
            local i = role.GetRoleItemNum(RoleID, 3050026)
			if role.GetTrialState(RoleID, 7014) == 1 and role.GetTrialCompleteNum(RoleID,7014) < i then
				role.SetTrialCompleteNum(RoleID, 7014, i)
			end
			if role.GetTrialState(RoleID, 7014) == 1 and role.GetTrialCompleteNum(RoleID,7014) >= Trail_maxnum[7014] then
				role.TrialComplete(RoleID,7014)
			end
		end
	elseif ItemID >= 8160001 and ItemID <= 8170022 then
	    if ItemID % 4 == 0 then
		    local Trail = {3013,5013,7015,6007,9014}
		    local k = (ItemID % 100) / 4
			if k ~= 4 then  --6007没有这个成就，被关闭了
				if role.GetTrialCompleteNum(RoleID,Trail[k]) < 1 and role.GetTrialState(RoleID, Trail[k]) == 1 then
					role.SetTrialCompleteNum(RoleID, Trail[k], 1)
				end
				if role.GetTrialState(RoleID, Trail[k]) == 1 and role.GetTrialCompleteNum(RoleID,Trail[k]) >= Trail_maxnum[Trail[k]] then
					role.TrialComplete(RoleID,Trail[k])
				end
			end
        elseif ItemID % 100 > 20 then
			if ItemID == 8160021 then
			    if role.GetTrialState(RoleID, 8025) == 1 and role.GetTrialCompleteNum(RoleID,8025) < 1 then
				    role.SetTrialCompleteNum(RoleID, 8025, 1)
				end
				if role.GetTrialState(RoleID, 8025) == 1 and role.GetTrialCompleteNum(RoleID,8025) >= Trail_maxnum[8025] then
					role.TrialComplete(RoleID,8025)
				end
			elseif ItemID == 8160022 then
			    if role.GetTrialState(RoleID, 8026) == 1 and role.GetTrialCompleteNum(RoleID,8026) < 1 then
				    role.SetTrialCompleteNum(RoleID, 8026, 1)
				end
				if role.GetTrialState(RoleID, 8026) == 1 and role.GetTrialCompleteNum(RoleID,8026) >= Trail_maxnum[8026] then
					role.TrialComplete(RoleID,8026)
				end
			elseif ItemID == 8170021 then
			    if role.GetTrialState(RoleID, 8027) == 1 and role.GetTrialCompleteNum(RoleID,8027) < 1 then
				    role.SetTrialCompleteNum(RoleID, 8027, 1)
				end
				if role.GetTrialState(RoleID, 8027) == 1 and role.GetTrialCompleteNum(RoleID,8027) >= Trail_maxnum[8027] then
					role.TrialComplete(RoleID,8027)
				end
			elseif ItemID == 8170022 then
			    if role.GetTrialState(RoleID, 8028) == 1 and role.GetTrialCompleteNum(RoleID,8028) < 1 then
				    role.SetTrialCompleteNum(RoleID, 8028, 1)
				end
				if role.GetTrialState(RoleID, 8028) == 1 and role.GetTrialCompleteNum(RoleID,8028) >= Trail_maxnum[8028] then
					role.TrialComplete(RoleID,8028)
				end
			end
		end
    end
	if ItemID == 8018001 or ItemID == 8018002 or ItemID == 8018003 or ItemID == 8028001 or ItemID == 8028002 or ItemID == 8028003 or ItemID == 8038001 or ItemID == 8038002 or ItemID == 8038003 or ItemID == 8048001 or ItemID == 8048002 or ItemID == 8048003 then
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["God_Equip"], 1)
		local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleID)
		if role.IsRoleHaveQuest(Target_MapID, Target_InstanceID, RoleID, 40065) then
			role.ModSpecialTargetValue(Target_MapID, Target_InstanceID, RoleID, 40065, 2)
		end
	end
end

aux.RegisterRoleEvent(54, "x_RoleGetItem")
