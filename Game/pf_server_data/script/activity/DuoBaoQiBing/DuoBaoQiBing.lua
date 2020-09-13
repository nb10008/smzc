----------------------------------------------------------------------------------------
-------------------------------- 夺宝奇兵活动 ------------------------------------------
----------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------
-- 一些常量
--------------------------------------------------------------------------------------
BaoXiang_ID						= 4500028						-- 宝箱物品ID
DuoBaoQiBing_Active_ID	= 27									-- 夺宝奇兵活动ID
ERS_HoldChest					= 16777216						-- 夺宝奇兵活动携宝模式0x1000000
HoldChest_BuffID				= 9003301						-- 夺宝奇兵活动携宝模式0x1000000
ERS_SafeArea						=	16								-- 0x0010,	// 安全区

-- 在C++注册的脚本事件
ESRE_OnChaseHolderDie				= 44							-- 夺宝奇兵活动携带宝箱者被杀死时触发事件
ESRE_ForceLootChase						= 45							-- 宝箱强制掉落
ESIE_CanUse									=	0							-- 物品是否可用
ESIE_CanDelGroundItem					=	2							-- 掉在地上的物品是否可以删除
ESIE_PutDownItem							=	3							-- 丢弃物品
ESIE_CanPickUpItem						=	4							-- 判断是否可以捡起物品
ESIE_PickUpItem								=	5							-- 捡起物品时触发
ESIE_CanPutDown							= 6							-- 判断物品是否可以丢弃
ESIE_CanDelItemOnRoleInit			= 7							-- 角色初始化时候，检查背包中物品需要删除（0表示需要删除， 1表示不需要）
ESAE_OnTimer									=	1							-- 定时更新
ESAE_OnStart									=	2							-- 活动开始
ESAE_OnEnd									=	3							-- 活动结束
ESAE_OnTimerMin							=	4							-- 活动每分钟更新
ESAE_OnActOpenClose					= 9							-- 控制台开启或关闭活动

-- 角色行走状态枚举
EMS_Swim							= 6					-- 游泳
EMS_SwimStand					= 7					-- 游泳漂浮状态

-- 错误码
E_Loot_Swim_Can_Not_Pickup_BaoXiang = 11			-- 游泳时不能拾取宝箱
E_Loot_Role_Level_Limit = 12									-- 角色等级低于20级不能拾取宝箱
E_Loot_Role_SafeArea_Limit = 13								-- 角色在安全区内不能拾取宝箱

EWeekDay = {["Sunday"] = 1, ["Monday"] = 2, ["Tuesday"] = 3, ["Wednesday"] = 4, ["Thursday"] = 5, ["Friday"] = 6, ["Saturday"] = 7}

--------------------------------------------------------------------------------------
-- 脚本局部变量
--------------------------------------------------------------------------------------
BaoXiang_SerialID_High	= 0								-- 宝箱物品序列号（高位）
BaoXiang_SerialID_Low	= 0								-- 宝箱物品序列号（低位）
BaoXiang_GroupID_High = 0								-- 宝箱地物ID（高位）
BaoXiang_GroupID_Low = 0								-- 宝箱地物ID（低位）
BaoXiang_MapID				= 0							-- 宝箱所在场景ID
BaoXiang_X					= 0								-- 宝箱X坐标
BaoXiang_Z					= 0								-- 宝箱Z坐标
BaoXiang_Holder				= 0								-- 拿到宝箱的玩家的角色ID
Min_Counter					= 0								-- 分钟计数器
Active_Started				= 0								-- 活动是否开始
Active_End					= 0								-- 活动是否结束
BaoXiang_OpenRemainTime		= BaoXiang_CDTime				-- 宝箱可开启剩余时间（单位为秒）
Tick_Count					= 0								-- tick计时
BaoXiang_UpdateRemainTime	= BaoXiang_Update_Interval	    -- 重新刷新宝箱的倒计时（单位为秒 ）
WeekDay_OpenState = {0, 0, 0, 0, 0, 0, 0}	 -- 夺宝奇兵开放状态，从周日开始

--------------------------------------------------------------------------------------
-- 脚本局部工具函数
--------------------------------------------------------------------------------------
-- 重置活动相关变量
function ResetActiveData()
	BaoXiang_MapID				= 0								-- 宝箱所在场景ID
	BaoXiang_X					= 0								-- 宝箱X坐标
	BaoXiang_Z					= 0								-- 宝箱Z坐标
	BaoXiang_Holder				= 0							-- 拿到宝箱的玩家的角色ID
	Min_Counter					= 0								-- 分钟计数器
	BaoXiang_OpenRemainTime		= BaoXiang_CDTime				-- 宝箱可开启剩余时间（单位为秒）
	Tick_Count					= 0								-- tick计时
	BaoXiang_SerialID_High	= 0							-- 宝箱物品序列号（高位）
	BaoXiang_SerialID_Low	= 0							-- 宝箱物品序列号（低位）
	BaoXiang_GroupID_High = 0								-- 宝箱地物ID（高位）
	BaoXiang_GroupID_Low = 0								-- 宝箱地物ID（低位）
	BaoXiang_UpdateRemainTime	= BaoXiang_Update_Interval	    -- 重新刷新宝箱的倒计时（单位为秒 ）
end

-- 广播“XX玩家在XX场景XX坐标已经携带了宝物，请玩家抓紧时间去抢夺吧！”
function SendPickUpBaoXiangMsg()
	if BaoXiang_MapID == nil or BaoXiang_Holder == nil then
		return
	end
	if BaoXiang_MapID == 0 or BaoXiang_Holder == nil then
		return
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100073)
	msg.AddMsgEvent(MsgID, 2, BaoXiang_Holder)
	msg.AddMsgEvent(MsgID, 6, BaoXiang_MapID)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_X)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_Z)
	msg.DispatchWorldMsgEvent(MsgID)
end

--广播活动开始(宝箱出现在XX场景、XX坐标点，大家赶快去抢夺宝物吧)
function SendChaseUpdateMsg()
	if BaoXiang_MapID == nil then
		return
	end
	if BaoXiang_MapID == 0 then
		return
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100072)
	msg.AddMsgEvent(MsgID, 6, BaoXiang_MapID)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_X)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_Z)
	msg.DispatchWorldMsgEvent(MsgID)
end

-- 宝箱持有者被玩家杀死
function SendChaseHolderBeKilledByRole(KillerID)
	if BaoXiang_MapID == nil then
		return
	end

	if BaoXiang_MapID == 0 then
		return
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100074)
	msg.AddMsgEvent(MsgID, 2, KillerID)
	msg.AddMsgEvent(MsgID, 2, BaoXiang_Holder)
	msg.AddMsgEvent(MsgID, 6, BaoXiang_MapID)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_X)
	msg.AddMsgEvent(MsgID, 9, BaoXiang_Z)
	msg.DispatchWorldMsgEvent(MsgID)
end

-- 同步宝箱开启剩余时间
function SyncOpenChaseRemainTime()
	if BaoXiang_Holder ~= 0 then
		role.SyncOpenChaseRemainTime(BaoXiang_Holder, BaoXiang_OpenRemainTime)
	end
end

-- 更新宝箱当前坐标()
function UpdateBaoXiangPos(RoleID)
	if BaoXiang_MapID == nil then
		return
	end
	if BaoXiang_MapID == 0 then
		return
	end

	if RoleID ~= 0 then
		local BaoXiang_Y = 0
		BaoXiang_X, BaoXiang_Y, BaoXiang_Z = unit.GetPosition(BaoXiang_MapID, 0, RoleID)
	end
end

-- 去除角色身上的携宝模式状态
function UnSetHoldChestState(RoleID)
	role.UnSetRoleState(RoleID, ERS_HoldChest, 1)
	unit.CancelBuff(BaoXiang_MapID, 0, RoleID, HoldChest_BuffID)
end

-- 给角色设置夺宝奇兵活动携宝模式状态
function SetHoldChestState(RoleID)
	role.SetRoleState(RoleID, ERS_HoldChest, 1)
	unit.AddBuff(BaoXiang_MapID, 0, RoleID, HoldChest_BuffID, RoleID)
end

-- 设置拿到宝箱的玩家的角色ID
function SetBaoXiangHolderRoleID(RoleID)
	if RoleID == 0 then
		-- 去除角色身上的携宝模式状态
		UnSetHoldChestState(BaoXiang_Holder)

		-- 如果宝箱被丢弃，重置宝箱开启剩余时间
		if BaoXiang_Hold_Time_Mod == 0 then
			BaoXiang_OpenRemainTime = BaoXiang_CDTime
		end

		-- 更新宝箱当前坐标
		UpdateBaoXiangPos(BaoXiang_Holder)
	else
		-- 给角色设置夺宝奇兵活动携宝模式状态
		SetHoldChestState(RoleID)

		-- 重置刷新宝箱的倒计时（单位为秒 ）
		BaoXiang_UpdateRemainTime = BaoXiang_Update_Interval

		-- 更新宝箱当前坐标
		UpdateBaoXiangPos(RoleID)
	end

	-- 设置拿到宝箱的玩家的角色ID
	BaoXiang_Holder = RoleID

	-- 同步宝箱开启剩余时间
	SyncOpenChaseRemainTime()
end

-- 删除活动宝箱
function RemoveBaoXiang()
	if BaoXiang_Holder ~= 0 then
		role.RemoveFromRole(BaoXiang_MapID, 0, BaoXiang_Holder, BaoXiang_ID, 1, 310)
	else
		map.RemoveGroundItem(BaoXiang_MapID, BaoXiang_GroupID_High, BaoXiang_GroupID_Low)
	end
end

-- 每日活动开始
function ActiveStart()
	-- 如果前一天的活动没有结束，并且宝箱在玩家身上，删掉宝箱
	if Active_End == 0 then
		-- 删除活动宝箱
		RemoveBaoXiang()
	end

	-- 去除角色身上的携宝模式状态
	UnSetHoldChestState(BaoXiang_Holder)

	-- 重置活动相关变量
	ResetActiveData()

	-- 广播活动规则“携带宝物者不可进行使用"传送道具，下线、进入新手村、主城以及副本、下水"如果玩家进行以上操作或使用了规定中的道具，宝物将自动掉落在进行操作或使用道具前的地点，请玩家慎重！！！”
	local RuleMsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(RuleMsgID, 102, 100075)
	msg.DispatchWorldMsgEvent(RuleMsgID)

	-- 将宝箱刷出来，放到随机地图里面的随机怪物的旁边
	-- 广播活动开始(宝箱出现在XX场景、XX坐标点，大家赶快去抢夺宝物吧)
	ActiveStartAndStartBroadcast()

	-- 活动状态标记
	Active_Started = 1
	Active_End = 0
end

-- 夺宝奇兵活动结束
function ActiveEnd()
	Active_End = 1

	-- 发活动结束广播
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100083)
	msg.DispatchWorldMsgEvent(MsgID)

	-- 去除角色身上的携宝模式状态
	UnSetHoldChestState(BaoXiang_Holder)
end

-- 夺宝奇兵活动开始，刷宝箱出来并发广播
function ActiveStartAndStartBroadcast()
	--将宝箱刷出来，放到随机地图里面的随机怪物的旁边
	BaoXiang_MapID, BaoXiang_X, BaoXiang_Z, BaoXiang_SerialID_High, BaoXiang_SerialID_Low, BaoXiang_GroupID_High, BaoXiang_GroupID_Low = map.PutItemNearRandCreature(BaoXiang_ID)
	if BaoXiang_MapID == nil or BaoXiang_X == nil or BaoXiang_Z == nil or BaoXiang_SerialID_High==nil or BaoXiang_SerialID_Low == nil or BaoXiang_GroupID_High == nil or BaoXiang_GroupID_Low == nil then
		return
	end

	--广播刷新宝箱(宝箱出现在XX场景、XX坐标点，大家赶快去抢夺宝物吧)
	SendChaseUpdateMsg()
end

-- 判断夺宝奇兵活动是否开始了，并且还没有结束
function IsActiveEnd()
	return Active_End
end

-- 更新宝箱开启剩余时间
function UpdateOpenRemainTime()
	if BaoXiang_OpenRemainTime == nil then
		BaoXiang_OpenRemainTime	= BaoXiang_CDTime				-- 宝箱可开启剩余时间（单位为秒）
	end

	if IsActiveEnd() == 1 then
		return
	end

	if BaoXiang_Holder ~= 0  then
		BaoXiang_OpenRemainTime = BaoXiang_OpenRemainTime-1

		-- 如果宝箱可开启剩余时间（单位为秒）小于0，夺宝奇兵活动结束
		if BaoXiang_OpenRemainTime < 0 then
			ActiveEnd()
		end
	end
end

-- 更新重新刷新宝箱剩余时间
function BaoXiangUpdateRemainTime()
	if BaoXiang_UpdateRemainTime == nil then
		BaoXiang_UpdateRemainTime = BaoXiang_Update_Interval	    -- 重新刷新宝箱的倒计时（单位为秒 ）
	end
	if IsActiveEnd() == 1 then
		return
	end

	if BaoXiang_Holder == 0 then
		BaoXiang_UpdateRemainTime = BaoXiang_UpdateRemainTime-1

		-- 如果刷新宝箱剩余时间（单位为秒）小于0，重新刷新宝箱
		if BaoXiang_UpdateRemainTime < 0 then
			map.RemoveGroundItem(BaoXiang_MapID, BaoXiang_GroupID_High, BaoXiang_GroupID_Low)

			-- 夺宝奇兵活动开始，刷宝箱出来并发广播
			ActiveStartAndStartBroadcast()

			-- 重新刷新宝箱的倒计时（单位为秒 ）
			BaoXiang_UpdateRemainTime	= BaoXiang_Update_Interval

		end
	end
end

-- 检查是今天是否有夺宝奇兵活动
function IsTodayHaveActive()
	return WeekDay_OpenState[EWeekDay[os.date("%A")]]
end

--------------------------------------------------------------------------------------
-- 脚本在C++程序里面注册的事件函数
--------------------------------------------------------------------------------------

-- 夺宝奇兵活动开始
function DuoBaoQiBing_OnActiveStart(ActiveID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	WeekDay_OpenState[1], WeekDay_OpenState[2], WeekDay_OpenState[3], WeekDay_OpenState[4], WeekDay_OpenState[5], WeekDay_OpenState[6], WeekDay_OpenState[7] = act.GetDuoBaoQiBingOpenState()

	-- 重置活动相关变量
	ResetActiveData()

	Active_Started				= 0								-- 活动是否开始
	Active_End					= 1								-- 活动是否结束
end

-- 每tick更新
function DuoBaoQiBing_OnActiveTimer(ActiveID, Sec)

	if DuoBaoQiBing_trigger == 0 then
		return
	end

	local Hour = tonumber(os.date("%H"))
	local Min  = tonumber(os.date("%M"))
	local Sec   = tonumber(os.date("%S"))

	-- tick计时（计算宝箱CD时间，单位为秒）
	Tick_Count = Tick_Count+1
	if Tick_Count == 6 then

		-- 更新宝箱开启剩余时间
		UpdateOpenRemainTime()

		-- 更新重新刷新宝箱剩余时间
		BaoXiangUpdateRemainTime()

		Tick_Count = 0
	end

	-- 每天晚上八点活动开始，七点五十九分的时候重置Active_Started
	if IsTodayHaveActive() == 1 then
		if Hour == 19 and Min == 59 and Active_Started == 1 then
			Active_Started = 0
		end

		if Hour == 20 and Min == 0 and Active_Started == 0 then
			ActiveStart()
		end
	end
end

-- 每分钟更新
function DuoBaoQiBing_OnActiveTimeMin(ActiveID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	if IsActiveEnd() == 1 then
		return
	end

	-- 如果宝箱还没有被玩家捡起
	if BaoXiang_Holder ~= 0 then
		-- 更新宝箱当前坐标
		UpdateBaoXiangPos(BaoXiang_Holder)

		-- 同步宝箱开启剩余时间
		SyncOpenChaseRemainTime()
	end

	Min_Counter = Min_Counter+1
	if Min_Counter > BaoXiang_Pos_Broadcast_Interval then

		-- 如果宝箱还没有被玩家捡起
		if BaoXiang_Holder ~= 0 then
			-- 广播“XX玩家在XX场景XX坐标已经携带了宝物，请玩家抓紧时间去抢夺吧！”
			SendPickUpBaoXiangMsg()
		else
			-- 广播(宝箱出现在XX场景、XX坐标点，大家赶快去抢夺宝物吧)
			SendChaseUpdateMsg()
		end

		Min_Counter = 0
	end

end

-- 活动结束
function DuoBaoQiBing_OnActiveEnd(ActiveID)

end

--玩家捡起宝箱，发广播
function DuoBaoQiBing_OnPickUpBaoXiang(ItemID, RoleID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	if IsActiveEnd() == 1 then
		return
	end

	if ItemID~=BaoXiang_ID then
		return
	end

	-- 设置拿到宝箱的玩家的角色ID
	SetBaoXiangHolderRoleID(RoleID)

	BaoXiang_GroupID_High = 0
	BaoXiang_GroupID_Low = 0

	-- 广播“XX玩家在XX场景XX坐标已经携带了宝物，请玩家抓紧时间去抢夺吧！”
	SendPickUpBaoXiangMsg()

end

--判断玩家是否能捡起宝箱
function DuoBaoQiBing_CanPickUpBaoXiang(ItemID, RoleID)
	if DuoBaoQiBing_trigger == 0 then
		return 0
	end

	-- 检查等级
	local RoleLvl = role.GetRoleLevel(BaoXiang_MapID, 0, RoleID)
	if RoleLvl == nil then
		return 0		--E_Success
	end
	if BaoXiang_ID == ItemID and  (RoleLvl < Active_Min_Level or RoleLvl > Active_Max_Level) then
		return E_Loot_Role_Level_Limit		-- 等级不满足要求
	end

	-- 检查角色游泳状态
	local CurMoveState = role.GetCurMoveState(RoleID)
	if CurMoveState == EMS_Swim	or CurMoveState == EMS_SwimStand then
		return E_Loot_Swim_Can_Not_Pickup_BaoXiang
	end

	-- 检查角色是不是身处安全区
	if role.IsRoleInStatus(RoleID, ERS_SafeArea) then
		return E_Loot_Role_SafeArea_Limit
	end

	return 0			--E_Success
end

-- 持有宝箱的玩家被杀死
function DuoBaoQiBing_OnChaseHolderDie(RoleID, KillerID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	if IsActiveEnd() == 1 then
		return
	end

	if BaoXiang_Holder ~= RoleID then
		return
	end

	-- 更新宝箱当前坐标
	UpdateBaoXiangPos(BaoXiang_Holder)

	if unit.IsPlayer(KillerID) then
		-- 发广播“<Msg id="100074" type="1" channel="5" value="玩家[&lt;p1&gt;]已经把携带宝物的玩家[&lt;p2&gt;]杀死，宝箱掉落在&lt;p3&gt;(&lt;p4&gt;,&lt;p5&gt;)坐标，玩家赶快去抢夺吧！"/>”
		SendChaseHolderBeKilledByRole(KillerID)
	else
		--广播刷新宝箱(宝箱出现在XX场景、XX坐标点，大家赶快去抢夺宝物吧)
		SendChaseUpdateMsg()
	end

	-- 将玩家身上的宝箱丢弃到地上
	role.PutDownItem(RoleID, BaoXiang_ID)

	-- 重置宝箱携带者ID
	SetBaoXiangHolderRoleID(0)

end

-- 宝箱是否可以删除(0表示可以删除， 1表示不可以)
function DuoBaoQiBing_CanDelBaoXiangOnGround(TypeID, SerialID_High, SerialID_Low)
	if DuoBaoQiBing_trigger == 0 then
		return 0
	end

	if IsActiveEnd() == 1 then
		return 0
	end

	if TypeID == BaoXiang_ID and BaoXiang_SerialID_High == SerialID_High and BaoXiang_SerialID_Low == SerialID_Low then
		return 1
	else
		return 0
	end
end

-- 宝箱强制掉落
function DuoBaoQiBing_ForceLootChase(RoleID)
	if DuoBaoQiBing_trigger == 0 then
		return
	end

	if IsActiveEnd() == 1 then
		return
	end

	if BaoXiang_Holder == RoleID then
		-- 更新宝箱当前坐标
		UpdateBaoXiangPos(BaoXiang_Holder)

		-- 将玩家身上的宝箱丢弃到地上
		role.PutDownItem(RoleID, BaoXiang_ID)

		-- 通知玩家宝箱掉落
		role.ForceLootChase(BaoXiang_Holder)

		-- 重置宝箱携带者ID
		SetBaoXiangHolderRoleID(0)

		--广播刷新宝箱(宝箱出现在XX场景、XX坐标点，大家赶快去抢夺宝物吧)
		SendChaseUpdateMsg()
	end
end


--[[-- 服了，宝箱不能丢弃了
function DuoBaoQiBing_PutDownItem(TypeID)
	if IsActiveEnd() == 1 then
		return
	end

	-- 重置宝箱携带者ID
	SetBaoXiangHolderRoleID(0)

	--广播刷新宝箱(宝箱出现在XX场景、XX坐标点，大家赶快去抢夺宝物吧)
	SendChaseUpdateMsg()
end
--]]

-- 服了，宝箱不能丢弃了(0表示可以， 1表示不可以)
function DuoBaoQiBing_CanPutDownBaoXiang(TypeID, SerialID_High, SerialID_Low)
	if DuoBaoQiBing_trigger == 0 then
		return 0
	end

	if IsActiveEnd() == 1 then
		return 0
	end

	if TypeID == BaoXiang_ID and BaoXiang_SerialID_High == SerialID_High and BaoXiang_SerialID_Low == SerialID_Low then
		return 1
	else
		return 0
	end
end

-- 宝箱是否能开启
function DuoBaoQiBing_CanOpenBaoXiang(MapID, InstanceID, TypeID, TargetID)
	if DuoBaoQiBing_trigger == 0 then
		return 0, 0
	end

	if IsActiveEnd() == 1 then
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 8) then
			--提示玩家背包空间不足
			return 40, false
		end
		return 0, 0
	end

	-- E_UseItem_TreasureInTime		=	60,		// 夺宝奇兵，尚未到达活动结束时间
	return 60, 1
end

--角色初始化时候，检查背包中物品需要删除（0表示需要删除， 1表示不需要）
function DuoBaoQiBing_CanDeleteBaoXiang(TypeID, SerialID_High, SerialID_Low)
	if TypeID == BaoXiang_ID  then
		if BaoXiang_SerialID_High == SerialID_High and BaoXiang_SerialID_Low == SerialID_Low and IsActiveEnd() == 1 then
			return 1
		else
			return 0
		end
	else
		return 1
	end
end

--活动控制台开关
function DuoBaoQiBing_OnActOpenClose(ActiveID, IsOpen)
	if DuoBaoQiBing_Active_ID ~= ActiveID then
		return
	end

	if DuoBaoQiBing_trigger == 0 and IsActiveEnd() ~= 1 then
		-- 删除活动宝箱
		RemoveBaoXiang()

		-- 夺宝奇兵活动结束
		ActiveEnd()
	end

	DuoBaoQiBing_trigger = IsOpen
end

-- 向活动注册初始化和适时更新操作
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnTimer, "DuoBaoQiBing_OnActiveTimer")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnStart, "DuoBaoQiBing_OnActiveStart")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnEnd, "DuoBaoQiBing_OnActiveEnd")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnTimerMin, "DuoBaoQiBing_OnActiveTimeMin")
aux.RegisterActEvent(DuoBaoQiBing_Active_ID, ESAE_OnActOpenClose, "DuoBaoQiBing_OnActOpenClose")

aux.RegisterRoleEvent(ESRE_OnChaseHolderDie,	"DuoBaoQiBing_OnChaseHolderDie")
aux.RegisterRoleEvent(ESRE_ForceLootChase,		"DuoBaoQiBing_ForceLootChase")

aux.RegisterItemEvent(BaoXiang_ID, ESIE_CanPickUpItem,					"DuoBaoQiBing_CanPickUpBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID, ESIE_PickUpItem,							"DuoBaoQiBing_OnPickUpBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID, ESIE_CanDelGroundItem,				"DuoBaoQiBing_CanDelBaoXiangOnGround")
--aux.RegisterItemEvent(BaoXiang_ID, ESIE_PutDownItem,					"DuoBaoQiBing_PutDownItem")
aux.RegisterItemEvent(BaoXiang_ID, ESIE_CanPutDown,						"DuoBaoQiBing_CanPutDownBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID, ESIE_CanUse,								"DuoBaoQiBing_CanOpenBaoXiang")
aux.RegisterItemEvent(BaoXiang_ID, ESIE_CanDelItemOnRoleInit,		"DuoBaoQiBing_CanDeleteBaoXiang")

