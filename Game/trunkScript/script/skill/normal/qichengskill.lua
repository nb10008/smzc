-------------不能在副本使用的技能

Skill_InstanceCRC = {}
Skill_InstanceCRC[1] = 1266372430 --f01
Skill_InstanceCRC[2] = 1266371662 --f02
Skill_InstanceCRC[3] = 1266371918 --f03
Skill_InstanceCRC[4] = 1266371150 --f04
Skill_InstanceCRC[5] = 1266371406 --f05
Skill_InstanceCRC[6] = 1266370638 --f06
Skill_InstanceCRC[7] = 1266370894 --f07
Skill_InstanceCRC[8] = 619033530 --h01
Skill_InstanceCRC[9] = 619032762 --h02
Skill_InstanceCRC[10] = 3373741608 --s53
Skill_InstanceCRC[11] = 1266370126 --f08
Skill_InstanceCRC[12] = 3390517544 --s64
-----------------------------------------

Skill_NormalCRC = {}
Skill_NormalCRC[1] = 3424072744
Skill_NormalCRC[2] = 3424073512
Skill_NormalCRC[3] = 3424073256
Skill_NormalCRC[4] = 3424071976
Skill_NormalCRC[5] = 3424071720
Skill_NormalCRC[6] = 3424072488
Skill_NormalCRC[7] = 3424072232
Skill_NormalCRC[8] = 3424075048
Skill_NormalCRC[9] = 3951677761
Skill_NormalCRC[10] = 3951678017
Skill_NormalCRC[11] = 3951678273
Skill_NormalCRC[12] = 3951678529
Skill_NormalCRC[13] = 3951678785
Skill_NormalCRC[14] = 3951679041
Skill_NormalCRC[15] = 3951679297
Skill_NormalCRC[16] = 3951679553
Skill_NormalCRC[17] = 3951679809
Skill_NormalCRC[18] = 3934900289
Skill_NormalCRC[19] = 881225009
-------------不能在竞技场之外使用的技能

Skill_BattleGround = {}
Skill_BattleGround[1] = 2092183860 --a01
Skill_BattleGround[2] = 2092184116 --a02
Skill_BattleGround[3] = 2092184372 --a03
Skill_BattleGround[4] = 2216111541 --a04
Skill_BattleGround[5] = 2216112053 --a05
Skill_BattleGround[6] = 2216110261 --a06

function s9000101_CanCast(MapID, InstanceID, SkillID, OwnerID, TargetID)
	for i in pairs(Skill_InstanceCRC) do
		if MapID == Skill_InstanceCRC[i] then
        		return 23
		end
   	end
	return 0

end


--注册
aux.RegisterSkillEvent(9000101, 0, "s9000101_CanCast")

function s2013301_CanCast(MapID, InstanceID, SkillID, OwnerID, TargetID)
	for i in pairs(Skill_InstanceCRC) do
		if MapID == Skill_InstanceCRC[i] then
        		return 23
		end
   	end
	return 0

end


--注册
aux.RegisterSkillEvent(2013301, 0, "s2013301_CanCast")

function sArena_CanCast(MapID, InstanceID, SkillID, OwnerID, TargetID)
	for i in pairs(Skill_BattleGround) do
		if MapID == Skill_BattleGround[i] then
        		return 0
		end
   	end
	return 23

end


--注册
aux.RegisterSkillEvent(4100101, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100201, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100301, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100401, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100402, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100403, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100501, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100502, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100503, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100504, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100505, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100601, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100602, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100603, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100701, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100702, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100703, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100704, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100705, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100801, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100802, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100803, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100901, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100902, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100903, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100904, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4100905, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4101001, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4101002, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4101003, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4101101, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4101102, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4101103, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4101104, 0, "sArena_CanCast")
aux.RegisterSkillEvent(4101105, 0, "sArena_CanCast")
