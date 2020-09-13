--魔树，每隔两分钟和树对话一次，种子、幼苗、成树、开花、结果，5种，最后结果时候刷出树魔

function zhongshu_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false
	local MsgID = msg.BeginMsgEvent()
	--如果不在s06场景，则不能使用
	if MapID ~= 3424072488 then
	    bRet = 43
	else
		local m = map.GetMapScriptData(MapID,InstanceID,1,1)
		if m == nil then
			m = 0
		end
		if m > 150 then
			msg.AddMsgEvent(MsgID, 13, 0)
			msg.AddMsgEvent(MsgID, 1, 405024)--魔树的数量已达到上限，过一段时间在来种植
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			bRet = 32
		end

	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4010209, 0, "zhongshu_CanUse")

function zhongshu_ONUse(MapID, InstanceID, TypeID, RoleID)
	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	local MsgID = msg.BeginMsgEvent()
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	local creatureID = map.MapCreateCreature(MapID, InstanceID, 5511019, x, y, z)--魔树幼苗
	cre.SetCreatureScriptData(MapID, InstanceID, creatureID, 1, 1, RoleID)--记录种植
	map.SetMapScriptData(MapID,InstanceID,1,1,m+1)
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 405010)--魔树幼苗已经长出，和它对话就知道何时照顾它了
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterItemEvent(4010209, 1, "zhongshu_ONUse")




function zhongshu_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,m-1)

end
aux.RegisterCreatureEvent(5511019, 13, "zhongshu_OnDie")


function zhongshu_OnIalk0(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--所属者ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	if RoleID == seedowner then
		if TalkIndex == -1 then
		    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				msg.AddMsgEvent(MsgID, 20, 405011)	-- 2分钟之后我就成年了，那时我需要你过来照顾我一下。
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "好的，我知道了。"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
		    else
				msg.AddMsgEvent(MsgID, 20, 405028)	-- 你又来看我了？我要等2分钟才能长大呢，不要太着急啊。
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405029)	-- "你现在长大了吗？"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
		    end

		elseif TalkIndex == 4 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local lTime = os.time()
				cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3,lTime)
			else
				local cTime = os.time() --当前时间
				local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
				local s = os.difftime(cTime,lTime) --距离上一次对话时间按
				if s > 120 then  --间隔大于2分钟
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID1 = map.MapCreateCreature(MapID, InstanceID, 5511020, x, y, z)--魔树成树
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID1, 1, 1, RoleID)
					map.SetMapScriptData(MapID,InstanceID,1,1,m-1)
				else
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 405012)--请耐心等待，2分钟还没有到！
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 405013)--这颗树不是你的，不能对话
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5511019, 7, "zhongshu_OnIalk0")

function zhongshu_OnIalk1(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--所属者ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				msg.AddMsgEvent(MsgID, 20, 405015)	-- 很不容易才长大啊。2分钟之后我就可以开花了，而且我只能存活三分钟，那时记得过来看我。
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "好的，我知道了。"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			 else
				msg.AddMsgEvent(MsgID, 20, 405028)	-- 你又来看我了？我要等2分钟才能长大呢，不要太着急啊。
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405029)	-- "你现在长大了吗？"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 4 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local lTime = os.time()
				cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3,lTime)
			else
				local cTime = os.time() --当前时间
				local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
				local s = os.difftime(cTime,lTime) --距离上一次对话时间按
				if s > 120 then  --间隔大于2分钟
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID2 = map.MapCreateCreature(MapID, InstanceID, 5511021, x, y, z)--魔树开花
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID2, 1, 1, RoleID)
				else
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 405012)--还没有到两分钟，不能和魔树对话
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 405013)--这颗树不是你的，不能对话
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5511020, 7, "zhongshu_OnIalk1")

function zhongshu_OnIalk2(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--所属者ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				msg.AddMsgEvent(MsgID, 20, 405016)	-- 开花儿了，多么的美丽。2分钟后就可以结果了，那时过来采摘哦。
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "好的，我知道了。"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				msg.AddMsgEvent(MsgID, 20, 405028)	-- 你又来看我了？我要等2分钟才能长大呢，不要太着急啊。
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405029)	-- "你现在长大了吗？"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end

		elseif TalkIndex == 4 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local lTime = os.time()
				cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3,lTime)
			else
				local cTime = os.time() --当前时间
				local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
				local s = os.difftime(cTime,lTime) --距离上一次对话时间按
				if s > 120 then  --间隔大于2分钟
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID3 = map.MapCreateCreature(MapID, InstanceID, 5511022, x, y, z)--魔树结果
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID3, 1, 1, RoleID)
				else
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 405012)--还没有到两分钟，不能和魔树对话
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 405013)--这颗树不是你的，不能对话
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5511021, 7, "zhongshu_OnIalk2")

function zhongshu_OnIalk3(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--所属者ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	local MsgID = msg.BeginMsgEvent()
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				msg.AddMsgEvent(MsgID, 20, 405017)	-- 你来采摘了，不过不知道是什么原因，你2分钟后再过来看看吧。还要记住我只能存活三分钟，
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "好的，我知道了。"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				msg.AddMsgEvent(MsgID, 20, 405028)	-- 你又来看我了？我要等2分钟才能长大呢，不要太着急啊。
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405029)	-- "你现在长大了吗？"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			end
		elseif TalkIndex == 4 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local lTime = os.time()
				cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3,lTime)
			else
				local cTime = os.time() --当前时间
				local lTime = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3)
				local s = os.difftime(cTime,lTime) --距离上一次对话时间按
				if s > 120 then  --间隔大于2分钟--60153-60157--60158-60162--60163-60167
						local a = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60153) 
						local b = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60158) 
						local c = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60163) 
						local d = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60154) 
						local e = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60159) 
						local f = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60164) 
						local g = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60155) 
						local h = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60160) 
						local i = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60165) 
						local j = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60156) 
						local k = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60161) 
						local l = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60166) 
						local m = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60157) 
						local n = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60162) 
						local o = role.IsRoleHaveBoardQuest(MapID, InstanceID, RoleID, 60167) 
						if a == true or b == true or c == true then
							map.MapCreateCreature(MapID, InstanceID, 1093001, x, y, z)--树魔1093001在第五次与魔树对话后刷出。51级普通怪物
						end
						if d == true or e == true or f == true then
							map.MapCreateCreature(MapID, InstanceID, 1093002, x, y, z)--树魔1093002	在第五次与魔树对话后刷出。54级普通怪物
						end
						if g == true or h == true or i == true then
							map.MapCreateCreature(MapID, InstanceID, 1093003, x, y, z)--树魔1093003	在第五次与魔树对话后刷出。57级普通怪物
						end
						if j == true or k == true or l == true then
							map.MapCreateCreature(MapID, InstanceID, 1093004, x, y, z)--树魔1093004	在第五次与魔树对话后刷出。60级普通怪物
						end
						if m == true or n == true or o == true then
							map.MapCreateCreature(MapID, InstanceID, 1093005, x, y, z)--树魔1093005	在第五次与魔树对话后刷出。63级普通怪物
						end
						map.MapDeleteCreature(MapID, InstanceID, TargetID)
				else
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 405012)--还没有到两分钟，不能和魔树对话
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 405013)--这颗树不是你的，不能对话
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(5511022, 7, "zhongshu_OnIalk3")
