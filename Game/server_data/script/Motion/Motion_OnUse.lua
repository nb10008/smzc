
function Motion_OnUse(MapID,InstanceID,RoleID, DstRoleID, MotionID)
		local FreeSize = role.GetBagFreeSize(RoleID)
		local i = role.GetRoleItemNum(RoleID, 4010242)
		local a = role.GetRoleItemNum(RoleID, 4040011)--Óµ±§¿¨
		local b = role.GetRoleItemNum(RoleID, 4040012)--ÒÀ¿¿¿¨
		local c = role.GetRoleItemNum(RoleID, 4040013)--ÒÀÙË¿¨
		local d = role.GetRoleItemNum(RoleID, 4040014)--Ç×ÃÜ¿¨
		local e = role.GetRoleItemNum(RoleID, 4040015)--Ç×ÎÇ¿¨
		local qingrenjie = role.GetRoleItemNum(RoleID, 4800257)--ÇéÈË½ÚÇ×ÎÇ¿¨
	if(FreeSize >= 1) then
			if MotionID==14 then
				if i >= 1 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4010226,1 , -1, 8, 420)--Ç×ÎÇÖ®Ó¡
				end
				if e >= 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4040026,1 , -1, 8, 420)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4040015, 1, 420)
				end
				if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30378) == true and role.GetRoleLoverID(MapID, InstanceID,RoleID) ~= nil and role.GetRoleLoverID(MapID, InstanceID,RoleID) ==  DstRoleID then
					if role.GetRoleItemNum(RoleID, 4800112) > 0 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, 4800112, 1, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 4800113, 1, -1, 8, 420)
						--role.RemoveFromRole(MapID, InstanceID, DstRoleID, 4800112, 1, 420)
						--role.AddRoleItem(MapID, InstanceID, DstRoleID, 4800113, 1, -1, 8, 420)
					    unit.AddBuff(MapID, InstanceID, RoleID, 6001301, RoleID)
					end
					if role.GetRoleItemNum(DstRoleID, 4800112) > 0 and role.IsRoleHaveQuest(MapID, InstanceID, DstRoleID, 30378) == true  then
					    role.RemoveFromRole(MapID, InstanceID, DstRoleID, 4800112, 1, 420)
						role.AddRoleItem(MapID, InstanceID, DstRoleID, 4800113, 1, -1, 8, 420)
						unit.AddBuff(MapID, InstanceID, DstRoleID, 6001301, DstRoleID)
	                end
				end
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleID)
				if act.GetActIsStart(53) and role.IsRoleHaveBuff(Target_MapID, Target_InstanceID,RoleID,69400) ~= true then
					if role.IsRoleHaveQuest(Target_MapID,Target_InstanceID,RoleID, 30912) == true and FreeSize > 1 and qingrenjie >= 1 and role.GetRoleItemNum(RoleID, 4800265) < 5 then
						role.AddRoleItem(Target_MapID, Target_InstanceID, RoleID, 4800265, 1, -1, 8, 420)
						local lastId = role.GetRoleScriptData(RoleID, 1, RoleDataType["qingrenjieqinwenID"])
						local num = role.GetRoleScriptData(RoleID, 1, RoleDataType["qingrenjieqinwenNum"])
						if num == nil or num < 0 then
							num =0
						end
						if(lastId ~= nil and lastId ~= 0 and lastId ~= DstRoleID) then
							role.SetRoleScriptData(RoleID, 1, RoleDataType["qingrenjieqinwenNum"],num+1)
						end
						role.SetRoleScriptData(RoleID, 1, RoleDataType["qingrenjieqinwenID"],DstRoleID)
					end
					unit.AddBuff(Target_MapID, Target_InstanceID, RoleID, 6940001, RoleID)
				end
			elseif MotionID==15 then
				if c >= 3 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4040026,1 , -1, 8, 420)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4040013, 3, 420)
				end
			elseif MotionID==16 then
				if i >= 1 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4010227,1 , -1, 8, 420)--Óµ±§Ö®Ó¡
				end
				if a >= 3 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4040026,1 , -1, 8, 420)
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4040011, 3, 420)
				end
				if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30377) == true and role.GetRoleLoverID(MapID, InstanceID,RoleID) ~= nil and role.GetRoleLoverID(MapID, InstanceID,RoleID) ==  DstRoleID then
					if role.GetRoleItemNum(RoleID, 4800111) > 0 then
						role.RemoveFromRole(MapID, InstanceID, RoleID, 4800111, 1, 420)
						role.AddRoleItem(MapID, InstanceID, RoleID, 4800112, 1, -1, 8, 420)
						--role.RemoveFromRole(MapID, InstanceID, DstRoleID, 4800111, 1, 420)
						--role.AddRoleItem(MapID, InstanceID, DstRoleID, 4800112, 1, -1, 8, 420)
						unit.AddBuff(MapID, InstanceID, RoleID, 6001301, RoleID)
					end
					if role.GetRoleItemNum(DstRoleID, 4800111) > 0 and role.IsRoleHaveQuest(MapID, InstanceID, DstRoleID, 30377) == true then
					    role.RemoveFromRole(MapID, InstanceID, DstRoleID, 4800111, 1, 420)
						role.AddRoleItem(MapID, InstanceID, DstRoleID, 4800112, 1, -1, 8, 420)
						unit.AddBuff(MapID, InstanceID, DstRoleID, 6001301, DstRoleID)
					end
				end
			elseif MotionID==17 then
				if i >= 1 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4010228,1 , -1, 8, 420)--Ç×ÃÜÖ®Ó¡
				end
				if d >= 1 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4040026,1 , -1, 8, 420)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4040014, 1, 420)
				end
			elseif MotionID==20 then
				if b >= 3 then
					role.AddRoleItem(MapID, InstanceID, RoleID, 4040026,1 , -1, 8, 420)
					role.RemoveFromRole(MapID, InstanceID, RoleID, 4040012, 3, 420)
				end
			end

	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
		return bRet, bIgnore
	end
end

-- ×¢²áÏàÓ¦ÊÂ¼þ
aux.RegisterMotionEvent(0, "Motion_OnUse")
