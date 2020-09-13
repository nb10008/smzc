--熊猫刷新f20=1232817742
--[[
XMS_zhihun = 0
XMYX_zhihun = 0
XIONGMAO_cre = {}
XIONGMAO_cre[1] = 0
XIONGMAO_cre[2] = 0
XIONGMAO_cre[3] = 0
XIONGMAO_cre[4] = 0
XIONGMAO_cre[5] = 0
XIONGMAO_cre[6] = 0
XIONGMAO_cre[7] = 0
XIONGMAO_cre[8] = 0
XIONGMAO_cre[9] = 0
XIONGMAO_cre[10] = 0
XIONGMAO_cre[11] = 0
XIONGMAO_cre[12] = 0
XIONGMAO_cre[13] = 0
XIONGMAO_cre[14] = 0
XIONGMAO_cre[15] = 0
XIONGMAO_cre[16] = 0
XIONGMAO_cre[17] = 0
XIONGMAO_cre[18] = 0
XIONGMAO_cre[19] = 0
XIONGMAO_cre[20] = 0
XIONGMAO_cre[21] = 0
XIONGMAO_cre[22] = 0
XIONGMAO_cre[23] = 0
XIONGMAO_cre[24] = 0
XIONGMAO_cre[25] = 0
XIONGMAO_cre[26] = 0
XIONGMAO_cre[27] = 0
XIONGMAO_cre[28] = 0
XIONGMAO_cre[29] = 0
XIONGMAO_cre[30] = 0
XIONGMAO_cre[31] = 0
XIONGMAO_cre[32] = 0
XIONGMAO_cre[33] = 0
XIONGMAO_cre[34] = 0
XIONGMAO_cre[35] = 0
XIONGMAO_cre[36] = 0
XIONGMAO_cre[37] = 0
XIONGMAO_cre[38] = 0
XIONGMAO_cre[39] = 0
XIONGMAO_cre[40] = 0
function Xiongmao_OnTimerMin(actID)
	Jiaozi_OnTimerMin(actID)
	local curmon = tonumber(os.date("%m"))--月
	local curday = tonumber(os.date("%d"))--日
	local curyear = tonumber(os.date("%Y"))--年
	local curhour = tonumber(os.date("%H"))--小时
	local curmin = tonumber(os.date("%M"))--分钟

	local BOSSID = {}
	BOSSID[1] = {}
	BOSSID[2] = {}
	BOSSID[1][1] = {a = 3040363}
	BOSSID[1][2] = {a = 3040364}
	BOSSID[1][3] = {a = 3040365}
	BOSSID[1][4] = {a = 3040366}
	BOSSID[2][1] = {a = 3040367}
	BOSSID[2][2] = {a = 3040368}
	BOSSID[2][3] = {a = 3040369}
	local XBOSSID = {}                                                                                                                                         XBOSSID[1] = {}
	XBOSSID[2] = {}
	XBOSSID[1][1] = {a = 3040355}
	XBOSSID[1][2] = {a = 3040356}
	XBOSSID[1][3] = {a = 3040357}
	XBOSSID[1][4] = {a = 3040358}
	XBOSSID[2][1] = {a = 3040359}
	XBOSSID[2][2] = {a = 3040360}
	XBOSSID[2][3] = {a = 3040361}
	XBOSSID[2][4] = {a = 3040362}

	local XIONGMAO_cre = {}
	XIONGMAO_cre[1] = {creID =3040343, x =2175, y =9049, z =1143 }
	XIONGMAO_cre[2] = {creID =3040343, x =1704, y =11546, z =1699 }
	XIONGMAO_cre[3] = {creID =3040343, x =1180, y =9396, z =2106 }
	XIONGMAO_cre[4] = {creID =3040343, x =975, y =7377, z =2579 }
	XIONGMAO_cre[5] = {creID =3040343, x =927, y =9024, z =3226 }
	XIONGMAO_cre[6] = {creID =3040343, x =1978, y =13918, z =3182 }
	XIONGMAO_cre[7] = {creID =3040343, x =2497, y =14341, z =3286 }
	XIONGMAO_cre[8] = {creID =3040343, x =2431, y =7985, z =2149 }
	XIONGMAO_cre[9] = {creID =3040343, x =2831, y =7536, z =2591 }
	XIONGMAO_cre[10] = {creID =3040343, x =3553, y =10394, z =1340 }
	XIONGMAO_cre[11] = {creID =3040344, x =2525, y =4337, z =1897 }
	XIONGMAO_cre[12] = {creID =3040344, x =1879, y =5957, z =1807 }
	XIONGMAO_cre[13] = {creID =3040344, x =1452, y =5828, z =1256 }
	XIONGMAO_cre[14] = {creID =3040344, x =1879, y =3100, z =479 }
	XIONGMAO_cre[15] = {creID =3040344, x =902, y =6175, z =2154 }
	XIONGMAO_cre[16] = {creID =3040344, x =1751, y =5748, z =1805 }
	XIONGMAO_cre[17] = {creID =3040344, x =2475, y =4197, z =1875 }
	XIONGMAO_cre[18] = {creID =3040344, x =2609, y =4800, z =1564 }
	XIONGMAO_cre[19] = {creID =3040344, x =1998, y =5854, z =1115 }
	XIONGMAO_cre[20] = {creID =3040344, x =2098, y =2920, z =1407 }
	XIONGMAO_cre[21] ={creID =3040345, x =1081, y =12955, z =1441 }
	XIONGMAO_cre[22] ={creID =3040345, x =1854, y =3456, z =1488 }
	XIONGMAO_cre[23] ={creID =3040345, x =1161, y =13010, z =1457 }
	XIONGMAO_cre[24] ={creID =3040345, x =1468, y =3586, z =1162 }
	XIONGMAO_cre[25] ={creID =3040345, x =2456, y =3634, z =1701 }
	XIONGMAO_cre[26] ={creID =3040345, x =2267, y =3013, z =1986 }
	XIONGMAO_cre[27] ={creID =3040345, x =1622, y =4837, z =2332 }
	XIONGMAO_cre[28] ={creID =3040345, x =1547, y =4521, z =2354 }
	XIONGMAO_cre[29] ={creID =3040345, x =1763, y =5351, z =2473 }
	XIONGMAO_cre[30] ={creID =3040345, x =740, y =13287, z =1699 }
	XIONGMAO_cre[31] ={creID =3040346, x =536, y =10494, z =2098 }
	XIONGMAO_cre[32] ={creID =3040346, x =873, y =9996, z =1990 }
	XIONGMAO_cre[33] ={creID =3040346, x =1238, y =11287, z =1435 }
	XIONGMAO_cre[34] ={creID =3040346, x =1552, y =13378, z =434 }
	XIONGMAO_cre[35] ={creID =3040346, x =1555, y =13378, z =434 }
	XIONGMAO_cre[36] ={creID =3040346, x =1497, y =13226, z =357 }
	XIONGMAO_cre[37] ={creID =3040346, x =2137, y =10489, z =357 }
	XIONGMAO_cre[38] ={creID =3040346, x =2495, y =9263, z =365 }
	XIONGMAO_cre[39] ={creID =3040346, x =2234, y =9325, z =1792 }
	XIONGMAO_cre[40] ={creID =3040346, x =1770, y =10716, z =2130 }
	if curyear == 2012 then
		if curmon == 1 then
			if curday >= 18 and curday <=31 then -- 熊猫神
				if curhour == 0 or curhour == 5 or curhour == 10 or curhour == 15 or curhour == 20 then
					if XMS_zhihun == 0 then
						XMSzhihun = map.MapCreateCreature(1232817742, -1, 3040372, 1049, 2297, 779)--1049，2297，779
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 102, 300080)  --熊猫神之魂已经出现在诺亚方舟上，勇士们快去争夺啊
						msg.DispatchWorldMsgEvent(MsgID1)
					end
				end
				if curhour <= 24 and curmin == 0 then --每整点的BOSS
					local b = math.random(1,2)
					local c = 0
					if b == 1 then
						c = math.random(1,4)
					else
						c = math.random(1,3)
					end
					map.MapCreateCreature(1232817742, -1, BOSSID[b][c].a, 1271, 2180, 1282)---- 63550   2180     64100
					local MsgID1 = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID1, 102, 300081)  -- 侵占诺亚舟的领导者已经出现在诺亚方舟停靠的附近，勇士们快去消灭它吧！！
					msg.DispatchWorldMsgEvent(MsgID1)

				end
				if curmin == 30 then--没半点的BOSS
					local d = math.random(1,2)
					local f = math.random(1,4)
					map.MapCreateCreature(1232817742, -1, XBOSSID[d][f].a, 1271, 2180, 1282)

					local MsgID1 = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID1, 102, 300081)  --侵占诺亚舟的领导者已经出现在诺亚方舟停靠的附近，勇士们快去消灭它吧！！
					msg.DispatchWorldMsgEvent(MsgID1)
				end
				if curmin == 15 or curmin == 45 then
					for i=1,40  do
						if XIONGMAO_cre[i] == 0 then
							if i >=1 and i <= 10 then
								XIONGMAO_cre[i] = map.MapCreateColCreature(3424071720, -1, XIONGMAO_cre[i].creID, XIONGMAO_cre[i].x, XIONGMAO_cre[i].y, XIONGMAO_cre[i].z, "")
							elseif i >= 11 and i <= 20 then
								XIONGMAO_cre[i] = map.MapCreateColCreature(3424072488, -1, XIONGMAO_cre[i].creID, XIONGMAO_cre[i].x, XIONGMAO_cre[i].y, XIONGMAO_cre[i].z, "")
							elseif i >= 21 and i <= 30 then
								XIONGMAO_cre[i] = map.MapCreateColCreature(3424072232, -1, XIONGMAO_cre[i].creID, XIONGMAO_cre[i].x, XIONGMAO_cre[i].y, XIONGMAO_cre[i].z, "")
							elseif i >= 21 and i <= 30 then
								XIONGMAO_cre[i] = map.MapCreateColCreature(3424075048, -1, XIONGMAO_cre[i].creID, XIONGMAO_cre[i].x, XIONGMAO_cre[i].y, XIONGMAO_cre[i].z, "")
							end
						end
					end
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 300082)      		--新春熊猫已经出现三界各地，快去拿起手中的武器去阻挡卖萌熊猫的入侵吧！新春熊猫只存在10分钟，大家请抓紧！
					msg.DispatchWorldMsgEvent(MsgID)

				end
				if curmin%3 == 0 or curmin == 0 then -- 熊猫英雄之魂
					if XMYX_zhihun == 0 then
						local i = math.random(3)
						local YXpoint = {}
						YXpoint[1] = {x = 1017,y = 2297,z = 755}--1017，2297，755
						YXpoint[2] = {x = 997,y = 2297,z = 829}--997，2297，829
						YXpoint[3] = {x = 1054,y = 2297,z = 746}--1054，2297，746
						XMYX_zhihun = map.MapCreateCreature(1232817742, -1, 3040371, YXpoint[i].x, YXpoint[i].y, YXpoint[i].z)
						local MsgID1 = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID1, 102, 300083)  --熊猫英雄之魂已经出现在诺亚方舟上，勇士们快去争夺啊
						msg.DispatchWorldMsgEvent(MsgID1)
					end
				end
			end
		end
	end
end
aux.RegisterActEvent(50, 4, "Xiongmao_OnTimerMin")

function XMSzhihun_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
	if TargetID == 3040372 then
		if XMS_zhihun ~= 0 then
			XMSzhihun = 0
		end
	elseif TargetID == 3040371 then
		if XMYX_zhihun ~= 0 then
			XMSzhihun = 0
		end
	else
		for i=1,40  do
			if XIONGMAO_cre[i] ~= 0 then
				XIONGMAO_cre[i] = 0
			end
		end
	end
end
aux.RegisterCreatureEvent(3040372, 13, "XMSzhihun_OnDisappear")
aux.RegisterCreatureEvent(3040343, 13, "XMSzhihun_OnDisappear")
aux.RegisterCreatureEvent(3040344, 13, "XMSzhihun_OnDisappear")
aux.RegisterCreatureEvent(3040345, 13, "XMSzhihun_OnDisappear")
aux.RegisterCreatureEvent(3040346, 13, "XMSzhihun_OnDisappear")
aux.RegisterCreatureEvent(3040371, 13, "XMSzhihun_OnDisappear")

function XMSzhihun_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	if TargetID == 3040372 then
		if XMS_zhihun ~= 0 then
			XMSzhihun = 0
		end
	elseif TargetID == 3040371 then
		if XMYX_zhihun ~= 0 then
			XMSzhihun = 0
		end
	else
		for i=1,40  do
			if XIONGMAO_cre[i] ~= 0 then
				XIONGMAO_cre[i] = 0
			end
		end
	end
end
aux.RegisterCreatureEvent(3040372, 4, "XMSzhihun_OnDie")
aux.RegisterCreatureEvent(3040343, 4, "XMSzhihun_OnDie")
aux.RegisterCreatureEvent(3040344, 4, "XMSzhihun_OnDie")
aux.RegisterCreatureEvent(3040345, 4, "XMSzhihun_OnDie")
aux.RegisterCreatureEvent(3040346, 4, "XMSzhihun_OnDie")
aux.RegisterCreatureEvent(3040371, 4, "XMSzhihun_OnDie")

function Xihunqi_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet
	local X = 0
	local YXpoint = {}
	YXpoint[1] = {x = 1012,y = 2297,z = 1175}
	YXpoint[2] = {x = 978,y = 2297,z = 1109}
	YXpoint[3] = {x = 1042,y = 2297,z = 1063}
	YXpoint[4] = {x = 987,y = 2297,z = 1038}
	YXpoint[5] = {x = 1078,y = 2297,z = 1000}
	YXpoint[6] = {x = 1038,y = 2297,z = 949}
	YXpoint[7] = {x = 979,y = 2297,z = 952}
	YXpoint[8] = {x = 1089,y = 2297,z = 1044}
	YXpoint[9] = {x = 1042,y = 2297,z = 1126}
	YXpoint[10] = {x = 1023,y = 2297,z = 1269}
	YXpoint[11] = {x = 1017,y = 2297,z = 755}--1017，2297，755
	YXpoint[12] = {x = 997,y = 2297,z = 829}--997，2297，829
	YXpoint[13] = {x = 1054,y = 2297,z = 746}--1054，2297，746
	YXpoint[14] = {x = 1049,y = 2297,z = 177}--1049，2297，779
	local x,y,z = unit.GetPosition(MapID, InstanceID, RoleID)
	if role.GetBagFreeSize(RoleID) >= 1 then
		if MapID ~= 1232817742 then
			bRet = 43
		else
			if X == 0 then
				for a = 1,14 do
					if x >= YXpoint[a].x-10 and x <= YXpoint[a].x+10 and y >= YXpoint[a].y-10 and y <= YXpoint[a].x+10 and z >= YXpoint[a].z-10 and z <= YXpoint[a].z+10 then
						X = 1
					else
						bRet = 43
					end
				end
			end
		end
	else
		bRet = 32
	end
	return bRet
end
aux.RegisterItemEvent(4830001, 0, "Xihunqi_CanUse")
aux.RegisterItemEvent(4830002, 0, "Xihunqi_CanUse")
aux.RegisterItemEvent(4830003, 0, "Xihunqi_CanUse")

function Xihunqi_OnUse(MapID, InstanceID, TypeID, RoleID)
	local YXpoint = {}
	YXpoint[1] = {x = 1012,y = 2297,z = 1175}
	YXpoint[2] = {x = 978,y = 2297,z = 1109}
	YXpoint[3] = {x = 1042,y = 2297,z = 1063}
	YXpoint[4] = {x = 987,y = 2297,z = 1038}
	YXpoint[5] = {x = 1078,y = 2297,z = 1000}
	YXpoint[6] = {x = 1038,y = 2297,z = 949}
	YXpoint[7] = {x = 979,y = 2297,z = 952}
	YXpoint[8] = {x = 1089,y = 2297,z = 1044}
	YXpoint[9] = {x = 1042,y = 2297,z = 1126}
	YXpoint[10] = {x = 1023,y = 2297,z = 1269}
	YXpoint[11] = {x = 1017,y = 2297,z = 755}--1017，2297，755
	YXpoint[12] = {x = 997,y = 2297,z = 829}--997，2297，829
	YXpoint[13] = {x = 1054,y = 2297,z = 746}--1054，2297，746
	YXpoint[14] = {x = 1049,y = 2297,z = 177}--1049，2297，779
	YXpoint[4830001] = {}
	YXpoint[4830002] = {}
	YXpoint[4830003] = {}
	YXpoint[4830001][1] = {x = 1012,y = 2297,z = 1175}
	YXpoint[4830001][2] = {x = 978,y = 2297,z = 1109}
	YXpoint[4830001][3] = {x = 1042,y = 2297,z = 1063}
	YXpoint[4830001][4] = {x = 987,y = 2297,z = 1038}
	YXpoint[4830001][5] = {x = 1078,y = 2297,z = 1000}
	YXpoint[4830001][6] = {x = 1038,y = 2297,z = 949}
	YXpoint[4830001][7] = {x = 979,y = 2297,z = 952}
	YXpoint[4830001][8] = {x = 1089,y = 2297,z = 1044}
	YXpoint[4830001][9] = {x = 1042,y = 2297,z = 1126}
	YXpoint[4830001][10] = {x = 1023,y = 2297,z = 1269}
	YXpoint[4830002][1] = {x = 1017,y = 2297,z = 755}--1017，2297，755
	YXpoint[4830002][2] = {x = 997,y = 2297,z = 829}--997，2297，829
	YXpoint[4830002][3] = {x = 1054,y = 2297,z = 746}--1054，2297，746
	YXpoint[4830003][1] = {x = 1049,y = 2297,z = 177}--1049，2297，779
	local x,y,z = unit.GetPosition(MapID, InstanceID, RoleID)
	local b = math.random(1,100)
	for a = 1,14 do
		if a >= 1 and a <= 10 then
			if x >= YXpoint[a].x-10 and x <= YXpoint[a].x+10 and y >= YXpoint[a].y-10 and y <= YXpoint[a].x+10 and z >= YXpoint[a].z-10 and z <= YXpoint[a].z+10 then
				if TypeID == 4830001 then
					if b == 30 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 4830020, 1, -1, 8, 420)
					end
				elseif TypeID == 4830002 then
					if b == 60 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 4830020, 1, -1, 8, 420)
					end
				elseif TypeID == 4830003 then
					if b == 80 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 4830020, 1, -1, 8, 420)
					end
				end
			end
		elseif a >= 11 and a <= 13 then
			if x >= YXpoint[a].x-10 and x <= YXpoint[a].x+10 and y >= YXpoint[a].y-10 and y <= YXpoint[a].x+10 and z >= YXpoint[a].z-10 and z <= YXpoint[a].z+10 then
				if TypeID == 4830001 then
					if b == 2 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 4830021, 1, -1, 8, 420)
					end
				elseif TypeID == 4830002 then
					if b == 10 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 4830021, 1, -1, 8, 420)
					end
				elseif TypeID == 4830003 then
					if b == 50 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 4830021, 1, -1, 8, 420)
					end
				end
			end
		else
			--if x >= YXpoint[1].x-10 and x <= YXpoint[1].x+10 and y >= YXpoint[1].y-10 and y <= YXpoint[1].x+10 and z >= YXpoint[1].z-10 and z <= YXpoint[1].z+10 then
				if TypeID == 4830001 then
					if b == 0 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 4830022, 1, -1, 8, 420)
					end
				elseif TypeID == 4830002 then
					if b == 1 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 4830022, 1, -1, 8, 420)
					end
				elseif TypeID == 4830003 then
					if b == 5 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 4830022, 1, -1, 8, 420)
					end
				end
			--end
		end
	end
end
aux.RegisterItemEvent(4830001, 1, "Xihunqi_OnUse")
aux.RegisterItemEvent(4830002, 1, "Xihunqi_OnUse")
aux.RegisterItemEvent(4830003, 1, "Xihunqi_OnUse")
--]]


