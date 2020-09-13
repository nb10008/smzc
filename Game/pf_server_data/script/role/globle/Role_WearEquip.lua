
function x_OnRoleWearEpuip(MapID, InstanceID, RoleID, EquipID, WearorPutoff, Pos, IDHigh, IDLow)
    local Map_InstancID = {3390518568,52648950,52648182,52648438,52649718,52649974,52649206,52649462}
	for k,v in pairs(Map_InstancID) do
	    if MapID == v then
		    InstanceID = 1
			break
		end
	end
	if WearorPutoff == 1 then
	    if (EquipID == 8220401 or (EquipID >= 8220501 and EquipID <= 8220599)) then
			unit.AddBuff(MapID, InstanceID, RoleID, 5941702, RoleID)
			--[[local level = role.GetRoleSoulCrystalLvl(MapID, InstanceID, RoleID, IDHigh, IDLow)
		    if level ~= nil then
			    if level <= 3 then
				    unit.AddBuff(MapID, InstanceID, RoleID, 3920001, RoleID)
			    elseif level <= 6 then
				    unit.AddBuff(MapID, InstanceID, RoleID, 3920101, RoleID)
			    elseif level <= 9 then
				    unit.AddBuff(MapID, InstanceID, RoleID, 3920201, RoleID)
				elseif level <= 10 then
				    unit.AddBuff(MapID, InstanceID, RoleID, 3920301, RoleID)
				end
			end]]
		elseif (EquipID == 8220402 or (EquipID >= 8220601 and EquipID <= 8220699)) then
			unit.AddBuff(MapID, InstanceID, RoleID, 5941902, RoleID)
			--[[local level = role.GetRoleSoulCrystalLvl(MapID, InstanceID, RoleID, IDHigh, IDLow)
		    if level ~= nil then
			    if level <= 3 then
				    unit.AddBuff(MapID, InstanceID, RoleID, 3920401, RoleID)
			    elseif level <= 6 then
				    unit.AddBuff(MapID, InstanceID, RoleID, 3920501, RoleID)
			    elseif level <= 9 then
				    unit.AddBuff(MapID, InstanceID, RoleID, 3920601, RoleID)
				elseif level <= 10 then
				    unit.AddBuff(MapID, InstanceID, RoleID, 3920701, RoleID)
				end
			end]]
		end
		--801枪802弓803琴804法器805头806衣807腿808手809鞋810披风811戒指812项链813腰饰
		local Qlty = role.GetEquipQlty(RoleID, IDHigh, IDLow)
		if Qlty >= 5 and EquipID >= 8013007 and EquipID <= 8130809 then
			--local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
			local Equip60 = {weapon = {8013007, 4}, qiang = {8050307, 5},gong = {8050407, 5},faqi = {8051307, 5},qin = {8051407, 5},neishi = {8110307, 8121307, 8130307},waishi = {8111307, 8122307, 8130407}}
			local Equip80 = {weapon = {8013009, 4}, qiang = {8050309, 5},gong = {8050409, 5},faqi = {8051309, 5},qin = {8051409, 5},neishi = {8110309, 8121309, 8130309},waishi = {8111309, 8122309, 8130409}}
			local Equip90 = {weapon = {8013010, 4}, qiang = {8050310, 5},gong = {8050410, 5},faqi = {8051310, 5},qin = {8051410, 5},neishi = {8110310, 8121310, 8130310},waishi = {8111310, 8122310, 8130410}}
			local Equip100 = {weapon = {8013011, 4}, qiang = {8050311, 5},gong = {8050411, 5},faqi = {8051311, 5},qin = {8051411, 5},neishi = {8110311, 8121311, 8130311},waishi = {8111311, 8122311, 8130411}}
			local EquipSM = {weapon = {8014009, 4}, qiang = {8050509, 5},gong = {8050609, 5},faqi = {8051509, 5},qin = {8051609, 5},qiangshi = {8110409, 8110410, 8121409, 8130509},gongshi = {8110509, 8110510, 8121509, 8130609},fashi = {8111409, 8111410, 8122409, 8130709},qinshi = {8111509, 8111510, 8122509, 8130809}}
			--[[EEP_Head			= 0, // 头部
				EEP_Face			= 1, // 法宝
				EEP_Body			= 2, // 上身
				EEP_Legs			= 3, // 下身
				EEP_Back			= 4, // 背部
				EEP_Wrist			= 5, // 腕部
				EEP_Feet			= 6, // 足部

				EEP_Finger1			= 7, // 戒指
				EEP_Finger2			= 8, // 戒指
				EEP_Waist			= 9, // 腰饰
				EEP_Neck			= 10, // 项链

				EEP_RightHand		= 11, // 主手武器
				EEP_LeftHand		= 12, // 副手武器]]
			local levellimit = role.GetEquipLvLimit(RoleID, IDHigh, IDLow)
			if Qlty == 5 then
				if Pos >=0 and Pos <= 6 then
					if levellimit >= 60 and levellimit < 80 then
						if (EquipID - Equip60.qiang[1]) % 10000 == 0 or EquipID == 8100307 then
							if role.GetTrialCompleteNum(RoleID,6017) < 1 and role.GetTrialState(RoleID, 6017) >= 1 then
								role.SetTrialCompleteNum(RoleID, 6017, 1)
							end
							if role.GetTrialState(RoleID, 6017) == 1 and role.GetTrialCompleteNum(RoleID,6017) >= Trail_maxnum[6017] then
								role.TrialComplete(RoleID,6017)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip60.qiang[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100307 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,6019) < EquipNum and role.GetTrialState(RoleID, 6019) == 1 then
								role.SetTrialCompleteNum(RoleID, 6019, EquipNum)
							end
							if role.GetTrialState(RoleID, 6019) == 1 and role.GetTrialCompleteNum(RoleID,6019) >= Trail_maxnum[6019] then
								role.TrialComplete(RoleID,6019)
							end
						elseif (EquipID - Equip60.gong[1]) % 10000 == 0 or EquipID == 8100407 then
							if role.GetTrialCompleteNum(RoleID,6017) < 1 and role.GetTrialState(RoleID, 6017) >= 1 then
								role.SetTrialCompleteNum(RoleID, 6017, 1)
							end
							if role.GetTrialState(RoleID, 6017) == 1 and role.GetTrialCompleteNum(RoleID,6017) >= Trail_maxnum[6017] then
								role.TrialComplete(RoleID,6017)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip60.gong[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100407 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,6019) < EquipNum and role.GetTrialState(RoleID, 6019) == 1 then
								role.SetTrialCompleteNum(RoleID, 6019, EquipNum)
							end
							if role.GetTrialState(RoleID, 6019) == 1 and role.GetTrialCompleteNum(RoleID,6019) >= Trail_maxnum[6019] then
								role.TrialComplete(RoleID,6019)
							end
						elseif (EquipID - Equip60.faqi[1]) % 10000 == 0 or EquipID == 8100507 then
							if role.GetTrialCompleteNum(RoleID,6017) < 1 and role.GetTrialState(RoleID, 6017) >= 1 then
								role.SetTrialCompleteNum(RoleID, 6017, 1)
							end
							if role.GetTrialState(RoleID, 6017) == 1 and role.GetTrialCompleteNum(RoleID,6017) >= Trail_maxnum[6017] then
								role.TrialComplete(RoleID,6017)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip60.faqi[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100507 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,6019) < EquipNum and role.GetTrialState(RoleID, 6019) == 1 then
								role.SetTrialCompleteNum(RoleID, 6019, EquipNum)
							end
							if role.GetTrialState(RoleID, 6019) == 1 and role.GetTrialCompleteNum(RoleID,6019) >= Trail_maxnum[6019] then
								role.TrialComplete(RoleID,6019)
							end
						elseif (EquipID - Equip60.qin[1]) % 10000 == 0 or EquipID == 8100607 then
							if role.GetTrialCompleteNum(RoleID,6017) < 1 and role.GetTrialState(RoleID, 6017) >= 1 then
								role.SetTrialCompleteNum(RoleID, 6017, 1)
							end
							if role.GetTrialState(RoleID, 6017) == 1 and role.GetTrialCompleteNum(RoleID,6017) >= Trail_maxnum[6017] then
								role.TrialComplete(RoleID,6017)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip60.qin[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100607 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,6019) < EquipNum and role.GetTrialState(RoleID, 6019) == 1 then
								role.SetTrialCompleteNum(RoleID, 6019, EquipNum)
							end
							if role.GetTrialState(RoleID, 6019) == 1 and role.GetTrialCompleteNum(RoleID,6019) >= Trail_maxnum[6019] then
								role.TrialComplete(RoleID,6019)
							end
						end
					elseif levellimit >= 80 and levellimit < 90 then
						if (EquipID - Equip80.qiang[1]) % 10000 == 0 or EquipID == 8100309 then
							if role.GetTrialCompleteNum(RoleID,8012) < 1 and role.GetTrialState(RoleID, 8012) >= 1 then
								role.SetTrialCompleteNum(RoleID, 8012, 1)
							end
							if role.GetTrialState(RoleID, 8012) == 1 and role.GetTrialCompleteNum(RoleID,8012) >= Trail_maxnum[8012] then
								role.TrialComplete(RoleID,8012)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip80.qiang[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100309 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,8014) < EquipNum and role.GetTrialState(RoleID, 8014) == 1 then
								role.SetTrialCompleteNum(RoleID, 8014, EquipNum)
							end
							if role.GetTrialState(RoleID, 8014) == 1 and role.GetTrialCompleteNum(RoleID,8014) >= Trail_maxnum[8014] then
								role.TrialComplete(RoleID,8014)
							end
						elseif (EquipID - Equip80.gong[1]) % 10000 == 0 or EquipID == 8100409 then
							if role.GetTrialCompleteNum(RoleID,8012) < 1 and role.GetTrialState(RoleID, 8012) >= 1 then
								role.SetTrialCompleteNum(RoleID, 8012, 1)
							end
							if role.GetTrialState(RoleID, 8012) == 1 and role.GetTrialCompleteNum(RoleID,8012) >= Trail_maxnum[8012] then
								role.TrialComplete(RoleID,8012)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip80.gong[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100409 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,8014) < EquipNum and role.GetTrialState(RoleID, 8014) == 1 then
								role.SetTrialCompleteNum(RoleID, 8014, EquipNum)
							end
							if role.GetTrialState(RoleID, 8014) == 1 and role.GetTrialCompleteNum(RoleID,8014) >= Trail_maxnum[8014] then
								role.TrialComplete(RoleID,8014)
							end
						elseif (EquipID - Equip80.faqi[1]) % 10000 == 0 or EquipID == 8100509 then
							if role.GetTrialCompleteNum(RoleID,8012) < 1 and role.GetTrialState(RoleID, 8012) >= 1 then
								role.SetTrialCompleteNum(RoleID, 8012, 1)
							end
							if role.GetTrialState(RoleID, 8012) == 1 and role.GetTrialCompleteNum(RoleID,8012) >= Trail_maxnum[8012] then
								role.TrialComplete(RoleID,8012)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip80.faqi[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100509 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,8014) < EquipNum and role.GetTrialState(RoleID, 8014) == 1 then
								role.SetTrialCompleteNum(RoleID, 8014, EquipNum)
							end
							if role.GetTrialState(RoleID, 8014) == 1 and role.GetTrialCompleteNum(RoleID,8014) >= Trail_maxnum[8014] then
								role.TrialComplete(RoleID,8014)
							end
						elseif (EquipID - Equip80.qin[1]) % 10000 == 0 or EquipID == 8100609 then
							if role.GetTrialCompleteNum(RoleID,8012) < 1 and role.GetTrialState(RoleID, 8012) >= 1 then
								role.SetTrialCompleteNum(RoleID, 8012, 1)
							end
							if role.GetTrialState(RoleID, 8012) == 1 and role.GetTrialCompleteNum(RoleID,8012) >= Trail_maxnum[8012] then
								role.TrialComplete(RoleID,8012)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip80.qin[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100609 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,8014) < EquipNum and role.GetTrialState(RoleID, 8014) == 1 then
								role.SetTrialCompleteNum(RoleID, 8014, EquipNum)
							end
							if role.GetTrialState(RoleID, 8014) == 1 and role.GetTrialCompleteNum(RoleID,8014) >= Trail_maxnum[8014] then
								role.TrialComplete(RoleID,8014)
							end
						end
					elseif levellimit >= 90 and levellimit < 100 then
						if (EquipID - Equip90.qiang[1]) % 10000 == 0 or EquipID == 8100310 then
							if role.GetTrialCompleteNum(RoleID,9016) < 1 and role.GetTrialState(RoleID, 9016) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9016, 1)
							end
							if role.GetTrialState(RoleID, 9016) == 1 and role.GetTrialCompleteNum(RoleID,9016) >= Trail_maxnum[9016] then
								role.TrialComplete(RoleID,9016)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip90.qiang[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100310 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,9018) < EquipNum and role.GetTrialState(RoleID, 9018) == 1 then
								role.SetTrialCompleteNum(RoleID, 9018, EquipNum)
							end
							if role.GetTrialState(RoleID, 9018) == 1 and role.GetTrialCompleteNum(RoleID,9018) >= Trail_maxnum[9018] then
								role.TrialComplete(RoleID,9018)
							end
						elseif (EquipID - Equip90.gong[1]) % 10000 == 0 or EquipID == 8100410 then
							if role.GetTrialCompleteNum(RoleID,9016) < 1 and role.GetTrialState(RoleID, 9016) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9016, 1)
							end
							if role.GetTrialState(RoleID, 9016) == 1 and role.GetTrialCompleteNum(RoleID,9016) >= Trail_maxnum[9016] then
								role.TrialComplete(RoleID,9016)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip90.gong[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100410 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,9018) < EquipNum and role.GetTrialState(RoleID, 9018) == 1 then
								role.SetTrialCompleteNum(RoleID, 9018, EquipNum)
							end
							if role.GetTrialState(RoleID, 9018) == 1 and role.GetTrialCompleteNum(RoleID,9018) >= Trail_maxnum[9018] then
								role.TrialComplete(RoleID,9018)
							end
						elseif (EquipID - Equip90.faqi[1]) % 10000 == 0 or EquipID == 8100510 then
							if role.GetTrialCompleteNum(RoleID,9016) < 1 and role.GetTrialState(RoleID, 9016) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9016, 1)
							end
							if role.GetTrialState(RoleID, 9016) == 1 and role.GetTrialCompleteNum(RoleID,9016) >= Trail_maxnum[9016] then
								role.TrialComplete(RoleID,9016)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip90.faqi[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100510 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,9018) < EquipNum and role.GetTrialState(RoleID, 9018) == 1 then
								role.SetTrialCompleteNum(RoleID, 9018, EquipNum)
							end
							if role.GetTrialState(RoleID, 9018) == 1 and role.GetTrialCompleteNum(RoleID,9018) >= Trail_maxnum[9018] then
								role.TrialComplete(RoleID,9018)
							end
						elseif (EquipID - Equip90.qin[1]) % 10000 == 0 or EquipID == 8100610 then
							if role.GetTrialCompleteNum(RoleID,9016) < 1 and role.GetTrialState(RoleID, 9016) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9016, 1)
							end
							if role.GetTrialState(RoleID, 9016) == 1 and role.GetTrialCompleteNum(RoleID,9016) >= Trail_maxnum[9016] then
								role.TrialComplete(RoleID,9016)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip90.qin[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100610 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,9018) < EquipNum and role.GetTrialState(RoleID, 9018) == 1 then
								role.SetTrialCompleteNum(RoleID, 9018, EquipNum)
							end
							if role.GetTrialState(RoleID, 9018) == 1 and role.GetTrialCompleteNum(RoleID,9018) >= Trail_maxnum[9018] then
								role.TrialComplete(RoleID,9018)
							end
						end
					elseif levellimit == 100 then
						if (EquipID - Equip100.qiang[1]) % 10000 == 0 or EquipID == 8100311 then
							if role.GetTrialCompleteNum(RoleID,9021) < 1 and role.GetTrialState(RoleID, 9021) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9021, 1)
							end
							if role.GetTrialState(RoleID, 9021) == 1 and role.GetTrialCompleteNum(RoleID,9021) >= Trail_maxnum[9021] then
								role.TrialComplete(RoleID,9021)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip100.qiang[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100311 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,9023) < EquipNum and role.GetTrialState(RoleID, 9023) == 1 then
								role.SetTrialCompleteNum(RoleID, 9023, EquipNum)
							end
							if role.GetTrialState(RoleID, 9023) == 1 and role.GetTrialCompleteNum(RoleID,9023) >= Trail_maxnum[9023] then
								role.TrialComplete(RoleID,9023)
							end
						elseif (EquipID - Equip100.gong[1]) % 10000 == 0 or EquipID == 8100411 then
							if role.GetTrialCompleteNum(RoleID,9021) < 1 and role.GetTrialState(RoleID, 9021) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9021, 1)
							end
							if role.GetTrialState(RoleID, 9021) == 1 and role.GetTrialCompleteNum(RoleID,9021) >= Trail_maxnum[9021] then
								role.TrialComplete(RoleID,9021)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip100.gong[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100411 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,9023) < EquipNum and role.GetTrialState(RoleID, 9023) == 1 then
								role.SetTrialCompleteNum(RoleID, 9023, EquipNum)
							end
							if role.GetTrialState(RoleID, 9023) == 1 and role.GetTrialCompleteNum(RoleID,9023) >= Trail_maxnum[9023] then
								role.TrialComplete(RoleID,9023)
							end
						elseif (EquipID - Equip100.faqi[1]) % 10000 == 0 or EquipID == 8100511 then
							if role.GetTrialCompleteNum(RoleID,9021) < 1 and role.GetTrialState(RoleID, 9021) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9021, 1)
							end
							if role.GetTrialState(RoleID, 9021) == 1 and role.GetTrialCompleteNum(RoleID,9021) >= Trail_maxnum[9021] then
								role.TrialComplete(RoleID,9021)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip100.faqi[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100511 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,9023) < EquipNum and role.GetTrialState(RoleID, 9023) == 1 then
								role.SetTrialCompleteNum(RoleID, 9023, EquipNum)
							end
							if role.GetTrialState(RoleID, 9023) == 1 and role.GetTrialCompleteNum(RoleID,9023) >= Trail_maxnum[9023] then
								role.TrialComplete(RoleID,9023)
							end
						elseif (EquipID - Equip100.qin[1]) % 10000 == 0 or EquipID == 8100611 then
							if role.GetTrialCompleteNum(RoleID,9021) < 1 and role.GetTrialState(RoleID, 9021) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9021, 1)
							end
							if role.GetTrialState(RoleID, 9021) == 1 and role.GetTrialCompleteNum(RoleID,9021) >= Trail_maxnum[9021] then
								role.TrialComplete(RoleID,9021)
							end
							local EquipPos = {0,2,3,5,6}
							local EquipNum = 0
							for i = 1, 5 do
								if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+Equip100.qin[1] then
									EquipNum = EquipNum + 1
								end
							end
							if role.GetEquipIDOnPos(RoleID, 4) == 8100611 then
								EquipNum = EquipNum + 1
							end
							if role.GetTrialCompleteNum(RoleID,9023) < EquipNum and role.GetTrialState(RoleID, 9023) == 1 then
								role.SetTrialCompleteNum(RoleID, 9023, EquipNum)
							end
							if role.GetTrialState(RoleID, 9023) == 1 and role.GetTrialCompleteNum(RoleID,9023) >= Trail_maxnum[9023] then
								role.TrialComplete(RoleID,9023)
							end
						end
					end
				elseif Pos >= 7 and Pos <= 10 then
					if levellimit >= 60 and levellimit < 80 then
						if EquipID == Equip60.waishi[1] or EquipID == Equip60.waishi[2] or EquipID == Equip60.waishi[3] then
							if role.GetTrialCompleteNum(RoleID,6018) < 1 and role.GetTrialState(RoleID, 6018) >= 1 then
								role.SetTrialCompleteNum(RoleID, 6018, 1)
							end
							if role.GetTrialState(RoleID, 6018) == 1 and role.GetTrialCompleteNum(RoleID,6018) >= Trail_maxnum[6018] then
								role.TrialComplete(RoleID,6018)
							end
							local EquipPos = {7,8,10,9}
							local EquipNum = 0
							for i = 1, 4 do
								if (i == 1 or i == 2) and EquipNum == 0 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip60.waishi[1] then
										EquipNum = EquipNum + 1
									end
								elseif i >2 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip60.waishi[i-1] then
										EquipNum = EquipNum + 1
									end
								end
							end
							if role.GetTrialCompleteNum(RoleID,6020) < EquipNum and role.GetTrialState(RoleID, 6020) == 1 then
								role.SetTrialCompleteNum(RoleID, 6020, EquipNum)
							end
							if role.GetTrialState(RoleID, 6020) == 1 and role.GetTrialCompleteNum(RoleID,6020) >= Trail_maxnum[6020] then
								role.TrialComplete(RoleID,6020)
							end
						elseif EquipID == Equip60.neishi[1] or EquipID == Equip60.neishi[2] or EquipID == Equip60.neishi[3] then
							if role.GetTrialCompleteNum(RoleID,6018) < 1 and role.GetTrialState(RoleID, 6018) >= 1 then
								role.SetTrialCompleteNum(RoleID, 6018, 1)
							end
							if role.GetTrialState(RoleID, 6018) == 1 and role.GetTrialCompleteNum(RoleID,6018) >= Trail_maxnum[6018] then
								role.TrialComplete(RoleID,6018)
							end
							local EquipPos = {7,8,10,9}
							local EquipNum = 0
							for i = 1, 4 do
								if (i == 1 or i == 2) and EquipNum == 0 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip60.neishi[1] then
										EquipNum = EquipNum + 1
									end
								elseif i >2 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip60.neishi[i-1] then
										EquipNum = EquipNum + 1
									end
								end
							end
							if role.GetTrialCompleteNum(RoleID,6020) < EquipNum and role.GetTrialState(RoleID, 6020) == 1 then
								role.SetTrialCompleteNum(RoleID, 6020, EquipNum)
							end
							if role.GetTrialState(RoleID, 6020) == 1 and role.GetTrialCompleteNum(RoleID,6020) >= Trail_maxnum[6020] then
								role.TrialComplete(RoleID,6020)
							end
						end
					elseif levellimit >= 80 and levellimit < 90 then
						if EquipID == Equip80.waishi[1] or EquipID == Equip80.waishi[2] or EquipID == Equip80.waishi[3] then
							if role.GetTrialCompleteNum(RoleID,8013) < 1 and role.GetTrialState(RoleID, 8013) >= 1 then
								role.SetTrialCompleteNum(RoleID, 8013, 1)
							end
							if role.GetTrialState(RoleID, 8013) == 1 and role.GetTrialCompleteNum(RoleID,8013) >= Trail_maxnum[8013] then
								role.TrialComplete(RoleID,8013)
							end
							local EquipPos = {7,8,10,9}
							local EquipNum = 0
							for i = 1, 4 do
								if (i == 1 or i == 2) and EquipNum == 0 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip80.waishi[1] then
										EquipNum = EquipNum + 1
									end
								elseif i >2 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip80.waishi[i-1] then
										EquipNum = EquipNum + 1
									end
								end
							end
							if role.GetTrialCompleteNum(RoleID,8015) < EquipNum and role.GetTrialState(RoleID, 8015) == 1 then
								role.SetTrialCompleteNum(RoleID, 8015, EquipNum)
							end
							if role.GetTrialState(RoleID, 8015) == 1 and role.GetTrialCompleteNum(RoleID,8015) >= Trail_maxnum[8015] then
								role.TrialComplete(RoleID,8015)
							end
						elseif EquipID == Equip80.neishi[1] or EquipID == Equip80.neishi[2] or EquipID == Equip80.neishi[3] then
							if role.GetTrialCompleteNum(RoleID,8013) < 1 and role.GetTrialState(RoleID, 8013) >= 1 then
								role.SetTrialCompleteNum(RoleID, 8013, 1)
							end
							if role.GetTrialState(RoleID, 8013) == 1 and role.GetTrialCompleteNum(RoleID,8013) >= Trail_maxnum[8013] then
								role.TrialComplete(RoleID,8013)
							end
							local EquipPos = {7,8,10,9}
							local EquipNum = 0
							for i = 1, 4 do
								if (i == 1 or i == 2) and EquipNum == 0 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip80.neishi[1] then
										EquipNum = EquipNum + 1
									end
								elseif i >2 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip80.neishi[i-1] then
										EquipNum = EquipNum + 1
									end
								end
							end
							if role.GetTrialCompleteNum(RoleID,8015) < EquipNum and role.GetTrialState(RoleID, 8015) == 1 then
								role.SetTrialCompleteNum(RoleID, 8015, EquipNum)
							end
							if role.GetTrialState(RoleID, 8015) == 1 and role.GetTrialCompleteNum(RoleID,8015) >= Trail_maxnum[8015] then
								role.TrialComplete(RoleID,8015)
							end
						end
					elseif levellimit >= 90 and levellimit < 100 then
						if EquipID == Equip90.waishi[1] or EquipID == Equip90.waishi[2] or EquipID == Equip90.waishi[3] then
							if role.GetTrialCompleteNum(RoleID,9017) < 1 and role.GetTrialState(RoleID, 9017) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9017, 1)
							end
							if role.GetTrialState(RoleID, 9017) == 1 and role.GetTrialCompleteNum(RoleID,9017) >= Trail_maxnum[9017] then
								role.TrialComplete(RoleID,9017)
							end
							local EquipPos = {7,8,10,9}
							local EquipNum = 0
							for i = 1, 4 do
								if (i == 1 or i == 2) and EquipNum == 0 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip90.waishi[1] then
										EquipNum = EquipNum + 1
									end
								elseif i >2 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip90.waishi[i-1] then
										EquipNum = EquipNum + 1
									end
								end
							end
							if role.GetTrialCompleteNum(RoleID,9019) < EquipNum and role.GetTrialState(RoleID, 9019) == 1 then
								role.SetTrialCompleteNum(RoleID, 9019, EquipNum)
							end
							if role.GetTrialState(RoleID, 9019) == 1 and role.GetTrialCompleteNum(RoleID,9019) >= Trail_maxnum[9019] then
								role.TrialComplete(RoleID,9019)
							end
						elseif EquipID == Equip90.neishi[1] or EquipID == Equip90.neishi[2] or EquipID == Equip90.neishi[3] then
							if role.GetTrialCompleteNum(RoleID,9017) < 1 and role.GetTrialState(RoleID, 9017) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9017, 1)
							end
							if role.GetTrialState(RoleID, 9017) == 1 and role.GetTrialCompleteNum(RoleID,9017) >= Trail_maxnum[9017] then
								role.TrialComplete(RoleID,9017)
							end
							local EquipPos = {7,8,10,9}
							local EquipNum = 0
							for i = 1, 4 do
								if (i == 1 or i == 2) and EquipNum == 0 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip90.neishi[1] then
										EquipNum = EquipNum + 1
									end
								elseif i >2 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip90.neishi[i-1] then
										EquipNum = EquipNum + 1
									end
								end
							end
							if role.GetTrialCompleteNum(RoleID,9019) < EquipNum and role.GetTrialState(RoleID, 9019) == 1 then
								role.SetTrialCompleteNum(RoleID, 9019, EquipNum)
							end
							if role.GetTrialState(RoleID, 9019) == 1 and role.GetTrialCompleteNum(RoleID,9019) >= Trail_maxnum[9019] then
								role.TrialComplete(RoleID,9019)
							end
						end
					elseif levellimit == 100 then
						if EquipID == Equip100.waishi[1] or EquipID == Equip100.waishi[2] or EquipID == Equip100.waishi[3] then
							if role.GetTrialCompleteNum(RoleID,9022) < 1 and role.GetTrialState(RoleID, 9022) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9022, 1)
							end
							if role.GetTrialState(RoleID, 9022) == 1 and role.GetTrialCompleteNum(RoleID,9022) >= Trail_maxnum[9022] then
								role.TrialComplete(RoleID,9022)
							end
							local EquipPos = {7,8,10,9}
							local EquipNum = 0
							for i = 1, 4 do
								if (i == 1 or i == 2) and EquipNum == 0 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip100.waishi[1] then
										EquipNum = EquipNum + 1
									end
								elseif i >2 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip100.waishi[i-1] then
										EquipNum = EquipNum + 1
									end
								end
							end
							if role.GetTrialCompleteNum(RoleID,9024) < EquipNum and role.GetTrialState(RoleID, 9024) == 1 then
								role.SetTrialCompleteNum(RoleID, 9024, EquipNum)
							end
							if role.GetTrialState(RoleID, 9024) == 1 and role.GetTrialCompleteNum(RoleID,9024) >= Trail_maxnum[9024] then
								role.TrialComplete(RoleID,9024)
							end
						elseif EquipID == Equip100.neishi[1] or EquipID == Equip100.neishi[2] or EquipID == Equip100.neishi[3] then
							if role.GetTrialCompleteNum(RoleID,9022) < 1 and role.GetTrialState(RoleID, 9022) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9022, 1)
							end
							if role.GetTrialState(RoleID, 9022) == 1 and role.GetTrialCompleteNum(RoleID,9022) >= Trail_maxnum[9022] then
								role.TrialComplete(RoleID,9022)
							end
							local EquipPos = {7,8,10,9}
							local EquipNum = 0
							for i = 1, 4 do
								if (i == 1 or i == 2) and EquipNum == 0 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip100.neishi[1] then
										EquipNum = EquipNum + 1
									end
								elseif i >2 then
									if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == Equip100.neishi[i-1] then
										EquipNum = EquipNum + 1
									end
								end
							end
							if role.GetTrialCompleteNum(RoleID,9024) < EquipNum and role.GetTrialState(RoleID, 9024) == 1 then
								role.SetTrialCompleteNum(RoleID, 9024, EquipNum)
							end
							if role.GetTrialState(RoleID, 9024) == 1 and role.GetTrialCompleteNum(RoleID,9024) >= Trail_maxnum[9024] then
								role.TrialComplete(RoleID,9024)
							end
						end
					end
				elseif Pos >= 11 and Pos <= 12 then
					if levellimit >= 60 and levellimit < 80 then
						if (EquipID - Equip60.weapon[1]) % 10000 == 0 then
							if role.GetTrialCompleteNum(RoleID,6016) < 1 and role.GetTrialState(RoleID, 6016) >= 1 then
								role.SetTrialCompleteNum(RoleID, 6016, 1)
							end
							if role.GetTrialState(RoleID, 6016) == 1 and role.GetTrialCompleteNum(RoleID,6016) >= Trail_maxnum[6016] then
								role.TrialComplete(RoleID,6016)
							end
						end
					elseif levellimit >= 80 and levellimit < 90 then
						if (EquipID - Equip80.weapon[1]) % 10000 == 0 then
							if role.GetTrialCompleteNum(RoleID,8011) < 1 and role.GetTrialState(RoleID, 8011) >= 1 then
								role.SetTrialCompleteNum(RoleID, 8011, 1)
							end
							if role.GetTrialState(RoleID, 8011) == 1 and role.GetTrialCompleteNum(RoleID,8011) >= Trail_maxnum[8011] then
								role.TrialComplete(RoleID,8011)
							end
						end
					elseif levellimit >= 90 and levellimit < 100 then
						if (EquipID - Equip90.weapon[1]) % 10000 == 0 then
							if role.GetTrialCompleteNum(RoleID,9015) < 1 and role.GetTrialState(RoleID, 9015) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9015, 1)
							end
							if role.GetTrialState(RoleID, 9015) == 1 and role.GetTrialCompleteNum(RoleID,9015) >= Trail_maxnum[9015] then
								role.TrialComplete(RoleID,9015)
							end
						end
					elseif levellimit == 100 then
						if (EquipID - Equip100.weapon[1]) % 10000 == 0 then
							if role.GetTrialCompleteNum(RoleID,9020) < 1 and role.GetTrialState(RoleID, 9020) >= 1 then
								role.SetTrialCompleteNum(RoleID, 9020, 1)
							end
							if role.GetTrialState(RoleID, 9020) == 1 and role.GetTrialCompleteNum(RoleID,9020) >= Trail_maxnum[9020] then
								role.TrialComplete(RoleID,9020)
							end
						end
					end
				end
			elseif Qlty == 6 then
				if Pos >= 0 and Pos <= 6 then
					if (EquipID - EquipSM.qiang[1]) % 10000 == 0 or EquipID == 8100709 then
						if role.GetTrialCompleteNum(RoleID,8017) < 1 and role.GetTrialState(RoleID, 8017) >= 1 then
							role.SetTrialCompleteNum(RoleID, 8017, 1)
						end
						if role.GetTrialState(RoleID, 8017) == 1 and role.GetTrialCompleteNum(RoleID,8017) >= Trail_maxnum[8017] then
							role.TrialComplete(RoleID,8017)
						end
						local EquipPos = {0,2,3,5,6}
						local EquipNum = 0
						for i = 1, 5 do
							if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+EquipSM.qiang[1] then
								EquipNum = EquipNum + 1
							end
						end
						if role.GetEquipIDOnPos(RoleID, 4) == 8100709 then
							EquipNum = EquipNum + 1
						end
						if role.GetTrialCompleteNum(RoleID,9025) < EquipNum and role.GetTrialState(RoleID, 9025) == 1 then
							role.SetTrialCompleteNum(RoleID, 9025, EquipNum)
						end
						if role.GetTrialState(RoleID, 9025) == 1 and role.GetTrialCompleteNum(RoleID,9025) >= Trail_maxnum[9025] then
							role.TrialComplete(RoleID,9025)
						end
					elseif (EquipID - EquipSM.gong[1]) % 10000 == 0 or EquipID == 8100809 then
						if role.GetTrialCompleteNum(RoleID,8017) < 1 and role.GetTrialState(RoleID, 8017) >= 1 then
							role.SetTrialCompleteNum(RoleID, 8017, 1)
						end
						if role.GetTrialState(RoleID, 8017) == 1 and role.GetTrialCompleteNum(RoleID,8017) >= Trail_maxnum[8017] then
							role.TrialComplete(RoleID,8017)
						end
						local EquipPos = {0,2,3,5,6}
						local EquipNum = 0
						for i = 1, 5 do
							if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+EquipSM.qiang[1] then
								EquipNum = EquipNum + 1
							end
						end
						if role.GetEquipIDOnPos(RoleID, 4) == 8100809 then
							EquipNum = EquipNum + 1
						end
						if role.GetTrialCompleteNum(RoleID,9025) < EquipNum and role.GetTrialState(RoleID, 9025) == 1 then
							role.SetTrialCompleteNum(RoleID, 9025, EquipNum)
						end
						if role.GetTrialState(RoleID, 9025) == 1 and role.GetTrialCompleteNum(RoleID,9025) >= Trail_maxnum[9025] then
							role.TrialComplete(RoleID,9025)
						end
					elseif (EquipID - EquipSM.faqi[1]) % 10000 == 0 or EquipID == 8100909 then
						if role.GetTrialCompleteNum(RoleID,8017) < 1 and role.GetTrialState(RoleID, 8017) >= 1 then
							role.SetTrialCompleteNum(RoleID, 8017, 1)
						end
						if role.GetTrialState(RoleID, 8017) == 1 and role.GetTrialCompleteNum(RoleID,8017) >= Trail_maxnum[8017] then
							role.TrialComplete(RoleID,8017)
						end
						local EquipPos = {0,2,3,5,6}
						local EquipNum = 0
						for i = 1, 5 do
							if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+EquipSM.qiang[1] then
								EquipNum = EquipNum + 1
							end
						end
						if role.GetEquipIDOnPos(RoleID, 4) == 8100909 then
							EquipNum = EquipNum + 1
						end
						if role.GetTrialCompleteNum(RoleID,9025) < EquipNum and role.GetTrialState(RoleID, 9025) == 1 then
							role.SetTrialCompleteNum(RoleID, 9025, EquipNum)
						end
						if role.GetTrialState(RoleID, 9025) == 1 and role.GetTrialCompleteNum(RoleID,9025) >= Trail_maxnum[9025] then
							role.TrialComplete(RoleID,9025)
						end
					elseif (EquipID - EquipSM.qin[1]) % 10000 == 0 or EquipID == 8101009 then
						if role.GetTrialCompleteNum(RoleID,8017) < 1 and role.GetTrialState(RoleID, 8017) >= 1 then
							role.SetTrialCompleteNum(RoleID, 8017, 1)
						end
						if role.GetTrialState(RoleID, 8017) == 1 and role.GetTrialCompleteNum(RoleID,8017) >= Trail_maxnum[8017] then
							role.TrialComplete(RoleID,8017)
						end
						local EquipPos = {0,2,3,5,6}
						local EquipNum = 0
						for i = 1, 5 do
							if role.GetEquipIDOnPos(RoleID, EquipPos[i]) == (i-1)*10000+EquipSM.qiang[1] then
								EquipNum = EquipNum + 1
							end
						end
						if role.GetEquipIDOnPos(RoleID, 4) == 8101009 then
							EquipNum = EquipNum + 1
						end
						if role.GetTrialCompleteNum(RoleID,9025) < EquipNum and role.GetTrialState(RoleID, 9025) == 1 then
							role.SetTrialCompleteNum(RoleID, 9025, EquipNum)
						end
						if role.GetTrialState(RoleID, 9025) == 1 and role.GetTrialCompleteNum(RoleID,9025) >= Trail_maxnum[9025] then
							role.TrialComplete(RoleID,9025)
						end
					end
				elseif Pos >= 7 and Pos <= 10 then
					if EquipID == EquipSM.qiangshi[1] or EquipID == EquipSM.qiangshi[2] or EquipID == EquipSM.qiangshi[3] or EquipID == EquipSM.qiangshi[4] then
						if role.GetTrialCompleteNum(RoleID,8018) < 1 and role.GetTrialState(RoleID, 8018) >= 1 then
							role.SetTrialCompleteNum(RoleID, 8018, 1)
						end
						if role.GetTrialState(RoleID, 8018) == 1 and role.GetTrialCompleteNum(RoleID,8018) >= Trail_maxnum[8018] then
							role.TrialComplete(RoleID,8018)
						end
						local EquipPos = {7,8,10,9}
						local EquipNum = 0
						if role.GetEquipIDOnPos(RoleID, EquipPos[1]) == EquipSM.qiangshi[1] or role.GetEquipIDOnPos(RoleID, EquipPos[2]) == EquipSM.qiangshi[1] then
							EquipNum = EquipNum + 1
						end
						if (role.GetEquipIDOnPos(RoleID, EquipPos[1]) == EquipSM.qiangshi[2] or role.GetEquipIDOnPos(RoleID, EquipPos[2]) == EquipSM.qiangshi[2]) and role.GetEquipIDOnPos(RoleID, EquipPos[1]) ~= role.GetEquipIDOnPos(RoleID, EquipPos[2]) then
							EquipNum = EquipNum + 1
						end
						if role.GetEquipIDOnPos(RoleID, EquipPos[3]) == EquipSM.qiangshi[3] then
							EquipNum = EquipNum + 1
						end
						if role.GetEquipIDOnPos(RoleID, EquipPos[4]) == EquipSM.qiangshi[4] then
							EquipNum = EquipNum + 1
						end
						if role.GetTrialCompleteNum(RoleID,9026) < EquipNum and role.GetTrialState(RoleID, 9026) == 1 then
							role.SetTrialCompleteNum(RoleID, 9026, EquipNum)
						end
						if role.GetTrialState(RoleID, 9026) == 1 and role.GetTrialCompleteNum(RoleID,9026) >= Trail_maxnum[9026] then
							role.TrialComplete(RoleID, 9026)
						end
					elseif EquipID == EquipSM.gongshi[1] or EquipID == EquipSM.gongshi[2] or EquipID == EquipSM.gongshi[3] or EquipID == EquipSM.gongshi[4] then
						if role.GetTrialCompleteNum(RoleID,8018) < 1 and role.GetTrialState(RoleID, 8018) >= 1 then
							role.SetTrialCompleteNum(RoleID, 8018, 1)
						end
						if role.GetTrialState(RoleID, 8018) == 1 and role.GetTrialCompleteNum(RoleID,8018) >= Trail_maxnum[8018] then
							role.TrialComplete(RoleID,8018)
						end
						local EquipPos = {7,8,10,9}
						local EquipNum = 0
						if role.GetEquipIDOnPos(RoleID, EquipPos[1]) == EquipSM.gongshi[1] or role.GetEquipIDOnPos(RoleID, EquipPos[2]) == EquipSM.gongshi[1] then
							EquipNum = EquipNum + 1
						end
						if (role.GetEquipIDOnPos(RoleID, EquipPos[1]) == EquipSM.gongshi[2] or role.GetEquipIDOnPos(RoleID, EquipPos[2]) == EquipSM.gongshi[2]) and role.GetEquipIDOnPos(RoleID, EquipPos[1]) ~= role.GetEquipIDOnPos(RoleID, EquipPos[2]) then
							EquipNum = EquipNum + 1
						end
						if role.GetEquipIDOnPos(RoleID, EquipPos[3]) == EquipSM.gongshi[3] then
							EquipNum = EquipNum + 1
						end
						if role.GetEquipIDOnPos(RoleID, EquipPos[4]) == EquipSM.gongshi[4] then
							EquipNum = EquipNum + 1
						end
						if role.GetTrialCompleteNum(RoleID,9026) < EquipNum and role.GetTrialState(RoleID, 9026) == 1 then
							role.SetTrialCompleteNum(RoleID, 9026, EquipNum)
						end
						if role.GetTrialState(RoleID, 9026) == 1 and role.GetTrialCompleteNum(RoleID,9026) >= Trail_maxnum[9026] then
							role.TrialComplete(RoleID, 9026)
						end
					elseif EquipID == EquipSM.fashi[1] or EquipID == EquipSM.fashi[2] or EquipID == EquipSM.fashi[3] or EquipID == EquipSM.fashi[4] then
						if role.GetTrialCompleteNum(RoleID,8018) < 1 and role.GetTrialState(RoleID, 8018) >= 1 then
							role.SetTrialCompleteNum(RoleID, 8018, 1)
						end
						if role.GetTrialState(RoleID, 8018) == 1 and role.GetTrialCompleteNum(RoleID,8018) >= Trail_maxnum[8018] then
							role.TrialComplete(RoleID,8018)
						end
						local EquipPos = {7,8,10,9}
						local EquipNum = 0
						if role.GetEquipIDOnPos(RoleID, EquipPos[1]) == EquipSM.fashi[1] or role.GetEquipIDOnPos(RoleID, EquipPos[2]) == EquipSM.fashi[1] then
							EquipNum = EquipNum + 1
						end
						if (role.GetEquipIDOnPos(RoleID, EquipPos[1]) == EquipSM.fashi[2] or role.GetEquipIDOnPos(RoleID, EquipPos[2]) == EquipSM.fashi[2]) and role.GetEquipIDOnPos(RoleID, EquipPos[1]) ~= role.GetEquipIDOnPos(RoleID, EquipPos[2]) then
							EquipNum = EquipNum + 1
						end
						if role.GetEquipIDOnPos(RoleID, EquipPos[3]) == EquipSM.fashi[3] then
							EquipNum = EquipNum + 1
						end
						if role.GetEquipIDOnPos(RoleID, EquipPos[4]) == EquipSM.fashi[4] then
							EquipNum = EquipNum + 1
						end
						if role.GetTrialCompleteNum(RoleID,9026) < EquipNum and role.GetTrialState(RoleID, 9026) == 1 then
							role.SetTrialCompleteNum(RoleID, 9026, EquipNum)
						end
						if role.GetTrialState(RoleID, 9026) == 1 and role.GetTrialCompleteNum(RoleID,9026) >= Trail_maxnum[9026] then
							role.TrialComplete(RoleID, 9026)
						end
					elseif EquipID == EquipSM.qinshi[1] or EquipID == EquipSM.qinshi[2] or EquipID == EquipSM.qinshi[3] or EquipID == EquipSM.qinshi[4] then
						if role.GetTrialCompleteNum(RoleID,8018) < 1 and role.GetTrialState(RoleID, 8018) >= 1 then
							role.SetTrialCompleteNum(RoleID, 8018, 1)
						end
						if role.GetTrialState(RoleID, 8018) == 1 and role.GetTrialCompleteNum(RoleID,8018) >= Trail_maxnum[8018] then
							role.TrialComplete(RoleID,8018)
						end
						local EquipPos = {7,8,10,9}
						local EquipNum = 0
						if role.GetEquipIDOnPos(RoleID, EquipPos[1]) == EquipSM.qinshi[1] or role.GetEquipIDOnPos(RoleID, EquipPos[2]) == EquipSM.qinshi[1] then
							EquipNum = EquipNum + 1
						end
						if (role.GetEquipIDOnPos(RoleID, EquipPos[1]) == EquipSM.qinshi[2] or role.GetEquipIDOnPos(RoleID, EquipPos[2]) == EquipSM.qinshi[2]) and role.GetEquipIDOnPos(RoleID, EquipPos[1]) ~= role.GetEquipIDOnPos(RoleID, EquipPos[2]) then
							EquipNum = EquipNum + 1
						end
						if role.GetEquipIDOnPos(RoleID, EquipPos[3]) == EquipSM.qinshi[3] then
							EquipNum = EquipNum + 1
						end
						if role.GetEquipIDOnPos(RoleID, EquipPos[4]) == EquipSM.qinshi[4] then
							EquipNum = EquipNum + 1
						end
						if role.GetTrialCompleteNum(RoleID,9026) < EquipNum and role.GetTrialState(RoleID, 9026) == 1 then
							role.SetTrialCompleteNum(RoleID, 9026, EquipNum)
						end
						if role.GetTrialState(RoleID, 9026) == 1 and role.GetTrialCompleteNum(RoleID,9026) >= Trail_maxnum[9026] then
							role.TrialComplete(RoleID, 9026)
						end
					end
				elseif Pos >= 11 and Pos <= 12 then
					if (EquipID - EquipSM.weapon[1]) % 10000 == 0 then
						if role.GetTrialCompleteNum(RoleID,8016) < 1 and role.GetTrialState(RoleID, 8016) >= 1 then
							role.SetTrialCompleteNum(RoleID, 8016, 1)
						end
						if role.GetTrialState(RoleID, 8016) == 1 and role.GetTrialCompleteNum(RoleID,8016) >= Trail_maxnum[8016] then
							role.TrialComplete(RoleID,8016)
						end
					end
				end
			end
		end
	else
	    if (EquipID == 8220401 or (EquipID >= 8220501 and EquipID <= 8220599)) then
			local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 59417)
			if bhave == true then
				unit.CancelBuff(MapID, InstanceID, RoleID, 5941702)
			end
			--[[local level = role.GetRoleSoulCrystalLvl(MapID, InstanceID, RoleID, IDHigh, IDLow)
		    if level ~= nil then
			    if level <= 3 then
				    unit.CancelBuff(MapID, InstanceID, RoleID, 3920001)
			    elseif level <= 6 then
				    unit.CancelBuff(MapID, InstanceID, RoleID, 3920101)
			    elseif level <= 9 then
				    unit.CancelBuff(MapID, InstanceID, RoleID, 3920201)
				elseif level <= 10 then
				    unit.CancelBuff(MapID, InstanceID, RoleID, 3920301)
				end
			end]]
		elseif (EquipID == 8220402 or (EquipID >= 8220601 and EquipID <= 8220699)) then
			local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 59419)
			if bhave == true then
				unit.CancelBuff(MapID, InstanceID, RoleID, 5941902)
			end
			--[[local level = role.GetRoleSoulCrystalLvl(MapID, InstanceID, RoleID, IDHigh, IDLow)
		    if level ~= nil then
			    if level <= 3 then
				    unit.CancelBuff(MapID, InstanceID, RoleID, 3920401)
			    elseif level <= 6 then
				    unit.CancelBuff(MapID, InstanceID, RoleID, 3920501)
			    elseif level <= 9 then
				    unit.CancelBuff(MapID, InstanceID, RoleID, 3920601)
				elseif level <= 10 then
				    unit.CancelBuff(MapID, InstanceID, RoleID, 3920701)
				end
			end]]
		end
	end
end
--注册函数

aux.RegisterRoleEvent(52, "x_OnRoleWearEpuip")

