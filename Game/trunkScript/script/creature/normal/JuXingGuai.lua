

--击杀怪物几率刷出巨型怪物


---------------------------------------------------野外场景-------------------------------------------------

-- MapID == 3424072744 --s01
-- MapID == 3424073512 --s02
-- MapID == 3424073256 --s03
-- MapID == 3424071976 --s04
-- MapID == 3424071720 --s05
-- MapID == 3424072488 --s06
-- MapID == 3424072232 --s07
-- MapID == 3424075048 --s08

function YW_JXG_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if MapID == 3424073256 then  ---------------------s03------------------------

	    if math.random(200) == 50 and role.GetBagFreeSize(RoleID) > 0 and role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69370) then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4400503, 1, -1, 8, 420)
		end

    elseif MapID == 3424071976 then  ---------------------s04------------------------

        if math.random(200) == 50 and role.GetBagFreeSize(RoleID) > 0 and role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69370) then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4400503, 1, -1, 8, 420)
		end

		if TargetTypeID == 1030002 then
				local temp = math.random(1,200)
				if temp==50 then
					--沉睡的##被您惊醒了!
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 302001)
					msg.AddMsgEvent(MsgID, 5, 1080001)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 302001)
					msg.AddMsgEvent(MsgID, 5, 1080001)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					--1/200几率出现
					local ID = map.MapCreateCreature(MapID, InstanceID, 1080001, x, y, z)
					unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030003 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080002)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080002)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080002, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030004 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
					msg.AddMsgEvent(MsgID, 5, 1080003)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 26, 302001)
					msg.AddMsgEvent(MsgID, 5, 1080003)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
					--1/200几率出现
					local ID = map.MapCreateCreature(MapID, InstanceID, 1080003, x, y, z)
					unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030005 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080004)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080004)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080004, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030006 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080005)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080005)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080005, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030007 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080006)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080006)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						 local ID = map.MapCreateCreature(MapID, InstanceID, 1080006, x, y, z)
						 unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030008 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080007)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080007)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080007, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030009 or TargetTypeID == 1030010 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080008)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080008)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080008, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030011 or TargetTypeID == 1030012 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080009)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080009)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080009, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030013 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080010)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080010)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080010, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030014 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080011)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080011)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080011, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030015 then
				local temp = math.random(1,200)
				if temp==1 then
						--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080012)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080012)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080012, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030016 then
				local temp = math.random(1,200)
				if temp==1 then
							--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080013)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080013)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080013, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1030017 then
				local temp = math.random(1,200)
				if temp==1 then
							--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080014)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080014)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080014, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		end

	elseif MapID == 3424071720  then  -------------------- s05 -----------------------

		if math.random(200) == 50 and role.GetBagFreeSize(RoleID) > 0 and role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69370) then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4400503, 1, -1, 8, 420)
		end

		if TargetTypeID == 1040003 then
				local temp = math.random(1,200)
				if temp==1 then
						--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080015)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080015)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080015, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040004 or TargetTypeID == 1040005 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080016)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080016)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080016, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040006 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080017)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080017)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080017, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040007 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080018)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080018)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080018, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040008 or TargetTypeID == 1040009 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080019)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080019)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080019, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040010 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080020)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080020)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080020, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040011 or TargetTypeID == 1040012 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080021)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080021)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080021, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040022 or TargetTypeID == 1040023 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080022)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080022)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080022, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040021 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080023)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080023)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080023, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040024 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080024)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080024)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080024, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040025 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080025)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080025)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080025, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040013 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080026)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080026)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080026, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040015 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080027)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080027)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080027, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040016 or TargetTypeID == 1040017 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080028)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080028)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080028, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040018 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080029)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080029)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080029, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040020 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080030)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080030)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080030, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040026 or TargetTypeID == 1040027 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080031)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080031)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080031, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040028 or TargetTypeID == 1040029 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080032)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080032)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080032, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040030 or TargetTypeID == 1040031 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080033)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080033)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080033, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040032 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080034)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080034)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080034, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040033 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080035)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080035)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080035, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040034 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080036)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080036)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080036, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040035 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080037)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080037)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080037, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040039 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080038)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080038)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080038, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040040 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080039)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080039)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080039, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040036 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080040)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080040)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080040, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040037 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080041)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080041)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080041, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040038 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080042)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080042)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080042, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1040041 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080043)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080043)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080043, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		end

	elseif MapID == 3424072488  then --------------------- s06--------------------------

		if math.random(200) == 50 and role.GetBagFreeSize(RoleID) > 0 and role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 69370) then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4400503, 1, -1, 8, 420)
		end

		if TargetTypeID == 1050001 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080044)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080044)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080044, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050002 or TargetTypeID == 1050003 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080045)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080045)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080045, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050004 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080046)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080046)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080046, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050005 or TargetTypeID == 1050006 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080047)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080047)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080047, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050007 or TargetTypeID == 1050008 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080048)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080048)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080048, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050009 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080049)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080049)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080049, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050010 or TargetTypeID == 1050011 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080050)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080050)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080050, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050012 or TargetTypeID == 1050013 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080051)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080051)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080051, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050014 or TargetTypeID == 1050015 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080052)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080052)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080052, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050016 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080053)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080053)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080053, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050017 or TargetTypeID == 1050018 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080054)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080054)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080054, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050019 or TargetTypeID == 1050020 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080055)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080055)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080055, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050021 or TargetTypeID == 1050022 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080056)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080056)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080056, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050044 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080058)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080058)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080058, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050025 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080059)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080059)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080059, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050026 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080060)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080060)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080060, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050027 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080061)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080061)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080061, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050028 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080062)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080062)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080062, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050029 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080063)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080063)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080063, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050030 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080064)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080064)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080064, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050031 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080065)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080065)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080065, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050032 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080066)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080066)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080066, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050033 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080067)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080067)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080067, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050034 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080068)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080068)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080068, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050035 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080069)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080069)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080069, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1050036 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080070)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080070)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080070, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		end

	elseif MapID == 3424072232   then ----------------s07---------------

		if TargetTypeID == 1060003 or TargetTypeID == 1060004 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080071)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080071)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080071, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060005 or TargetTypeID == 1060006 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080072)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080072)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080072, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060007 or TargetTypeID == 1060008 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080073)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080073)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080073, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060009 or TargetTypeID == 1060010 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080074)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080074)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080074, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060011 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080075)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080075)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080075, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060012 or TargetTypeID == 1060013 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080076)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080076)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080076, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060014 or TargetTypeID == 1060015 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080077)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080077)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080077, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060016 or TargetTypeID == 1060017 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080078)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080078)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080078, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060018 or TargetTypeID == 1060019 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080079)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080079)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080079, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060020 or TargetTypeID == 1060021 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080080)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080080)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080080, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060022 or TargetTypeID == 1060023 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080081)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080081)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080081, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060024 or TargetTypeID == 1060025 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080082)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080082)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080082, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060026 or TargetTypeID == 1060027 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080083)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080083)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080083, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060028 or TargetTypeID == 1060029 or TargetTypeID == 1060030 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080084)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080084)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080084, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060031 or TargetTypeID == 1060032 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080085)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080085)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080085, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060033 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080086)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080086)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080086, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060034 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080087)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080087)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080087, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060035 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080088)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080088)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080088, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060036 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080089)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080089)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080089, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060037 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080090)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080090)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080090, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060038 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080091)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080091)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080091, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060039 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080092)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080092)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080092, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060040 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080093)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080093)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080093, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060041 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080094)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080094)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080094, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060042 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080095)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080095)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080095, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060043 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080096)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080096)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080096, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060044 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080097)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080097)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080097, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060045 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080098)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080098)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080098, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1060046 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080099)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080099)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080099, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		end

	elseif MapID == 3424075048  then ----------------------- s08---------------------

		if TargetTypeID == 1070002 or TargetTypeID == 1070003 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080100)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080100)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080100, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070004 or TargetTypeID == 1070005 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080101)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080101)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080101, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070006 or TargetTypeID == 1070007 or TargetTypeID == 1070008 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080102)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080102)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080102, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070009 or TargetTypeID == 1070010 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080103)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080103)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080103, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070011 or TargetTypeID == 1070012 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080104)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080104)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080104, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070013 or TargetTypeID == 1070014 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080105)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080105)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080105, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070015 or TargetTypeID == 1070016 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080106)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080106)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080106, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070017 or TargetTypeID == 1070001 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080107)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080107)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080107, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070018 or TargetTypeID == 1070019 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080108)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080108)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080108, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070020 or TargetTypeID == 1070021 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080109)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080109)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080109, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070022 or TargetTypeID == 1070023 or TargetTypeID == 1070024 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080110)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080110)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080110, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070025 or TargetTypeID == 1070026 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080111)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080111)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080111, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070027 or TargetTypeID == 1070028 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080112)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080112)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080112, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070029 or TargetTypeID == 1070030 or TargetTypeID == 1070031 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080113)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080113)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080113, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070032 or TargetTypeID == 1070033 or TargetTypeID == 1070034 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080114)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080114)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080114, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070035 or TargetTypeID == 1070036 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080115)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080115)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080115, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070037 or TargetTypeID == 1070038 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080116)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080116)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080116, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070039 or TargetTypeID == 1070040 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080117)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080117)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080117, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070041 or TargetTypeID == 1070042 or TargetTypeID == 1070043 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080118)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080118)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080118, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070044 or TargetTypeID == 1070045 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080119)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080119)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080119, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070046 or TargetTypeID == 1070047  or TargetTypeID == 1070048 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080120)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080120)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080120, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070049 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080121)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080121)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080121, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070050 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080122)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080122)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080122, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070051 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080123)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080123)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080123, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070052 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080124)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080124)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080124, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070053 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080125)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080125)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080125, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070054 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080126)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080126)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080126, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070055 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080127)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080127)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080127, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070056 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080128)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080128)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080128, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070057 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080129)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080129)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080129, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070058 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080130)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080130)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080130, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070059 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080131)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080131)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080131, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070060 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080132)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080132)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080132, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070061 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080133)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080133)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080133, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070062 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080134)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080134)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080134, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070063 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080135)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080135)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080135, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070064 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080136)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080136)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080136, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070065 or TargetTypeID == 1070066 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080137)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080137)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080137, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070067 or TargetTypeID == 1070068 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080138)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080138)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080138, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070069 or TargetTypeID == 1070070 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080139)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080139)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080139, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		elseif TargetTypeID == 1070071 or TargetTypeID == 1070072 then
				local temp = math.random(1,200)
				if temp==1 then
					--沉睡的##被您惊醒了!
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080140)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 302001)
						msg.AddMsgEvent(MsgID, 5, 1080140)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
						--1/200几率出现
						local ID = map.MapCreateCreature(MapID, InstanceID, 1080140, x, y, z)
						unit.AddBuff(MapID, InstanceID, ID, 6100201, ID)
				end
		end

	end
end
aux.RegisterCreatureEvent(1020001,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020002,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020003,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020004,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020005,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020006,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020007,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020008,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020009,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020010,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020011,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020012,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020013,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1020014,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030002,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030003,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030004,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030005,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030006,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030007,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030008,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030009,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030010,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030011,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030012,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030013,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030014,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030015,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030016,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1030017,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040003,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040004,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040005,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040006,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040007,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040008,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040009,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040010,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040011,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040012,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040022,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040023,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040021,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040024,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040025,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040013,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040015,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040016,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040017,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040018,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040020,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040026,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040027,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040028,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040029,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040030,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040031,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040032,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040033,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040034,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040035,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040039,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040040,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040036,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040037,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040038,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1040041,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050001,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050002,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050003,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050004,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050005,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050006,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050007,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050008,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050009,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050010,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050011,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050012,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050013,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050014,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050015,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050016,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050017,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050018,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050019,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050020,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050021,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050022,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050023,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050024,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050044,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050025,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050026,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050027,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050028,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050029,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050030,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050031,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050032,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050033,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050034,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050035,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1050036,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060003,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060004,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060005,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060006,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060007,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060008,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060009,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060010,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060011,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060012,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060013,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060014,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060015,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060016,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060017,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060018,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060019,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060020,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060021,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060022,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060023,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060024,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060025,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060026,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060027,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060028,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060029,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060030,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060031,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060032,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060033,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060034,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060035,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060036,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060037,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060038,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060039,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060040,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060041,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060042,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060043,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060044,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060045,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1060046,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070002,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070003,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070004,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070005,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070006,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070007,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070008,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070009,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070010,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070011,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070012,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070013,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070014,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070015,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070016,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070017,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070001,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070018,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070019,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070020,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070021,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070022,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070023,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070024,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070025,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070026,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070027,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070028,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070029,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070030,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070031,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070032,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070033,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070034,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070035,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070036,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070037,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070038,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070039,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070040,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070041,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070042,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070043,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070044,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070045,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070046,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070047,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070048,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070049,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070050,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070051,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070052,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070053,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070054,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070055,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070056,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070057,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070058,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070059,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070060,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070061,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070062,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070063,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070064,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070065,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070066,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070067,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070068,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070069,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070070,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070071,4,"YW_JXG_OnDie")
aux.RegisterCreatureEvent(1070072,4,"YW_JXG_OnDie")



