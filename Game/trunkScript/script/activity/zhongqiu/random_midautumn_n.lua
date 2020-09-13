--接取任务
function q30830_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)
	local Index = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local Quest_RandomCollect = {}
	Quest_RandomCollect[30]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=11000,MsgID=20004}
	Quest_RandomCollect[31]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=11600,MsgID=20004}
	Quest_RandomCollect[32]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=12000,MsgID=20004}
	Quest_RandomCollect[33]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=13000,MsgID=20004}
	Quest_RandomCollect[34]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=13600,MsgID=20004}
	Quest_RandomCollect[35]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=14600,MsgID=20004}
	Quest_RandomCollect[36]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=15600,MsgID=20004}
	Quest_RandomCollect[37]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=16300,MsgID=20004}
	Quest_RandomCollect[38]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=17300,MsgID=20004}
	Quest_RandomCollect[39]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=18300,MsgID=20004}
	Quest_RandomCollect[40]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=30600,MsgID=20004}
	Quest_RandomCollect[41]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=32300,MsgID=20004}
	Quest_RandomCollect[42]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=33600,MsgID=20004}
	Quest_RandomCollect[43]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=36000,MsgID=20004}
	Quest_RandomCollect[44]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=37600,MsgID=20004}
	Quest_RandomCollect[45]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=41300,MsgID=20004}
	Quest_RandomCollect[46]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=43000,MsgID=20004}
	Quest_RandomCollect[47]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=45300,MsgID=20004}
	Quest_RandomCollect[48]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=47300,MsgID=20004}
	Quest_RandomCollect[49]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=49600,MsgID=20004}
	Quest_RandomCollect[50]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=81000,MsgID=20004}
	Quest_RandomCollect[51]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=85000,MsgID=20004}
	Quest_RandomCollect[52]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=88300,MsgID=20004}
	Quest_RandomCollect[53]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=92000,MsgID=20004}
	Quest_RandomCollect[54]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=96300,MsgID=20004}
	Quest_RandomCollect[55]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=104600,MsgID=20004}
	Quest_RandomCollect[56]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=109000,MsgID=20004}
	Quest_RandomCollect[57]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=113000,MsgID=20004}
	Quest_RandomCollect[58]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=117300,MsgID=20004}
	Quest_RandomCollect[59]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=30,TargetID2=4800158,Num2=30,RewMoney=0,RewXP=129300,MsgID=20004}
	Quest_RandomCollect[60]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=169000,MsgID=20005}
	Quest_RandomCollect[61]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=175000,MsgID=20005}
	Quest_RandomCollect[62]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=181300,MsgID=20005}
	Quest_RandomCollect[63]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=188000,MsgID=20005}
	Quest_RandomCollect[64]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=194000,MsgID=20005}
	Quest_RandomCollect[65]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=202000,MsgID=20005}
	Quest_RandomCollect[66]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=209000,MsgID=20005}
	Quest_RandomCollect[67]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=216000,MsgID=20005}
	Quest_RandomCollect[68]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=223300,MsgID=20005}
	Quest_RandomCollect[70]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=240000,MsgID=20005}
	Quest_RandomCollect[71]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=357600,MsgID=20005}
	Quest_RandomCollect[72]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=369000,MsgID=20005}
	Quest_RandomCollect[73]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=381000,MsgID=20005}
	Quest_RandomCollect[74]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=393300,MsgID=20005}
	Quest_RandomCollect[75]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=405600,MsgID=20005}
	Quest_RandomCollect[76]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=418600,MsgID=20005}
	Quest_RandomCollect[77]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=431000,MsgID=20005}
	Quest_RandomCollect[78]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=443600,MsgID=20005}
	Quest_RandomCollect[79]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=40,TargetID2=4800158,Num2=40,RewMoney=0,RewXP=456600,MsgID=20005}
	Quest_RandomCollect[80]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=705300,MsgID=20006}
	Quest_RandomCollect[81]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=726000,MsgID=20006}
	Quest_RandomCollect[82]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=747000,MsgID=20006}
	Quest_RandomCollect[83]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=767600,MsgID=20006}
	Quest_RandomCollect[84]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=790000,MsgID=20006}
	Quest_RandomCollect[85]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=812600,MsgID=20006}
	Quest_RandomCollect[86]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=835000,MsgID=20006}
	Quest_RandomCollect[87]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=857300,MsgID=20006}
	Quest_RandomCollect[88]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=880000,MsgID=20006}
	Quest_RandomCollect[89]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=903000,MsgID=20006}
	Quest_RandomCollect[90]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=927300,MsgID=20006}
	Quest_RandomCollect[91]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=1163000,MsgID=20006}
	Quest_RandomCollect[92]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=1182000,MsgID=20006}
	Quest_RandomCollect[93]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=1200000,MsgID=20006}
	Quest_RandomCollect[94]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=1220000,MsgID=20006}
	Quest_RandomCollect[95]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=1240000,MsgID=20006}
	Quest_RandomCollect[96]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=1259000,MsgID=20006}
	Quest_RandomCollect[97]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=1270000,MsgID=20006}
	Quest_RandomCollect[98]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=1290000,MsgID=20006}
	Quest_RandomCollect[99]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=1310000,MsgID=20006}
	Quest_RandomCollect[100]={TargetType=QuestTargetType["Collect"],TargetID1=4800157,Num1=50,TargetID2=4800158,Num2=50,RewMoney=0,RewXP=1350000,MsgID=20006}


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
aux.RegisterQuestEvent(30830, 7, "q30830_OnQuestInit")
