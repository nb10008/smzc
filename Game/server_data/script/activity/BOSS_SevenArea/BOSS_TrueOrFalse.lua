 --记录活动scriptdata，记录下一次刷出最终BOSS至少还有多少天。

--emozhizi_alive = 0
--RoleDataType["zhenjiaVIP"]记录真假魔王完成日期
function BOSS_TrueOrFalse_OnStart(actID)

	if act.GetActScriptData(28, 1, 1) > 0 then
	    act.SetActScriptData(28, 1, 1, act.GetActScriptData(28, 1, 1)-1)
	end

	if act.GetActScriptData(28, 1, 2) > 0 then
	    act.SetActScriptData(28, 1, 2, act.GetActScriptData(28, 1, 2)-1)
	end

	if act.GetActScriptData(28, 1, 2) > 0 then
	    act.SetActScriptData(28, 1, 2, act.GetActScriptData(28, 1, 2)-1)
	end

	act.SaveActScriptData(28)

	--重置TimerMin
	BOSS_TrueOrFalse_TimerMin = 0

    B_T_CreaturePoint = {}
	B_T_CreaturePoint[1] = {{x=249,y=1067,z=883},{x=252,y=1067,z=810},{x=425,y=1067,z=820},{x=432,y=1067,z=887},{x=337,y=1067,z=801}}
    B_T_CreaturePoint[2] = {{x=648,y=1821,z=866},{x=710,y=1208,z=717},{x=591,y=1208,z=707},{x=650,y=1208,z=714},{x=646,y=1208,z=827}}
	B_T_CreaturePoint[3] = {{x=856,y=124,z=896},{x=856,y=124,z=842},{x=857,y=124,z=633},{x=857,y=124,z=737},{x=856,y=124,z=684}}
	B_T_CreaturePoint[4] = {{x=198,y=1013,z=610},{x=192,y=1013,z=503},{x=100,y=1013,z=505},{x=97,y=1013,z=615},{x=141,y=1013,z=605}}
	B_T_CreaturePoint[5] = {{x=451,y=1123,z=652},{x=487,y=1123,z=567},{x=487,y=1123,z=495},{x=415,y=1123,z=514},{x=420,y=1123,z=586}}
	B_T_CreaturePoint[6] = {{x=232,y=1212,z=234},{x=236,y=1212,z=136},{x=357,y=1213,z=153},{x=359,y=1213,z=223},{x=237,y=1214,z=188}}
	B_T_CreaturePoint[7] = {{x=723,y=851,z=242},{x=741,y=849,z=181},{x=715,y=851,z=107},{x=873,y=851,z=136},{x=884,y=851,z=226}}

	B_T_Entrance = {}
	B_T_Entrance[1] = {{x=339,y=41,z=972},{x=321,y=21,z=744},{x=470,y=21,z=783}}
    B_T_Entrance[2] = {{x=732,y=43,z=924},{x=563,y=43,z=917},{x=734,y=36,z=868}}
	B_T_Entrance[3] = {{x=856,y=13,z=971},{x=899,y=10,z=955},{x=813,y=10,z=954}}
	B_T_Entrance[4] = {{x=70,y=20,z=558},{x=219,y=20,z=560},{x=146,y=20,z=489}}
	B_T_Entrance[5] = {{x=508,y=22,z=664},{x=393,y=22,z=660},{x=388,y=22,z=471}}
	B_T_Entrance[6] = {{x=303,y=24,z=128},{x=270,y=24,z=242},{x=198,y=24,z=132}}
	B_T_Entrance[7] = {{x=652,y=21,z=300},{x=650,y=21,z=71},{x=966,y=21,z=304}}

	B_T_BOSSPoint = {}
	B_T_BOSSPoint[1] = {x=339,y=1067,z=887}
    B_T_BOSSPoint[2] = {x=648,y=1208,z=771}
	B_T_BOSSPoint[3] = {x=856,y=124,z=790}
	B_T_BOSSPoint[4] = {x=148,y=1013,z=566}
	B_T_BOSSPoint[5] = {x=448,y=1123,z=547}
	B_T_BOSSPoint[6] = {x=386,y=1213,z=193}
	B_T_BOSSPoint[7] = {x=803,y=966,z=184}


	--[[B_T_NPCPoint = {}                --记录传送出去的NPC的坐标
	B_T_NPCPoint[1] = {x=339,y=1147,z=898}
    B_T_NPCPoint[2] = {x=212,y=1154,z=524}
	B_T_NPCPoint[3] = {x=500,y=1148,z=541}
	B_T_NPCPoint[4] = {x=179,y=802,z=214}
	B_T_NPCPoint[5] = {x=617,y=601,z=178}
	B_T_NPCPoint[6] = {x=812,y=123,z=540}
	B_T_NPCPoint[7] = {x=777,y=519,z=878}]]

	B_T_Trigger = {}
	B_T_Trigger[1487] = {4,6}
	B_T_Trigger[1488] = {3,7}
	B_T_Trigger[1489] = {1,6}
	B_T_Trigger[1490] = {2,3}
	B_T_Trigger[1491] = {1,2}
	B_T_Trigger[1649] = {5,7}
	B_T_Trigger[1492] = {4,5}

	B_T_BOSSDie = {}             --记录BOSS的状态：0为未刷新，1为已经刷新，2为已经击杀
	B_T_BOSSDie[1] = {0,0,0,0,0,0}
	B_T_BOSSDie[2] = {0,0,0,0,0,0}
	B_T_BOSSDie[3] = {0,0,0,0,0,0}

	B_T_Door = 0

	B_T_YaoJing = {0,0,0}

	B_T_ZhuangBei = {}

	map.CreateInstance(2766543506, 1)
	map.CreateInstance(2766543250, 2)
	map.CreateInstance(2766542994, 3)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100118)
	msg.DispatchBroadcast(MsgID, -1, -1, -1)

	--if emozhizi_alive == 0 then
		--map.MapCreateColCreature(3424073512, -1, 6010067, 955, 23692, 727, 1)  	--刷出恶魔之子
	   -- emozhizi_alive = 1
	--end
    map.MapCreateColCreature(2766543506, 1, 6010070, 1, 1, 1, 1)

end


function BOSS_TrueOrFalse_OnTimerMin(actID)
    if BOSS_TrueOrFalse_TimerMin ~= nil then
		BOSS_TrueOrFalse_TimerMin = BOSS_TrueOrFalse_TimerMin + 1

		if BOSS_TrueOrFalse_TimerMin <= 54 then
			local k1 = math.random(7)
			local k2 = math.random(5)
			if BOSS_TrueOrFalse_TimerMin % 18 == 0 then
				map.MapCreateColCreature(2766543506, 1, 2055003, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766543250, 2, 2055030, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766542994, 3, 2055057, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
			elseif BOSS_TrueOrFalse_TimerMin % 15 == 0 then
				map.MapCreateColCreature(2766543506, 1, 2055006, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766543250, 2, 2055033, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766542994, 3, 2055060, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
			elseif BOSS_TrueOrFalse_TimerMin % 12 == 0 then
				map.MapCreateColCreature(2766543506, 1, 2055009, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766543250, 2, 2055036, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766542994, 3, 2055063, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
			elseif BOSS_TrueOrFalse_TimerMin % 9 == 0 then
				map.MapCreateColCreature(2766543506, 1, 2055012, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766543250, 2, 2055039, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766542994, 3, 2055066, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
			elseif BOSS_TrueOrFalse_TimerMin % 6 == 0 then
				map.MapCreateColCreature(2766543506, 1, 2055015, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766543250, 2, 2055042, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766542994, 3, 2055069, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
			elseif BOSS_TrueOrFalse_TimerMin % 3 == 0 then
				map.MapCreateColCreature(2766543506, 1, 2055018, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766543250, 2, 2055045, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
				map.MapCreateColCreature(2766542994, 3, 2055072, B_T_CreaturePoint[k1][k2].x, B_T_CreaturePoint[k1][k2].y, B_T_CreaturePoint[k1][k2].z, 1)
			end
		end
		if BOSS_TrueOrFalse_TimerMin == 90 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101,	700106)
			msg.DispatchMapMsgEvent(MsgID, 2766543506, 1)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101,	700106)
			msg.DispatchMapMsgEvent(MsgID, 2766543250, 2)

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101,	700106)
			msg.DispatchMapMsgEvent(MsgID, 2766542994, 3)

        end
	end
end

-- 活动结束
function BOSS_TrueOrFalse_OnEnd(actID)

	B_T_CreaturePoint = {}
	B_T_Entrance = {}
	B_T_BOSSPoint = {}
	B_T_Trigger = {}
	B_T_BOSSDie = {}
	B_T_RoleMap = {}
	B_T_YaoJing = {}
	B_T_ZhuangBei = {}
	B_T_Door = nil
	BOSS_TrueOrFalse_TimerMin = nil
	map.DeleteInstance(2766543506, 1)
	map.DeleteInstance(2766543250, 2)
	map.DeleteInstance(2766542994, 3)

end

--注册该活动的相关事件
aux.RegisterActEvent(28, 2, "BOSS_TrueOrFalse_OnStart")
aux.RegisterActEvent(28, 4, "BOSS_TrueOrFalse_OnTimerMin")
aux.RegisterActEvent(28, 3, "BOSS_TrueOrFalse_OnEnd")

--传送进副本

function x6010067_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --获取玩家等级
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
    if TalkIndex == -1 then
	    if BOSS_TrueOrFalse_TimerMin ~= nil then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206101)		--“恶魔之子：\n     混沌魔域的大门已经开启，无数的宝物在里面等着你们！虚伪的人类啊，快带着你们的贪婪和狡诈来魔域探险吧！ \n    活动中击杀小怪有一定几率刷新出6个恶魔首领中的一个。如果在某次活动中6个恶魔首领全部出现并被杀死，则将刷新活动的终极BOSS。\n    只有55级以上的玩家才可以参加此活动,30分钟后将不能通过恶魔之子进入混沌魔域。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			if rolelevel >= 55 then
				msg.AddMsgEvent(MsgID, 21, 4)			--确定按钮
				msg.AddMsgEvent(MsgID, 1, 206102)		--“进入混沌魔域一层（55级以上区域）”
			end
			if rolelevel >= 70 then
				msg.AddMsgEvent(MsgID, 21, 5)			--确定按钮
				msg.AddMsgEvent(MsgID, 1, 206103)		--“进入混沌魔域二层（70级以上区域）”
			end
			if rolelevel >= 85 then
				msg.AddMsgEvent(MsgID, 21, 6)			--确定按钮
				msg.AddMsgEvent(MsgID, 1, 206104)		--“进入混沌魔域三层（85级以上区域）”
			end
			msg.AddMsgEvent(MsgID, 21,	10)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20,	206105)		--“恶魔之子：\n     混沌魔域的大门已经关闭，你等到下次大门开启的时候再来吧。\n    只有55级以上的玩家才可以参加此活动,30分钟后将不能通过恶魔之子进入混沌魔域。”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.AddMsgEvent(MsgID, 21,	10)			--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex > 3 and TalkIndex < 7 then
		if BOSS_TrueOrFalse_TimerMin ~= nil then --现在可以再任何时间进入混沌魔域
		    local k1 = math.random(7)
			local k2 = math.random(3)
			if TalkIndex ==  4 then
				if k1 == 4 then
					unit.AddBuff(MapID, InstanceID, RoleID, 5935401, RoleID) --火
				elseif k1 == 5 then
				    unit.AddBuff(MapID, InstanceID, RoleID, 5935501, RoleID) --冰
				end
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2766543506, B_T_Entrance[k1][k2].x, B_T_Entrance[k1][k2].y, B_T_Entrance[k1][k2].z)
			elseif TalkIndex ==  5 then
				if k1 == 4 then
					unit.AddBuff(MapID, InstanceID, RoleID, 5935401, RoleID) --火
				elseif k1 == 5 then
				    unit.AddBuff(MapID, InstanceID, RoleID, 5935501, RoleID) --冰
				end
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2766543250, B_T_Entrance[k1][k2].x, B_T_Entrance[k1][k2].y, B_T_Entrance[k1][k2].z)
		    elseif TalkIndex ==  6 then
				if k1 == 4 then
					unit.AddBuff(MapID, InstanceID, RoleID, 5935401, RoleID) --火
				elseif k1 == 5 then
				    unit.AddBuff(MapID, InstanceID, RoleID, 5935501, RoleID) --冰
				end
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, 2766542994, B_T_Entrance[k1][k2].x, B_T_Entrance[k1][k2].y, B_T_Entrance[k1][k2].z)
			end
		end
	end
end
--注册恶魔之子对话
aux.RegisterCreatureEvent(6010067, 7, "x6010067_OnTalk")

function x6010069_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	206106)		--“妖精哈苏：\n     你想离开这里回到斗战天城吗？我可以帮你。\n    只有55级以上的玩家才可以参加此活动,30分钟后将不能通过恶魔之子进入混沌魔域。”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	4)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	206107)		--“离开混沌魔域”
		msg.AddMsgEvent(MsgID, 21,	10)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
	    local k = math.random(3)
		local point = {}
		point[1] = {x=954,y=458,z=537}
		point[2] = {x=330,y=458,z=527}
		point[3] = {x=582,y=459,z=298}
	    role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424073512, point[k].x, point[k].y, point[k].z)
	end
end
--注册
aux.RegisterCreatureEvent(6010069, 7, "x6010069_OnTalk")


--杀掉特殊小怪后的BOSS刷新判断
function BOSS_TrueOrFalse_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    if TargetTypeID <= 2055018 then
		local k = TargetTypeID - 2055000
		k = k / 3
		if B_T_BOSSDie[InstanceID][k] == 0 then --and (act.GetActScriptData(28, 1, 1) == 0 or k ~= act.GetActScriptData(28, 1, 1) + 1) then
	        if math.random(100) <= 100 then
                local Index = math.random(7)
				map.MapCreateColCreature(MapID, InstanceID, 2055018 + k, B_T_BOSSPoint[Index].x, B_T_BOSSPoint[Index].y, B_T_BOSSPoint[Index].z, 1)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101,	206107+k)         --6个BOSS的提示不同
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
				B_T_BOSSDie[InstanceID][k] = 1
            end
		end
    elseif TargetTypeID <= 2055045 then
	    local k = TargetTypeID - 2055027
		k = k / 3
		if B_T_BOSSDie[InstanceID][k] == 0 then --and (act.GetActScriptData(28, 1, 2) == 0 or k ~= act.GetActScriptData(28, 1, 2) + 1) then
	        if math.random(100) <= 100 then
                local Index = math.random(7)
				map.MapCreateColCreature(MapID, InstanceID, 2055045 + k, B_T_BOSSPoint[Index].x, B_T_BOSSPoint[Index].y, B_T_BOSSPoint[Index].z, 1)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101,	206107+k)         --6个BOSS的提示不同
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
				B_T_BOSSDie[InstanceID][k] = 1
            end
		end
	elseif TargetTypeID <= 2055072 then
	    local k = TargetTypeID - 2055054
		k = k / 3
		if B_T_BOSSDie[InstanceID][k] == 0 then --and (act.GetActScriptData(28, 1, 3) == 0 or k ~= act.GetActScriptData(28, 1, 3) + 1) then
	        if math.random(100) <= 100 then
                local Index = math.random(7)
				map.MapCreateColCreature(MapID, InstanceID, 2055072 + k, B_T_BOSSPoint[Index].x, B_T_BOSSPoint[Index].y, B_T_BOSSPoint[Index].z, 1)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101,	206107+k)         --6个BOSS的提示不同
				msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
				B_T_BOSSDie[InstanceID][k] = 1
            end
		end
	end
end


aux.RegisterCreatureEvent(2055003, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055006, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055009, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055012, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055015, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055018, 4, "BOSS_TrueOrFalse_OnDie")

aux.RegisterCreatureEvent(2055030, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055033, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055036, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055039, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055042, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055045, 4, "BOSS_TrueOrFalse_OnDie")

aux.RegisterCreatureEvent(2055057, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055060, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055063, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055066, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055069, 4, "BOSS_TrueOrFalse_OnDie")
aux.RegisterCreatureEvent(2055072, 4, "BOSS_TrueOrFalse_OnDie")

--杀掉BOSS后的流程
function BOSS_TrueOrFalse_BOSSOnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	if TargetTypeID <= 2055024 then
	    local k = TargetTypeID - 2055018
		B_T_BOSSDie[InstanceID][k] = 2
		local num = 0
		for i = 1,6 do
			if B_T_BOSSDie[InstanceID][i] ~= nil and B_T_BOSSDie[InstanceID][i] == 2 then
				num = num + 1
			end
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,	206114)         --XXX已经死亡！它是本次活动中第X个被击杀的恶魔！
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 9, num)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		if num == 6 then
		    --act.SetActScriptData(28, 1, 1, 5)
			--act.SaveActScriptData(28)
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101,	206115)         --魔王凯·米隆：卑微的凡人，你们已经将我的耐性消磨殆尽！
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		    local Index = math.random(7)
		    map.MapCreateColCreature(MapID, InstanceID, 2055025, B_T_BOSSPoint[Index].x, B_T_BOSSPoint[Index].y, B_T_BOSSPoint[Index].z, 1)
		end

		if B_T_YaoJing[InstanceID] == 0 or math.random(20) == 15 then

		    B_T_YaoJing[InstanceID] = 1
		    local q = math.random(100)
			local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local ID = 0
			local yaojing = 0
			if q <= 50 then
			    yaojing = 1092002
				ID = map.MapCreateCreature(MapID, InstanceID, 1092002, x, y, z)
			elseif q > 50 then
			    yaojing = 1092007
				ID = map.MapCreateCreature(MapID, InstanceID, 1092007, x, y, z)
			end

			unit.AddBuff(MapID, InstanceID, ID, 5932801, ID)
			local MsgID = msg.BeginMsgEvent()
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

	elseif TargetTypeID ~= 2055025 and TargetTypeID <= 2055051 then
	    local k = TargetTypeID - 2055045
		B_T_BOSSDie[InstanceID][k] = 2
		local num = 0
		for i = 1,6 do
			if B_T_BOSSDie[InstanceID][i] ~= nil and B_T_BOSSDie[InstanceID][i] == 2 then
				num = num + 1
			end
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,	206114)         --XXX已经死亡！它是本次活动中第X个被击杀的恶魔！
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 9, num)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		if num == 6 then
		    --act.SetActScriptData(28, 1, 2, 5)
			--act.SaveActScriptData(28)
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101,	206115)         --魔王凯·米隆：卑微的凡人，你们已经将我的耐性消磨殆尽！
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		    local Index = math.random(7)
		    map.MapCreateColCreature(MapID, InstanceID, 2055052, B_T_BOSSPoint[Index].x, B_T_BOSSPoint[Index].y, B_T_BOSSPoint[Index].z, 1)
		end

		if B_T_YaoJing[InstanceID] == 0 or math.random(20) == 15 then

		    B_T_YaoJing[InstanceID] = 1
		    local q = math.random(100)
			local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local ID = 0
			local yaojing = 0
			if q <= 50 then
			    yaojing = 1092003
				ID = map.MapCreateCreature(MapID, InstanceID, 1092003, x, y, z)
			elseif q > 50 then
			    yaojing = 1092008
				ID = map.MapCreateCreature(MapID, InstanceID, 1092008, x, y, z)
			end

			unit.AddBuff(MapID, InstanceID, ID, 5932801, ID)
			local MsgID = msg.BeginMsgEvent()
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

	elseif TargetTypeID ~= 2055052 and TargetTypeID ~= 2055025 and TargetTypeID <= 2055078 then
	    local k = TargetTypeID - 2055072
		B_T_BOSSDie[InstanceID][k] = 2
		local num = 0
		for i = 1,6 do
			if B_T_BOSSDie[InstanceID][i] ~= nil and B_T_BOSSDie[InstanceID][i] == 2 then
				num = num + 1
			end
		end
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,	206114)         --XXX已经死亡！它是本次活动中第X个被击杀的恶魔！
		msg.AddMsgEvent(MsgID, 5, TargetTypeID)
		msg.AddMsgEvent(MsgID, 9, num)
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		if num == 6 then
		    --act.SetActScriptData(28, 1, 3, 5)
			--act.SaveActScriptData(28)
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101,	206115)         --魔王凯·米隆：卑微的凡人，你们已经将我的耐性消磨殆尽！
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)
		    local Index = math.random(7)
		    map.MapCreateColCreature(MapID, InstanceID, 2055079, B_T_BOSSPoint[Index].x, B_T_BOSSPoint[Index].y, B_T_BOSSPoint[Index].z, 1)
		end

		if B_T_YaoJing[InstanceID] == 0 or math.random(20) == 15 then

		    B_T_YaoJing[InstanceID] = 1
		    local q = math.random(100)
			local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
			local ID = 0
			local yaojing = 0
			if q <= 50 then
			    yaojing = 1092005
				ID = map.MapCreateCreature(MapID, InstanceID, 1092005, x, y, z)
			elseif q > 50 then
			    yaojing = 1092010
				ID = map.MapCreateCreature(MapID, InstanceID, 1092010, x, y, z)
			end

			unit.AddBuff(MapID, InstanceID, ID, 5932801, ID)
			local MsgID = msg.BeginMsgEvent()
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

	elseif TargetTypeID == 2055025 or TargetTypeID == 2055052 or TargetTypeID == 2055079 then
	    --act.SetActScriptData(28, 1, (TargetTypeID - 2054998)/27, 5)
		--act.SaveActScriptData(28)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101,	206116)         --魔王凯·米隆已经被XXX击败，混沌魔域的恶魔们再无还手之力！
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchWorldMsgEvent(MsgID)
	end

end
aux.RegisterCreatureEvent(2055019, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055020, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055021, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055022, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055023, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055024, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055025, 4, "BOSS_TrueOrFalse_BOSSOnDie")

aux.RegisterCreatureEvent(2055046, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055047, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055048, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055049, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055050, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055051, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055052, 4, "BOSS_TrueOrFalse_BOSSOnDie")

aux.RegisterCreatureEvent(2055073, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055074, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055075, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055076, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055077, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055078, 4, "BOSS_TrueOrFalse_BOSSOnDie")
aux.RegisterCreatureEvent(2055079, 4, "BOSS_TrueOrFalse_BOSSOnDie")

function BOSS_TOF_OnEnterCombat(MapID, InstanceID, CreatureID)

	unit.AddBuff(MapID, InstanceID, CreatureID, 5935001, CreatureID)

end

aux.RegisterCreatureEvent(2055019, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055020, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055021, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055022, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055023, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055024, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055025, 2, "BOSS_TOF_OnEnterCombat")

aux.RegisterCreatureEvent(2055046, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055047, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055048, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055049, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055050, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055051, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055052, 2, "BOSS_TOF_OnEnterCombat")

aux.RegisterCreatureEvent(2055073, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055074, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055075, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055076, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055077, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055078, 2, "BOSS_TOF_OnEnterCombat")
aux.RegisterCreatureEvent(2055079, 2, "BOSS_TOF_OnEnterCombat")


function C2055081_OnEnterCombat(MapID, InstanceID, CreatureID)

	unit.AddBuff(MapID, InstanceID, CreatureID, 5935201, CreatureID)

end

aux.RegisterCreatureEvent(2055081, 2, "C2055081_OnEnterCombat")
aux.RegisterCreatureEvent(2055054, 2, "C2055081_OnEnterCombat")
aux.RegisterCreatureEvent(2055027, 2, "C2055081_OnEnterCombat")


function h03_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID ~= 2631 then
		if B_T_Trigger[ObjID] ~= nil then
			local k = math.random(2)
			local k2 = math.random(3)
			if B_T_Trigger[ObjID][k] == 4 then
			    unit.AddBuff(MapID, InstanceID, RoleID, 5935401, RoleID)
			elseif B_T_Trigger[ObjID][k] == 5 then
                unit.AddBuff(MapID, InstanceID, RoleID, 5935501, RoleID)
            end
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, MapID, B_T_Entrance[B_T_Trigger[ObjID][k]][k2].x, B_T_Entrance[B_T_Trigger[ObjID][k]][k2].y, B_T_Entrance[B_T_Trigger[ObjID][k]][k2].z)
			if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 59354) and B_T_Trigger[ObjID][k] ~= 4 then --判断是否被触发过
				unit.CancelBuff(MapID, InstanceID, RoleID, 5935401)
			end
			if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 59355) and B_T_Trigger[ObjID][k] ~= 5 then --判断是否被触发过
				unit.CancelBuff(MapID, InstanceID, RoleID, 5935501)
			end
		end
	elseif ObjID == 2631 then
	    unit.AddBuff(MapID, InstanceID, RoleID, 5935601, RoleID)
	end
end
aux.RegisterMapEvent("h03_1",6,"h03_OnEnterTrigger")
aux.RegisterMapEvent("h03_2",6,"h03_OnEnterTrigger")
aux.RegisterMapEvent("h03_3",6,"h03_OnEnterTrigger")

--[[function _PlayerEnter(MapID, InstanceID, RoleID)
	map.MapCreateColCreature(MapID, InstanceID, 6010061, 6372/50, 32, 8191/50, 1)
end

--注册进入活动地图事件
aux.RegisterMapEvent("", 2, "_PlayerEnter")]]

--找到最合适的副本实例
function h03_1GetOnePerfectMap(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("h03_1",  14, "h03_1GetOnePerfectMap")

--找到最合适的副本实例
function h03_2GetOnePerfectMap(MapID, InstanceID, RoleID)

	return 2

end
aux.RegisterMapEvent("h03_2",  14, "h03_2GetOnePerfectMap")

--找到最合适的副本实例
function h03_3GetOnePerfectMap(MapID, InstanceID, RoleID)

	return 3

end
aux.RegisterMapEvent("h03_3",  14, "h03_3GetOnePerfectMap")

--脚本判断(60分钟后无法PK)
function h03_1_FriendEnemy(MapID, InstanceID, SrcID, TargetID)

	if BOSS_TrueOrFalse_TimerMin ~= nil and BOSS_TrueOrFalse_TimerMin > 90 and unit.IsPlayer(SrcID) and unit.IsPlayer(TargetID) and InstanceID ~= 3 then
		return true, false, false, true
	end
    return false, false, false, false
end
aux.RegisterMapEvent("h03_1",  16, "h03_1_FriendEnemy")
aux.RegisterMapEvent("h03_2",  16, "h03_1_FriendEnemy")
aux.RegisterMapEvent("h03_3",  16, "h03_1_FriendEnemy")


--用15秒怪物控制时间，当计时器使用
function c6010070_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    if B_T_Door ~= nil and B_T_Door == 0 then
	    B_T_Door = 1
	    map.OpenCloseDoor(2766543506, 1, 1515, 1)
		map.OpenCloseDoor(2766543250, 2, 1515, 1)
		map.OpenCloseDoor(2766542994, 3, 1515, 1)
		map.MapCreateColCreature(MapID, 1, 6010070, 1, 1, 1, 1)
	elseif B_T_Door ~= nil and B_T_Door == 1 then
		B_T_Door = 0
	    map.OpenCloseDoor(2766543506, 1, 1515, nil)
		map.OpenCloseDoor(2766543250, 2, 1515, nil)
		map.OpenCloseDoor(2766542994, 3, 1515, nil)
		map.MapCreateColCreature(2766543506, 1, 6010070, 1, 1, 1, 1)
	end
end

aux.RegisterCreatureEvent(6010070, 13, "c6010070_OnDisappear")

--玩家在活动中掉线后上线
function h03_CanTakeOverWhenOnline(MapID, RoleID)
	--修正玩家的上线地图和坐标todo
	local k = math.random(3)
	local point = {}
	point[1] = {x=954,y=458,z=537}
	point[2] = {x=330,y=458,z=527}
	point[3] = {x=582,y=459,z=298}
	return 3424073512, point[k].x, point[k].y*51, point[k].z
end

aux.RegisterMapEvent("h03_1",  12, "h03_CanTakeOverWhenOnline")
aux.RegisterMapEvent("h03_2",  12, "h03_CanTakeOverWhenOnline")
aux.RegisterMapEvent("h03_3",  12, "h03_CanTakeOverWhenOnline")

-------------------------------------------------------------------------------------------------------
--找到地图的出口和坐标
function h03_GetExporMapAndCoord(MapID, InstanceID, RoleID)
    local k = math.random(3)
	local point = {}
	point[1] = {x=954,y=458,z=537}
	point[2] = {x=330,y=458,z=527}
	point[3] = {x=582,y=459,z=298}
	return 1, 3424073512, point[k].x, point[k].y, point[k].z
end

aux.RegisterMapEvent("h03_1",  13, "h03_GetExporMapAndCoord")
aux.RegisterMapEvent("h03_2",  13, "h03_GetExporMapAndCoord")
aux.RegisterMapEvent("h03_3",  13, "h03_GetExporMapAndCoord")

--检测玩家是否能够进入活动场景
function h03_CanEnter(MapID, InstanceID, RoleID)

	return 1

end

aux.RegisterMapEvent("h03_1",  15, "h03_CanEnter")
aux.RegisterMapEvent("h03_2",  15, "h03_CanEnter")
aux.RegisterMapEvent("h03_3",  15, "h03_CanEnter")

--杀掉普通小怪会获得神光碎片
function BOSS_7Area_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

    if math.random(700) == 350 then
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		map.MapCreateCreature(MapID, InstanceID, 6011022, x, y, z)
	end

    if B_T_ZhuangBei[RoleID] == nil then
	    B_T_ZhuangBei[RoleID] = {moguang=0,shenguang=0}
	end
	if TargetTypeID < 2055018 and math.random(100) >= 99 and role.GetBagFreeSize(RoleID) >= 1 then    --8/12
	    if B_T_ZhuangBei[RoleID].shenguang < math.floor(BOSS_TrueOrFalse_TimerMin / 15) + 1 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4090016, 1, -1, 8, 420)
			B_T_ZhuangBei[RoleID].shenguang = B_T_ZhuangBei[RoleID].shenguang + 1
		end
	elseif TargetTypeID < 2055045 and math.random(100) >= 99 and role.GetBagFreeSize(RoleID) >= 1 then  --16/120
	    if B_T_ZhuangBei[RoleID].shenguang < math.floor(BOSS_TrueOrFalse_TimerMin / 8) + 1 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4090016, 1, -1, 8, 420)
			B_T_ZhuangBei[RoleID].shenguang = B_T_ZhuangBei[RoleID].shenguang + 1
		end
	elseif TargetTypeID < 2055072 and math.random(100) >= 98 and role.GetBagFreeSize(RoleID) >= 1 then  --24/120
	    if B_T_ZhuangBei[RoleID].shenguang < math.floor(BOSS_TrueOrFalse_TimerMin / 5) + 1 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4090016, 1, -1, 8, 420)
			B_T_ZhuangBei[RoleID].shenguang = B_T_ZhuangBei[RoleID].shenguang + 1
		end
	end
	if TargetTypeID < 2055018 and math.random(150) >= 149 and role.GetBagFreeSize(RoleID) >= 1 then    --4/6
	    if B_T_ZhuangBei[RoleID].moguang < math.floor(BOSS_TrueOrFalse_TimerMin / 30) + 1 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4090015, 1, -1, 8, 420)
			B_T_ZhuangBei[RoleID].moguang = B_T_ZhuangBei[RoleID].moguang + 1
		end
	elseif TargetTypeID < 2055045 and math.random(100) >= 99 and role.GetBagFreeSize(RoleID) >= 1 then  --8/60
	    if B_T_ZhuangBei[RoleID].moguang < math.floor(BOSS_TrueOrFalse_TimerMin / 15) + 1 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4090015, 1, -1, 8, 420)
			B_T_ZhuangBei[RoleID].moguang = B_T_ZhuangBei[RoleID].moguang + 1
		end
	elseif TargetTypeID < 2055072 and math.random(150) >= 149 and role.GetBagFreeSize(RoleID) >= 1 then  --12/60
	    if B_T_ZhuangBei[RoleID].moguang < math.floor(BOSS_TrueOrFalse_TimerMin / 10) + 1 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4090015, 1, -1, 8, 420)
			B_T_ZhuangBei[RoleID].moguang = B_T_ZhuangBei[RoleID].moguang + 1
		end
	end
end
aux.RegisterCreatureEvent(2055001, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055002, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055004, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055005, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055007, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055008, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055010, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055011, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055013, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055014, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055016, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055017, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055028, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055029, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055031, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055032, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055034, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055035, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055037, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055038, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055040, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055041, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055043, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055044, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055055, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055056, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055058, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055059, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055061, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055062, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055064, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055065, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055067, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055068, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055070, 4, "BOSS_7Area_OnDie")
aux.RegisterCreatureEvent(2055071, 4, "BOSS_7Area_OnDie")


function c6011022_OnCheck(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
 	local k = math.random(100)
	if k >= 80 then
	    local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
		if InstanceID == 1 then
			map.MapCreateCreature(MapID, InstanceID, 2055082, x, y, z)
		elseif InstanceID == 2 then
		    map.MapCreateCreature(MapID, InstanceID, 2055083, x, y, z)
		elseif InstanceID == 3 then
		    map.MapCreateCreature(MapID, InstanceID, 2055084, x, y, z)
		end
	elseif k >=56 and role.GetBagFreeSize(RoleID) >= 1 then
	    if InstanceID == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4090015, 1, -1, 8, 420)
		elseif InstanceID == 2 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4090015, 2, -1, 8, 420)
		elseif InstanceID == 3 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4090015, 2, -1, 8, 420)
		end
	elseif role.GetBagFreeSize(RoleID) >= 1 then
	    if InstanceID == 1 then
			role.AddRoleItem(MapID, InstanceID, RoleID, 4090016, 1, -1, 8, 420)
		elseif InstanceID == 2 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4090016, 2, -1, 8, 420)
		elseif InstanceID == 3 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4090016, 2, -1, 8, 420)
		end
	end
end

--注册
aux.RegisterCreatureEvent(6011022, 6, "c6011022_OnCheck")

function h03_PlayerEnter(MapID, InstanceID, RoleID)
    local Hour  	= tonumber(os.date("%H"))
	local Min  		= tonumber(os.date("%M"))
	local Second	= tonumber(os.date("%S"))
	local leftSec 	= (59 - Min) * 60 + 59 - Second		--活动剩余时间
	if Hour == 21 then
	    leftSec = leftSec + 3600
	end
	--同步客户端时间
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 16,	28)								--
	msg.AddMsgEvent(MsgID, 16,	leftSec)							--剩余时间
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

end

--注册进入活动地图事件
aux.RegisterMapEvent("h03_1", 2, "h03_PlayerEnter")
aux.RegisterMapEvent("h03_2", 2, "h03_PlayerEnter")
aux.RegisterMapEvent("h03_3", 2, "h03_PlayerEnter")

--完成真假魔王任务
--[[
function TrueOrFalse_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	--vip特殊奖励
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90034) == true then
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) == 0 or role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) == nil then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"], tonumber(os.date("%j")))
		end
		local theDays = tonumber(os.date("%j")) - role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) - 1
		local addedExp = {}
		addedExp[30501] = 1050000
		addedExp[30502] = 4500000
		addedExp[30503] = 12000000
		if theDays >= 4 or theDays >= -361 and theDays < 0 then
			role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[QuestID]*4)
		elseif theDays == 3 or theDays == -362 then
			role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[QuestID]*3)
		elseif theDays == 2 or theDays == -363 then
			role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[QuestID]*2)
		elseif theDays == 1 or theDays == -364 then
			role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[QuestID])
		end
	end
	role.SetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"], tonumber(os.date("%j")))
end
aux.RegisterQuestEvent(30501, 1, "TrueOrFalse_OnComplete")
aux.RegisterQuestEvent(30502, 1, "TrueOrFalse_OnComplete")
aux.RegisterQuestEvent(30503, 1, "TrueOrFalse_OnComplete")
--]]
function q30501_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	UpdateSeverData(50, 1)
	--玩家计数
	UpdateRoleData(RoleID, 11, 1)
	--vip特殊奖励
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90034) == true then
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) == 0 or role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) == nil then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"], tonumber(os.date("%j")))
		end
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) ~= tonumber(os.date("%j")) then
			local theDays = tonumber(os.date("%j")) - role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) - 1
			if theDays >= 4 or theDays >= -361 and theDays < 0 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 1050000*4)
			elseif theDays == 3 or theDays == -362 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 1050000*3)
			elseif theDays == 2 or theDays == -363 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 1050000*2)
			elseif theDays == 1 or theDays == -364 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 1050000)
			end
		end
	end
	role.SetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"], tonumber(os.date("%j")))
end
aux.RegisterQuestEvent(30501, 1, "q30501_Complete")
--51完成真假魔王任务2
function q30502_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	UpdateSeverData(51, 1)
	--玩家计数
	UpdateRoleData(RoleID, 11, 1)
	--vip特殊奖励
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90034) == true then
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) == 0 or role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) == nil then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"], tonumber(os.date("%j")))
		end
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) ~= tonumber(os.date("%j")) then
			local theDays = tonumber(os.date("%j")) - role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) - 1
			if theDays >= 4 or theDays >= -361 and theDays < 0 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 4500000*4)
			elseif theDays == 3 or theDays == -362 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 4500000*3)
			elseif theDays == 2 or theDays == -363 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 4500000*2)
			elseif theDays == 1 or theDays == -364 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 4500000)
			end
		end
	end
	role.SetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"], tonumber(os.date("%j")))
end
aux.RegisterQuestEvent(30502, 1, "q30502_Complete")
--52完成真假魔王任务3
function q30503_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	UpdateSeverData(52, 1)
	--玩家计数
	UpdateRoleData(RoleID, 11, 1)
	--vip特殊奖励
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90034) == true then
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) == 0 or role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) == nil then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"], tonumber(os.date("%j")))
		end
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) ~= tonumber(os.date("%j")) then
			local theDays = tonumber(os.date("%j")) - role.GetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"]) - 1
			if theDays >= 4 or theDays >= -361 and theDays < 0 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 12000000*4)
			elseif theDays == 3 or theDays == -362 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 12000000*3)
			elseif theDays == 2 or theDays == -363 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 12000000*2)
			elseif theDays == 1 or theDays == -364 then
				role.AddRoleExp(MapID, InstanceID, RoleID, 12000000)
			end
		end
	end
	role.SetRoleScriptData(RoleID, 1, RoleDataType["zhenjiaVIP"], tonumber(os.date("%j")))
end
aux.RegisterQuestEvent(30503, 1, "q30503_Complete")
