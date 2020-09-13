--摇钱树，每隔两分钟和树对话一次，种子、幼苗、成树、开花、结果，5种，最后结果时候刷出财宝精灵
function i4830012_CanUse(MapID, InstanceID, TypeID, RoleID)
	local bRet, bIgnore = 0, false

	--如果不在s03场景，则不能使用
	if MapID ~= 3424073256 then
	    bRet = 43--不可在该位置使用该物品
	else
		local m = map.GetMapScriptData(MapID,InstanceID,1,1)
		if m == nil then
			m = 0
		end
		if m > 150 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 444401)--摇钱树的数量已达到上限，过一段时间在来种植
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			bRet = 102
		end
	end
	--返回
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830012, 0, "i4830012_CanUse")

function i4830012_OnUse(MapID, InstanceID, TypeID, RoleID)
	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, RoleID)
	local creatureID = map.MapCreateCreature(MapID, InstanceID, 6040017, x, y, z)--摇钱树幼苗
	cre.SetCreatureScriptData(MapID, InstanceID, creatureID, 1, 1, RoleID)--记录种植
	map.SetMapScriptData(MapID,InstanceID,1,1,m+1)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 1, 444402)--摇钱树幼苗已经长出，和它对话就知道何时照顾它了
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterItemEvent(4830012, 1, "i4830012_OnUse")




function n6040017_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	map.SetMapScriptData(MapID,InstanceID,1,1,m-1)

end
aux.RegisterCreatureEvent(6040017, 13, "n6040017_OnDie")


function n6040017_OnIalk0(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local m = map.GetMapScriptData(MapID,InstanceID,1,1)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--所属者ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if RoleID == seedowner then
		if TalkIndex == -1 then
		    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444457)	-- 2分钟之后我就成年了，那时我需要你过来照顾我一下。
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "好的，我知道了。"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
		    else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444462)	-- 你又来看我了？我要等2分钟才能长大呢，不要太着急啊。
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
				if s > 60 then  --间隔大于1分钟
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID1 = map.MapCreateCreature(MapID, InstanceID, 6040026, x, y, z)--摇钱树成树
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID1, 1, 1, RoleID)
					map.SetMapScriptData(MapID,InstanceID,1,1,m-1)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 13, 0)
					msg.AddMsgEvent(MsgID, 1, 444458)--请耐心等待，2分钟还没有到！
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 1, 405013)--这颗树不是你的，不能对话
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(6040017, 7, "n6040017_OnIalk0")

function n6040026_OnIalk1(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--所属者ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444459)	-- 很不容易才长大啊。2分钟之后我就可以开花了，而且我只能存活三分钟，那时记得过来看我。
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "好的，我知道了。"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444462)	-- 你又来看我了？我要等2分钟才能长大呢，不要太着急啊。
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
				if s > 60 then  --间隔大于1分钟
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID2 = map.MapCreateCreature(MapID, InstanceID, 6040027, x, y, z)--摇钱树开花
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID2, 1, 1, RoleID)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 1, 444458)--还没有到两分钟，不能和摇钱树对话
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 13, 0)
		msg.AddMsgEvent(MsgID, 1, 405013)--这颗树不是你的，不能对话
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end
end
aux.RegisterCreatureEvent(6040026, 7, "n6040026_OnIalk1")

function n6040027_OnIalk2(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--所属者ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444460)	-- 开花儿了，多么的美丽。2分钟后就可以结果了，那时过来采摘哦。
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "好的，我知道了。"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444462)	-- 你又来看我了？我要等2分钟才能长大呢，不要太着急啊。
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
				if s > 60 then  --间隔大于1分钟
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
					local creatureID3 = map.MapCreateCreature(MapID, InstanceID, 6040028, x, y, z)--摇钱树结果
					cre.SetCreatureScriptData(MapID, InstanceID, creatureID3, 1, 1, RoleID)
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 1, 444458)--还没有到两分钟，不能和摇钱树对话
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 1, 405013)--这颗树不是你的，不能对话
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(6040027, 7, "n6040027_OnIalk2")

function n6040028_OnIalk3(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	local seedowner = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1)--所属者ID
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	if RoleID == seedowner then
		if TalkIndex == -1 then
			if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 3) == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444461)	-- 你来采摘了，不过不知道是什么原因，你2分钟后再过来看看吧。还要记住我只能存活三分钟，
				msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 405014)	-- "好的，我知道了。"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444462)	-- 你又来看我了？我要等2分钟才能长大呢，不要太着急啊。
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
				if s > 60 then  --间隔大于1分钟--60153-60157--60158-60162--60163-60167
					for i=0,3 do
						if role.IsRoleHaveQuest(MapID, InstanceID, RoleID,30863+i) == true then
							map.MapCreateCreature(MapID, InstanceID,3040339+i,x, y, z)
							map.MapDeleteCreature(MapID, InstanceID, TargetID)
							break
						end
					end
				else
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 1, 444458)--还没有到两分钟，不能和摇钱树对话
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				end
			end
		end
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 1, 405013)--这颗树不是你的，不能对话
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	end

end
aux.RegisterCreatureEvent(6040028, 7, "n6040028_OnIalk3")
