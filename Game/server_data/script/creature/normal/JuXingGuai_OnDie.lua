

function JuXingGuai_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local ID = 0
	local yaojing = 0
    if TargetTypeID >= 1080005 and TargetTypeID <= 1080014 then
		local k = math.random(100)
		--新增新手杀巨型怪刷出妖精的判断
		local times = role.GetRoleScriptData(RoleID, 1, RoleDataType["xinshouyaojing"])
		if(TargetTypeID ~= 1080011) then  --去除23级的精英巨型怪
			if(times%10 == 0) then
				if(k > 410 and k<= 500) then
					yaojing = 1092001
					ID = map.MapCreateCreature(MapID, InstanceID, 1092001, x, y, z)
				elseif(k > 510 and k<= 600) then
					yaojing = 1092006
					ID = map.MapCreateCreature(MapID, InstanceID, 1092006, x, y, z)
				end
				role.SetRoleScriptData(RoleID, 1, RoleDataType["xinshouyaojing"], times+1)
			end
		end
    elseif TargetTypeID >= 1080015 and TargetTypeID <= 1080043 then
		local k = math.random(100)
		if k == 49 then
		    yaojing = 1092001
			ID = map.MapCreateCreature(MapID, InstanceID, 1092001, x, y, z)
			--北州天雪刷妖计数
			UpdateSeverData(41,1)
		elseif k == 50 then
		    yaojing = 1092006
			ID = map.MapCreateCreature(MapID, InstanceID, 1092006, x, y, z)
			--北州天雪刷妖计数
			UpdateSeverData(41,1)
		end
		--新增新手杀巨型怪刷出妖精的判断
		local times = role.GetRoleScriptData(RoleID, 1, RoleDataType["xinshouyaojing"])
		if(TargetTypeID >= 1080015 and TargetTypeID <= 1080019) then
			if(times%100 < 10) then
				if(k > 410 and k<= 500) then
					yaojing = 1092001
					ID = map.MapCreateCreature(MapID, InstanceID, 1092001, x, y, z)
				elseif(k > 510 and k<= 600) then
					yaojing = 1092006
					ID = map.MapCreateCreature(MapID, InstanceID, 1092006, x, y, z)
				end
				role.SetRoleScriptData(RoleID, 1, RoleDataType["xinshouyaojing"], times+10)
			end
		end
    elseif TargetTypeID >= 1080044 and TargetTypeID <= 1080075 then
  	    local k = math.random(100)
		if k == 49 then
		    yaojing = 1092002
			ID = map.MapCreateCreature(MapID, InstanceID, 1092002, x, y, z)
			--西州幻林刷妖计数
			UpdateSeverData(42,1)
		elseif k == 50 then
		    yaojing = 1092007
			ID = map.MapCreateCreature(MapID, InstanceID, 1092007, x, y, z)
			--西州幻林刷妖计数
			UpdateSeverData(42,1)
		end
	elseif (TargetTypeID >= 1080076 and TargetTypeID <= 1080085) or (TargetTypeID >= 1080090 and TargetTypeID <= 1080099) then
		local k = math.random(100)
		if k == 49 then
			yaojing = 1092003
			ID = map.MapCreateCreature(MapID, InstanceID, 1092003, x, y, z)
			--南州迷沼刷妖计数
			UpdateSeverData(43,1)
		elseif k == 50 then
			yaojing = 1092008
			ID = map.MapCreateCreature(MapID, InstanceID, 1092008, x, y, z)
			--南州迷沼刷妖计数
			UpdateSeverData(43,1)
		end
	elseif (TargetTypeID >= 1080100 and TargetTypeID <= 1080109) or (TargetTypeID >= 1080121 and TargetTypeID <= 1080129) then
        local k = math.random(100)
		if k == 49 then
		    yaojing = 1092004
			ID = map.MapCreateCreature(MapID, InstanceID, 1092004, x, y, z)
			--东州冥山刷妖计数
			UpdateSeverData(44,1)
		elseif k == 50 then
		    yaojing = 1092009
			ID = map.MapCreateCreature(MapID, InstanceID, 1092009, x, y, z)
			--东州冥山刷妖计数
			UpdateSeverData(44,1)
		end
	elseif (TargetTypeID >= 1080110 and TargetTypeID <= 1080120) or (TargetTypeID >= 1080130 and TargetTypeID <= 1080140) then
	    local k = math.random(100)
		if k == 49 then
		    yaojing = 1092005
			ID = map.MapCreateCreature(MapID, InstanceID, 1092005, x, y, z)
			--东州冥山刷妖计数
			UpdateSeverData(44,1)
		elseif k == 50 then
		    yaojing = 1092010
			ID = map.MapCreateCreature(MapID, InstanceID, 1092010, x, y, z)
			--东州冥山刷妖计数
			UpdateSeverData(44,1)
		end
	end
	--[[if yaojing ~= 0 then
	    unit.AddBuff(MapID, InstanceID, ID, 5932801, ID)
		if(MapID == 3424072232 or MapID == 3424075048) then
			local MsgID = msg.BeginMsgEvent()  --刷出妖精的场景广播去掉
			msg.AddMsgEvent(MsgID, 101, 302002)
			msg.AddMsgEvent(MsgID, 5, yaojing)
			msg.AddMsgEvent(MsgID, 9, x)
			msg.AddMsgEvent(MsgID, 9, z)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 13, 1)
			msg.AddMsgEvent(MsgID, 1, 302002)
			msg.AddMsgEvent(MsgID, 5, yaojing)
			msg.AddMsgEvent(MsgID, 9, x)
			msg.AddMsgEvent(MsgID, 9, z)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		end
	end]]
end
aux.RegisterCreatureEvent(1080005,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080006,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080007,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080008,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080009,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080010,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080012,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080013,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080014,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080015,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080016,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080017,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080018,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080019,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080020,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080021,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080022,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080023,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080024,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080025,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080026,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080027,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080028,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080029,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080030,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080031,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080032,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080033,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080034,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080035,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080036,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080037,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080038,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080039,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080040,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080041,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080042,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080043,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080044,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080045,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080046,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080047,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080048,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080049,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080050,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080051,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080052,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080053,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080054,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080055,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080056,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080057,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080058,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080059,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080060,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080061,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080062,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080063,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080064,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080065,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080066,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080067,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080068,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080069,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080070,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080071,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080072,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080073,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080074,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080075,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080076,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080077,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080078,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080079,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080080,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080081,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080082,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080083,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080084,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080085,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080086,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080087,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080088,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080089,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080090,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080091,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080092,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080093,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080094,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080095,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080096,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080097,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080098,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080099,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080100,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080101,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080102,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080103,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080104,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080105,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080106,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080107,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080108,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080109,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080110,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080111,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080112,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080113,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080114,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080115,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080116,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080117,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080118,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080119,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080120,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080121,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080122,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080123,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080124,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080125,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080126,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080127,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080128,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080129,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080130,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080131,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080132,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080133,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080134,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080135,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080136,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080137,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080138,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080139,4,"JuXingGuai_OnDie")
aux.RegisterCreatureEvent(1080140,4,"JuXingGuai_OnDie")



function juxingyaojing_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local tbl_JXG = {}
	tbl_JXG[1092001] = 6010048
	tbl_JXG[1092002] = 6010052
	tbl_JXG[1092003] = 6010053
	tbl_JXG[1092004] = 6010054
	tbl_JXG[1092005] = 6010055
	tbl_JXG[1092006] = 6010056
	tbl_JXG[1092007] = 6010057
	tbl_JXG[1092008] = 6010058
	tbl_JXG[1092009] = 6010059
	tbl_JXG[1092010] = 6010060
	tbl_JXG[1092020] = 6010073
	tbl_JXG[1092021] = 6010072

    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, tbl_JXG[TargetTypeID], x, y, z)
end

aux.RegisterCreatureEvent(1092001,4,"juxingyaojing_OnDie")
aux.RegisterCreatureEvent(1092002,4,"juxingyaojing_OnDie")
aux.RegisterCreatureEvent(1092003,4,"juxingyaojing_OnDie")
aux.RegisterCreatureEvent(1092004,4,"juxingyaojing_OnDie")
aux.RegisterCreatureEvent(1092005,4,"juxingyaojing_OnDie")
aux.RegisterCreatureEvent(1092006,4,"juxingyaojing_OnDie")
aux.RegisterCreatureEvent(1092007,4,"juxingyaojing_OnDie")
aux.RegisterCreatureEvent(1092008,4,"juxingyaojing_OnDie")
aux.RegisterCreatureEvent(1092009,4,"juxingyaojing_OnDie")
aux.RegisterCreatureEvent(1092010,4,"juxingyaojing_OnDie")
aux.RegisterCreatureEvent(1092020,4,"juxingyaojing_OnDie")
aux.RegisterCreatureEvent(1092021,4,"juxingyaojing_OnDie")


