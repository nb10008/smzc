--接取任务
function q30829_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)
	local Index = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local Quest_RandomCollect = {}
	Quest_RandomCollect[30]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=60000,RewXP=99000,MsgID=20001}
	Quest_RandomCollect[31]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=61500,RewXP=105000,MsgID=20001}
	Quest_RandomCollect[32]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=63100,RewXP=108000,MsgID=20001}
	Quest_RandomCollect[33]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=64700,RewXP=117000,MsgID=20001}
	Quest_RandomCollect[34]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=66400,RewXP=123000,MsgID=20001}
	Quest_RandomCollect[35]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=68100,RewXP=132000,MsgID=20001}
	Quest_RandomCollect[36]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=69900,RewXP=141000,MsgID=20001}
	Quest_RandomCollect[37]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=71700,RewXP=147000,MsgID=20001}
	Quest_RandomCollect[38]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=73500,RewXP=156000,MsgID=20001}
	Quest_RandomCollect[39]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=75400,RewXP=165000,MsgID=20001}
	Quest_RandomCollect[40]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=77300,RewXP=276000,MsgID=20001}
	Quest_RandomCollect[41]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=79300,RewXP=291000,MsgID=20001}
	Quest_RandomCollect[42]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=81300,RewXP=303000,MsgID=20001}
	Quest_RandomCollect[43]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=83400,RewXP=324000,MsgID=20001}
	Quest_RandomCollect[44]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=85500,RewXP=339000,MsgID=20001}
	Quest_RandomCollect[45]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=87700,RewXP=372000,MsgID=20001}
	Quest_RandomCollect[46]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=89900,RewXP=387000,MsgID=20001}
	Quest_RandomCollect[47]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=92200,RewXP=408000,MsgID=20001}
	Quest_RandomCollect[48]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=94600,RewXP=426000,MsgID=20001}
	Quest_RandomCollect[49]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=97000,RewXP=447000,MsgID=20001}
	Quest_RandomCollect[50]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=99500,RewXP=729000,MsgID=20001}
	Quest_RandomCollect[51]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=102000,RewXP=765000,MsgID=20001}
	Quest_RandomCollect[52]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=104600,RewXP=795000,MsgID=20001}
	Quest_RandomCollect[53]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=107300,RewXP=828000,MsgID=20001}
	Quest_RandomCollect[54]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=110000,RewXP=867000,MsgID=20001}
	Quest_RandomCollect[55]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=112800,RewXP=942000,MsgID=20001}
	Quest_RandomCollect[56]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=115700,RewXP=981000,MsgID=20001}
	Quest_RandomCollect[57]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=118600,RewXP=1017000,MsgID=20001}
	Quest_RandomCollect[58]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=121600,RewXP=1056000,MsgID=20001}
	Quest_RandomCollect[59]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=150,RewMoney=124700,RewXP=1164000,MsgID=20001}
	Quest_RandomCollect[60]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=127900,RewXP=1521000,MsgID=20002}
	Quest_RandomCollect[61]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=131100,RewXP=1575000,MsgID=20002}
	Quest_RandomCollect[62]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=134400,RewXP=1632000,MsgID=20002}
	Quest_RandomCollect[63]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=137800,RewXP=1692000,MsgID=20002}
	Quest_RandomCollect[64]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=141300,RewXP=1746000,MsgID=20002}
	Quest_RandomCollect[65]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=144900,RewXP=1818000,MsgID=20002}
	Quest_RandomCollect[66]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=148600,RewXP=1881000,MsgID=20002}
	Quest_RandomCollect[67]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=152400,RewXP=1944000,MsgID=20002}
	Quest_RandomCollect[68]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=156300,RewXP=2010000,MsgID=20002}
	Quest_RandomCollect[69]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=160300,RewXP=2160000,MsgID=20002}
	Quest_RandomCollect[70]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=164400,RewXP=3219000,MsgID=20002}
	Quest_RandomCollect[71]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=168600,RewXP=3321000,MsgID=20002}
	Quest_RandomCollect[72]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=172900,RewXP=3429000,MsgID=20002}
	Quest_RandomCollect[73]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=177300,RewXP=3540000,MsgID=20002}
	Quest_RandomCollect[74]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=181800,RewXP=3651000,MsgID=20002}
	Quest_RandomCollect[75]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=186400,RewXP=3768000,MsgID=20002}
	Quest_RandomCollect[76]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=191100,RewXP=3879000,MsgID=20002}
	Quest_RandomCollect[77]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=195900,RewXP=3993000,MsgID=20002}
	Quest_RandomCollect[78]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=200800,RewXP=4110000,MsgID=20002}
	Quest_RandomCollect[79]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=180,RewMoney=205900,RewXP=6348000,MsgID=20002}
	Quest_RandomCollect[80]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=211100,RewXP=6534000,MsgID=20003}
	Quest_RandomCollect[81]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=216400,RewXP=6723000,MsgID=20003}
	Quest_RandomCollect[82]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=221900,RewXP=6909000,MsgID=20003}
	Quest_RandomCollect[83]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=227500,RewXP=7110000,MsgID=20003}
	Quest_RandomCollect[84]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=233200,RewXP=7314000,MsgID=20003}
	Quest_RandomCollect[85]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=239100,RewXP=7515000,MsgID=20003}
	Quest_RandomCollect[86]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=245100,RewXP=7716000,MsgID=20003}
	Quest_RandomCollect[87]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=251300,RewXP=7920000,MsgID=20003}
	Quest_RandomCollect[88]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=257600,RewXP=8127000,MsgID=20003}
	Quest_RandomCollect[89]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=264100,RewXP=8346000,MsgID=20003}
	Quest_RandomCollect[90]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=270800,RewXP=10470000,MsgID=20003}
	Quest_RandomCollect[91]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=277600,RewXP=10638000,MsgID=20003}
	Quest_RandomCollect[92]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=284600,RewXP=10806000,MsgID=20003}
	Quest_RandomCollect[93]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=291800,RewXP=10986000,MsgID=20003}
	Quest_RandomCollect[94]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=299100,RewXP=11160000,MsgID=20003}
	Quest_RandomCollect[95]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=306600,RewXP=11331000,MsgID=20003}
	Quest_RandomCollect[96]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=314300,RewXP=11499000,MsgID=20003}
	Quest_RandomCollect[97]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=322200,RewXP=11661000,MsgID=20003}
	Quest_RandomCollect[98]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=330300,RewXP=11823000,MsgID=20003}
	Quest_RandomCollect[99]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=338600,RewXP=11988000,MsgID=20003}
	Quest_RandomCollect[100]={TargetType=QuestTargetType["Collect"],TargetID1=4800164,Num1=210,RewMoney=347100,RewXP=12150000,MsgID=20003}


	if Index < 30 then
		Index = 30
	elseif Index > 100 then
		Index = 100
	end
	--初始化任务的动态数据
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_RandomCollect, Index))
	--同步客户端总任务环数和任务说明
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_RandomCollect[Index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterQuestEvent(30829, 7, "q30829_OnQuestInit")
