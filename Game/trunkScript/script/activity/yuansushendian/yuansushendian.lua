--0记录怪物死亡次数
--1记录门牌号
--2记录等级判断（1-6分别是30-39，40-49，50-59，60-69，70-79，80+）
--3怪的波数
--4地图的怪的总数
--5元素神殿
--传送进入元素神殿
local YSSD_pos = {}
	--地宫坐标
	YSSD_pos[1] = {x=80,y=40,z=145}
	YSSD_pos[2] = {x=100,y=40,z=131}
	YSSD_pos[3] = {x= 127,y=40,z=117}
	YSSD_pos[4] = {x=159,y=40,z=113}
	YSSD_pos[5] = {x=180,y=1534,z=139}
	YSSD_pos[6] = {x=225,y=1534,z=132}
	YSSD_pos[7] = {x=220,y=1534,z=179}
	YSSD_pos[8] = {x=221,y=1534,z=87}
	YSSD_pos[9] = {x=214,y=1534,z=34}
	YSSD_pos[10] = {x=172,y=421,z=56}
	--天宫
	YSSD_pos[101] = {x=37,y=40,z=172}
	YSSD_pos[102] = {x=34,y=40,z=128}
	YSSD_pos[103] = {x=26,y=40,z=85}
	YSSD_pos[104] = {x=69,y=784,z=65}
	YSSD_pos[105] = {x=90,y=784,z=76}
	YSSD_pos[106] = {x=119,y=784,z=74}
	YSSD_pos[107] = {x=117,y=784,z=42}
	YSSD_pos[108] = {x=115,y=784,z=23}
	YSSD_pos[109] = {x=62,y=784,z=36}
	YSSD_pos[110] = {x=35,y=784,z=31}

--副本开启对话
function shendian_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 206535)		--“你想要挑战元素神殿？看看自己的实力自己选择天支线或者地支线吧，地支线比较容易，而且免费进入。天支线则极为困难，而且你必须交纳3个天地票才可开启天支线的大门。”
		msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21, 4)
		msg.AddMsgEvent(MsgID, 1, 206536)		--“上交5个天地票，开启天支线”
		msg.AddMsgEvent(MsgID, 21, 5)
		msg.AddMsgEvent(MsgID, 1, 206537)		--“开启地支线”
		msg.AddMsgEvent(MsgID, 21, 6)
		msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then--开启天宫
		if role.GetRoleItemNum(RoleID, 4800084) >= 5 then
			if map.GetMapScriptData(MapID, InstanceID, 1, 3) == 0 then
				role.RemoveFromRole(MapID, InstanceID, RoleID, 4800084, 5, 420)--天地票
				map.OpenCloseDoor(MapID, InstanceID, 321, 1)
				map.SetMapScriptData(MapID, InstanceID, 1, 1, 1)
				map.SetMapScriptData(MapID, InstanceID, 1, 3, 1)
				map.SetMapScriptData(MapID, InstanceID, 1, 5, 1)
				YSSDD_CreatCreature(MapID, InstanceID)
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 206544)--天支线的大门已经开启
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			else
				local QuestID1 = msg.BeginMsgEvent()
				msg.AddMsgEvent(QuestID1, 13, 0)
				msg.AddMsgEvent(QuestID1, 1, 206546)--门已经开启，不用再次开启
				msg.DispatchRoleMsgEvent(RoleID, QuestID1)
			end
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 206538)		--“你身上没有5个天地票是无法进入天支线的”
			msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21, 4)
			msg.AddMsgEvent(MsgID, 1, 206536)		--“上交5个天地票，开启天支线”
			msg.AddMsgEvent(MsgID, 21, 6)
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 5 then--开启地宫
		if map.GetMapScriptData(MapID, InstanceID, 1, 3) == 0 then
			map.OpenCloseDoor(MapID, InstanceID, 331, 1)
			map.SetMapScriptData(MapID, InstanceID, 1, 1, 1)
			map.SetMapScriptData(MapID, InstanceID, 1, 3, 1)
			map.SetMapScriptData(MapID, InstanceID, 1, 5, 2)
			YSSDD_CreatCreature(MapID, InstanceID)
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 206545)--地支线的大门已经开启
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		else
			local QuestID1 = msg.BeginMsgEvent()
			msg.AddMsgEvent(QuestID1, 13, 0)
			msg.AddMsgEvent(QuestID1, 1, 206546)--门已经开启，不用再次开启
			msg.DispatchRoleMsgEvent(RoleID, QuestID1)
		end
	end
end
aux.RegisterCreatureEvent(1085205, 7, "shendian_OnTalk")
--
--地宫副本刷怪
function YSSDD_CreatCreature(MapID, InstanceID)
	local RoleTbl = map.GetMapAllRoleID(MapID, InstanceID)
	local totallevel = 0
	local num = 0
	local Player = {}
	local index = 0
	for u,v in pairs(RoleTbl) do
		if u ~= nil then
			local level = role.GetRoleLevel(MapID, InstanceID, v)
			totallevel = totallevel + level
			num = num + 1
			index = index + 1
			Player[index] = v
		end
	end
	local k = 1
	if num == 0 then
	    k = map.GetMapScriptData(MapID,InstanceID,1,2)
	else
	    k = math.floor(totallevel/num/10) - 2
		if k > 6 then
		   k = 6
		elseif k < 1 then
		   k = 1
		end
		map.SetMapScriptData(MapID,InstanceID,1,2,k)
	end
	local monster = {}--怪
	--地宫
	monster[1] = {mon = {1097001}, num = {6}}--门1开启
	monster[2] = {mon = {1097001, 1097007}, num = {4, 2}}--1波
	monster[3] = {mon = {1097001, 1097007, 1097013}, num = {4, 1, 1}}--二波
	monster[4] = {mon = {1097001}, num = {6}}--门2开启
	monster[5] = {mon = {1097001, 1097007}, num = {4, 3}}--1
	monster[6] = {mon = {1097001, 1097013}, num = {4, 3}}--2
	monster[7] = {mon = {0}, num = {0}}--门3单写
	monster[8] = {mon = {1097007, 1097013}, num = {4, 2}}--门4
	monster[9] = {mon = {1097013, 1097019}, num = {4, 2}}--1
	monster[10] = {mon = {1097007, 1097019}, num = {6, 2}}--2
	monster[11] = {mon = {1097013, 1097019}, num = {4, 2}}--门5	 
	monster[12] = {mon = {1097007, 1097019}, num = {4, 2}}--1
	monster[13] = {mon = {1097001, 1097019, 1097025}, num = {8, 1, 1}}--2
	monster[14] = {mon = {1097001,0}, num = {4,0}}--门6
	monster[15] = {mon = {1097007, 1097025}, num = {6, 2}}--门7
	monster[16] = {mon = {1097019, 1097001, 1097025}, num = {4, 6, 1}}--1
	monster[17] = {mon = {1097013, 1097001}, num = {6, 5}}--2
	monster[18] = {mon = {1097013, 1097007, 1097019}, num = {6, 2, 1}}--门8
	monster[19] = {mon = {1097001, 1097019, 1097025}, num = {6, 1, 2}}--1
	monster[20] = {mon = {1097019, 1097025}, num = {6, 2}}--2
	monster[21] = {mon = {1097013, 1097019, 1097025,0}, num = {1, 1, 1,0}}--门9
	monster[22] = {mon = {0}, num = {0}}--门10其它单写
	--天宫
	monster[101] = {mon = {1097101}, num = {8}}--门1开启
	monster[102] = {mon = {1097101, 1097107}, num = {4, 4}}--1波
	monster[103] = {mon = {1097119, 1097107}, num = {4, 4}}--二波
	monster[104] = {mon = {1097107, 1097113}, num = {6, 2}}--门2开启
	monster[105] = {mon = {1097113, 1097119, 1097101}, num = {2, 2, 4}}--1
	monster[106] = {mon = {1097125, 1097101, 1097113}, num = {4, 4, 2}}--2
	monster[107] = {mon = {0}, num = {0}}--门3单写
	monster[108] = {mon = {1097019, 1097101}, num = {3, 6}}--门4
	monster[109] = {mon = {1097113, 1097107}, num = {3, 6}}--1
	monster[110] = {mon = {1097101, 1097125}, num = {8, 2}}--2
	monster[111] = {mon = {1097101, 1097113, 1097107}, num = {6, 2, 2}}--门5
	monster[112] = {mon = {1097107, 1097101, 1097119}, num = {6, 2, 2}}--1
	monster[113] = {mon = {1097107, 1097101, 1097113}, num = {4, 4, 1}}--2
	monster[114] = {mon = {1097125, 1097113, 0}, num = {1, 1, 0}}--门6
	monster[115] = {mon = {1097107, 1097119, 1097125}, num = {6, 3, 1}}--门7
	monster[116] = {mon = {1097107, 1097119, 1097113, 1097125}, num = {5, 3, 3, 1}}--1
	monster[117] = {mon = {1097107, 1097119, 1097113, 1097125}, num = {4, 4, 2, 2}}--2
	monster[118] = {mon = {0}, num = {0}}--门8
	monster[119] = {mon = {1097125, 0}, num = {2, 0}}--门9
	monster[120] = {mon = {0}, num = {0}}--门10

	--坐标点
	local function GetNewYSSD_position(YSSD_pos1)
		local tbl_YSSD_pos = {}
		tbl_YSSD_pos[1] = {x=YSSD_pos1.x, y=YSSD_pos1.y,z=YSSD_pos1.z}
		tbl_YSSD_pos[2] = {x=YSSD_pos1.x, y=YSSD_pos1.y,z=YSSD_pos1.z+3}
		tbl_YSSD_pos[3] = {x=YSSD_pos1.x+3, y=YSSD_pos1.y,z=YSSD_pos1.z}
		tbl_YSSD_pos[4] = {x=YSSD_pos1.x, y=YSSD_pos1.y,z=YSSD_pos1.z-3}
		tbl_YSSD_pos[5] = {x=YSSD_pos1.x-3, y=YSSD_pos1.y,z=YSSD_pos1.z}
		tbl_YSSD_pos[6] = {x=YSSD_pos1.x+3, y=YSSD_pos1.y,z=YSSD_pos1.z+3}
		tbl_YSSD_pos[7] = {x=YSSD_pos1.x+3, y=YSSD_pos1.y,z=YSSD_pos1.z-3}
		tbl_YSSD_pos[8] = {x=YSSD_pos1.x-3, y=YSSD_pos1.y,z=YSSD_pos1.z+3}
		tbl_YSSD_pos[9] = {x=YSSD_pos1.x-3, y=YSSD_pos1.y,z=YSSD_pos1.z-3}
		tbl_YSSD_pos[10] = {x=YSSD_pos1.x, y=YSSD_pos1.y,z=YSSD_pos1.z+6}
		tbl_YSSD_pos[11] = {x=YSSD_pos1.x, y=YSSD_pos1.y,z=YSSD_pos1.z-6}
		tbl_YSSD_pos[12] = {x=YSSD_pos1.x+6, y=YSSD_pos1.y,z=YSSD_pos1.z+6}
		return tbl_YSSD_pos
	end

	local m = map.GetMapScriptData(MapID, InstanceID, 1, 5)
	local door = 0
	local x = 0
	if m == 1 then
		door = map.GetMapScriptData(MapID,InstanceID,1,1)+100
		x = map.GetMapScriptData(MapID,InstanceID,1,3)+100
	elseif m == 2 then
		door = map.GetMapScriptData(MapID,InstanceID,1,1)
		x = map.GetMapScriptData(MapID,InstanceID,1,3)
	end
	local newYSSD_pos = GetNewYSSD_position(YSSD_pos[door]) -------------------------
	map.SetMapScriptData(MapID, InstanceID, 1, 4, 0)
	--local y = map.GetMapScriptData(MapID, InstanceID, 1, 5)
	if ( x >= 1 and x <= 22) or ( x >= 101 and x <= 120) then
		for u,v in pairs(monster[x].mon) do
			if u ~= nil then
				if v ~= 0 then
					for i = 1,monster[x].num[u] do
						local MonID = map.MapCreateColCreature(MapID, InstanceID, v+k-1, newYSSD_pos[i].x, newYSSD_pos[i].y,newYSSD_pos[i].z, true, "")
						if index and index > 0 then
							cre.AddEnmity(MapID, InstanceID, MonID, Player[math.random(1,index)], 1)
						end
					end
				end
			end
			map.SetMapScriptData(MapID, InstanceID, 1, 4, map.GetMapScriptData(MapID, InstanceID, 1, 4)+monster[x].num[u])
		end
	--map.SetMapScriptData(MapID, InstanceID, 1, 4, map.GetMapScriptData(MapID, InstanceID, 1, 4)+CreatureNum)
	map.SetMapScriptData(MapID, InstanceID, 1, 0, map.GetMapScriptData(MapID, InstanceID, 1, 4))
	YSSD_boss(MapID, InstanceID, x)
	
	end
end
--副本BOSS
function YSSD_boss(MapID, InstanceID, siwang)
	local monster = {}
	monster[7] = true
	monster[14] = true
	monster[21] = true
	monster[22] = true
	monster[107] = true
	monster[114] = true
	monster[118] = true
	monster[119] = true
	monster[120] = true
	if monster[siwang] then
		local boss = {}
		boss[1] ={fuben = {2056040, 2056041, 2056042}, xiyou = 2056045, shouling = {2056043, 2056044}, digong= {1097201, 1097207, 1097213, 1097219}}
		boss[2] ={fuben = {2056024, 2056025, 2056026}, xiyou = 2056016, shouling = {2056001, 2056002}, digong= {1097202, 1097208, 1097214, 1097220}}
		boss[3] ={fuben = {2056022, 2056023, 2056027}, xiyou = 2056017, shouling = {2056003, 2056004}, digong= {1097203, 1097209, 1097215, 1097221}}
		boss[4] ={fuben = {2056028, 2056029, 2056030}, xiyou = 2056019, shouling = {2056005, 2056006}, digong= {1097204, 1097210, 1097216, 1097222}}
		boss[5] ={fuben = {2056031, 2056032, 2056033}, xiyou = 2056018, shouling = {2056008, 2056009}, digong= {1097205, 1097211, 1097217, 1097223}}
		boss[6] ={fuben = {2056034, 2056035, 2056036}, xiyou = 2056020, shouling = {2056010, 2056012}, digong= {1097206, 1097212, 1097218, 1097224}}

		boss[11] ={fuben = {2057040, 2057041, 2057042}, xiyou = 2057045, shouling = {2057043, 2057044}, digong= {1097301, 1097307, 1097313, 1097319, 1097325, 1097331, 1097337}}
		boss[12] ={fuben = {2057024, 2057025, 2057026}, xiyou = 2057016, shouling = {2057001, 2057002}, digong= {1097302, 1097308, 1097314, 1097320, 1097326, 1097332, 1097338}}
		boss[13] ={fuben = {2057022, 2057023, 2057027}, xiyou = 2057017, shouling = {2057003, 2057004}, digong= {1097303, 1097309, 1097315, 1097321, 1097327, 1097333, 1097339}}
		boss[14] ={fuben = {2057028, 2057029, 2057030}, xiyou = 2057019, shouling = {2057005, 2057006}, digong= {1097304, 1097310, 1097316, 1097322, 1097328, 1097334, 1097340}}
		boss[15] ={fuben = {2057031, 2057032, 2057033}, xiyou = 2057018, shouling = {2057008, 2057009}, digong= {1097305, 1097311, 1097317, 1097323, 1097329, 1097335, 1097341}}
		boss[16] ={fuben = {2057034, 2057035, 2057036}, xiyou = 2057020, shouling = {2057010, 2057012}, digong= {1097306, 1097312, 1097318, 1097324, 1097330, 1097336, 1097342}}
		local k = map.GetMapScriptData(MapID,InstanceID,1,2)
		local door = map.GetMapScriptData(MapID,InstanceID,1,1)
		local a = math.random(1, 100)
		local b = math.random(1, 2)
		local c = math.random(1, 3)
		local d = math.random(1, 4)
		local m = map.GetMapScriptData(MapID, InstanceID, 1, 5)
		if m == 1 then
			door = door + 100
			k = k + 10
		elseif m == 2 then
			door = door
			k = k
		end
		if door == 3 or door == 103 then
			if a <= 70 then
				if b == 1 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].shouling[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif b ==2 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].shouling[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				end
			elseif a > 70 and a <= 90 then
				if c == 1 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif c ==2 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif c ==3 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[3], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				end
			elseif a > 90 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].xiyou, YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			end
		elseif door == 6 or door == 106 then
			if a <= 50 then
				if b == 1 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].shouling[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif b ==2 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].shouling[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				end
			elseif a > 50 and a <= 80 then
				if c == 1 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif c ==2 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif c ==3 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[3], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				end
			elseif a > 80 and a <= 95 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].xiyou, YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif a > 95 then
				if d == 1 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif d == 2 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif d == 3 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[3], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif d == 4 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[4], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				end
			end
		elseif door == 9 or door == 108 then
			if a <= 40 then
				if b == 1 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].shouling[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif b ==2 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].shouling[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				end
			elseif a > 40 and a <= 75 then
				if c == 1 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif c ==2 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif c ==3 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[3], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				end
			elseif a > 75 and a <= 90 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].xiyou, YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif a > 90 then
				if d == 1 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif d == 2 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif d == 3 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[3], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif d == 4 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[4], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				end
			end
		elseif  door == 109 then
			if a <= 30 then
				if c == 1 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif c ==2 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif c ==3 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].fuben[3], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				end
			elseif a > 30 and a <= 65 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].xiyou, YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif a > 65 then
				if d == 1 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif d == 2 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif d == 3 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[3], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				elseif d == 4 then
					map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[4], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
				end
			end
		elseif door == 10 then
			if d == 1 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif d == 2 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif d == 3 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[3], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif d == 4 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[4], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			end
		elseif door == 110 then
			if a <= 10 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[1], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif a > 10 and a <= 30 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[2], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif a > 30 and a <= 40 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[3], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif a > 40 and a <= 60 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[4], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif a > 60 and a <= 80 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[5], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif a > 80 and a <= 90 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[6], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			elseif a > 90 then
				map.MapCreateColCreature(MapID, InstanceID, boss[k].digong[7], YSSD_pos[door].x,YSSD_pos[door].y, YSSD_pos[door].z, true, "")
			end
		end
		map.SetMapScriptData(MapID, InstanceID, 1, 4, map.GetMapScriptData(MapID, InstanceID, 1, 4)+1)
	    map.SetMapScriptData(MapID, InstanceID, 1, 0, map.GetMapScriptData(MapID, InstanceID, 1, 4))
	end
end
--d获得怪物死亡计数
function YSSD_guai_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local CreatureNum1 = map.GetMapScriptData(MapID, InstanceID, 1, 0)-1
	map.SetMapScriptData(MapID, InstanceID, 1, 0, CreatureNum1)
	local x = map.GetMapScriptData(MapID,InstanceID,1,3)
	local door = {}
	--地宫
	door[3] = {d = 332, id = 2}
	door[6] = {d = 333, id = 3}
	door[7] = {d = 334, id = 4}
	door[10] = {d = 344, id = 5}
	door[13] = {d = 335, id = 6}
	door[14] = {d = 336, id = 7}
	door[17] = {d = 337, id = 8}
	door[20] = {d = 338, id = 9}
	door[21] = {d = 339, id = 10}
	--天宫
	door[103] = {d = 322, id = 2}
	door[106] = {d = 323, id = 3}
	door[107] = {d = 324, id = 4}
	door[110] = {d = 325, id = 5}
	door[113] = {d = 326, id = 6}
	door[114] = {d = 327, id = 7}
	door[117] = {d = 328, id = 8}
	door[118] = {d = 329, id = 9}
	door[119] = {d = 330, id = 10}
	
	
	
	if CreatureNum1 <= 0 then
		map.SetMapScriptData(MapID, InstanceID, 1, 3, x+1)
		
		local m = map.GetMapScriptData(MapID, InstanceID, 1, 5)
		if m == 1 then
			x = x+100
			if door[x] ~= nil then
				map.OpenCloseDoor(MapID, InstanceID, door[x].d, 1)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 206539)--下一个房间的门已经开启，怪物已经出现，请开始挑战
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				map.SetMapScriptData(MapID, InstanceID, 1, 1, door[x].id)
			end
			YSSDD_CreatCreature(MapID, InstanceID)
		elseif m == 2 then
			if door[x] ~= nil then
				map.OpenCloseDoor(MapID, InstanceID, door[x].d, 1)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 13, 0)
				msg.AddMsgEvent(MsgID, 1, 206539)--下一个房间的门已经开启，怪物已经出现，请开始挑战
				msg.DispatchRoleMsgEvent(RoleID, MsgID)

				map.SetMapScriptData(MapID, InstanceID, 1, 1, door[x].id)
			end
			YSSDD_CreatCreature(MapID, InstanceID)
		end
	end
end
