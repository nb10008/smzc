--任务环相关属性定义
--动态任务只填写不为0的条件即可，其他不足由此函数自动置为0
QuestTargetType = {}
QuestTargetType["Kill"]    = 1		--击杀某种怪物
QuestTargetType["Collect"] = 2		--收集物品
QuestTargetType["NPCTalk"] = 3		--NPC对话


--得到随机任务table中一个任务的相关属性（tablename 随机任务表，Num 表的最大索引值）
function GetRandQuestTable(tablename, Index)

    local function Isnil(VAL)
	    if VAL == nil then
		    return 0
		else
		    return VAL
		end
	end

	--击杀怪物类型的任务
	local TargetTypeTB=
	{[1]=Isnil(tablename[Index].TargetID1),
	[2]=Isnil(tablename[Index].TargetID2),
	[3]=Isnil(tablename[Index].TargetID3),
	[4]=Isnil(tablename[Index].TargetID4),
	[5]=Isnil(tablename[Index].Num1),
	[6]=Isnil(tablename[Index].Num2),
	[7]=Isnil(tablename[Index].Num3),
	[8]=Isnil(tablename[Index].Num4),
	[9]=Isnil(tablename[Index].MsgID),
	[10]=Isnil(tablename[Index].RewMoney),
	[11]=Isnil(tablename[Index].RewXP),
	[12]=Isnil(tablename[Index].RewItemID1),
	[13]=Isnil(tablename[Index].value1),
	[14]=Isnil(tablename[Index].RewItemID2),
	[15]=Isnil(tablename[Index].value2),
	[16]=Isnil(tablename[Index].RewItemID3),
	[17]=Isnil(tablename[Index].value3),
	[18]=Isnil(tablename[Index].RewItemID4),
	[19]=Isnil(tablename[Index].value4),
	[20]=Isnil(tablename[Index].RewChoicesItemID1),
	[21]=Isnil(tablename[Index].Cvalue1),
	[22]=Isnil(tablename[Index].RewChoicesItemID2),
	[23]=Isnil(tablename[Index].Cvalue2),
	[24]=Isnil(tablename[Index].RewChoicesItemID3),
	[25]=Isnil(tablename[Index].Cvalue3),
	[26]=Isnil(tablename[Index].RewChoicesItemID4),
	[27]=Isnil(tablename[Index].Cvalue4),
	[28]=Isnil(tablename[Index].TargetType)}

	--for i = 1,28 do
	--	if TargetTypeTB[i] == nil then TargetTypeTB[i] = 0 end
	--end

	if TargetTypeTB[28] == QuestTargetType["Kill"] then
		--返回怪物ID和数量
		return QuestTargetType["Kill"], TargetTypeTB[9], TargetTypeTB[1], TargetTypeTB[5], TargetTypeTB[2], TargetTypeTB[6], TargetTypeTB[3], TargetTypeTB[7], TargetTypeTB[4], TargetTypeTB[8],
		TargetTypeTB[10],TargetTypeTB[11],TargetTypeTB[12],TargetTypeTB[13],TargetTypeTB[14],TargetTypeTB[15],TargetTypeTB[16],TargetTypeTB[17],TargetTypeTB[18],TargetTypeTB[19],TargetTypeTB[20],
		TargetTypeTB[21],TargetTypeTB[22],TargetTypeTB[23],TargetTypeTB[24],TargetTypeTB[25],TargetTypeTB[26],TargetTypeTB[27]
	end

	--收集物品类型的任务
	if TargetTypeTB[28] == QuestTargetType["Collect"] then
		--返回物品TypeID，和数量
		return QuestTargetType["Collect"], TargetTypeTB[9], TargetTypeTB[1], TargetTypeTB[5], TargetTypeTB[2], TargetTypeTB[6], TargetTypeTB[3], TargetTypeTB[7], TargetTypeTB[4], TargetTypeTB[8],
		TargetTypeTB[10],TargetTypeTB[11],TargetTypeTB[12],TargetTypeTB[13],TargetTypeTB[14],TargetTypeTB[15],TargetTypeTB[16],TargetTypeTB[17],TargetTypeTB[18],TargetTypeTB[19],TargetTypeTB[20],
		TargetTypeTB[21],TargetTypeTB[22],TargetTypeTB[23],TargetTypeTB[24],TargetTypeTB[25],TargetTypeTB[26],TargetTypeTB[27]
	end

	--NPC对话类型的任务
	if TargetTypeTB[28] == QuestTargetType["NPCTalk"] then
		--返回NPCTypeID
		return QuestTargetType["NPCTalk"], TargetTypeTB[9], TargetTypeTB[1],TargetTypeTB[2], TargetTypeTB[3], TargetTypeTB[4],
		TargetTypeTB[10],TargetTypeTB[11],TargetTypeTB[12],TargetTypeTB[13],TargetTypeTB[14],TargetTypeTB[15],TargetTypeTB[16],TargetTypeTB[17],TargetTypeTB[18],TargetTypeTB[19],TargetTypeTB[20],
		TargetTypeTB[21],TargetTypeTB[22],TargetTypeTB[23],TargetTypeTB[24],TargetTypeTB[25],TargetTypeTB[26],TargetTypeTB[27]
    end
end

