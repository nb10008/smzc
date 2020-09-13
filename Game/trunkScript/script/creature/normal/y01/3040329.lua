----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记3040306是否死亡 0未死亡 1 死亡
----scriptdata[2]标记3040307是否死亡 0未死亡 1 死亡
----scriptdata[3]标记3040332是否存在 0不存在 1 存在

----scriptdata[4]标记3040317是否死亡 0未死亡 1 死亡
----scriptdata[5]标记3040318是否死亡 0未死亡 1 死亡
----scriptdata[6]标记3040319是否死亡 0未死亡 1 死亡

--最后未刷宝箱，因为无掉落





function s5936501_Cast(MapID, InstanceID, SkillID, OwnerID)

	if MapID==3870157173 then
		
		local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)

		if TargetID and TargetID~=4294967295 then
			local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)

			if TeamID and TeamID~=4294967295 then
				local Role = {}
				Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
				for i=1, 6 do

					if Role[i] and Role[i]~= 4294967295 then
						local position={{x=183,y=2,z=230},{x=166,y=2,z=208},{x=119,y=2,z=208},{x=102,y=2,z=228},{x=100,y=2,z=256},{x=115,y=2,z=276},{x=140,y=2,z=284}}
						local positionID=math.random(1,7)

						if Role[i] and Role[i]~=-1 then
							role.RoleGotoNewMap(MapID, InstanceID, Role[i], MapID,  position[positionID].x, position[positionID].y, position[positionID].z)
							unit.AddBuff(MapID, InstanceID, Role[i],5055701,OwnerID)
						end
					end
				end
			end
		end

	end


end
aux.RegisterSkillEvent(5936501, 1, "s5936501_Cast")




function c3040329_OnEnterCombat(MapID, InstanceID, CreatureID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100142)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10208)
	unit.AddBuff(MapID, InstanceID, CreatureID,5938001,CreatureID)

end

aux.RegisterCreatureEvent(3040329, 2, "c3040329_OnEnterCombat")



function c3040329_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	cre.MonsterSay(MapID, InstanceID, TargetID, 10209)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100143)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	

end

aux.RegisterCreatureEvent(3040329, 4, "c3040329_OnDie")
