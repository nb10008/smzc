-- 39200 4245 6800
--开启守卫地图刷怪
function jidi_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local chenggong = 0
	if TargetTypeID == 4020199 then
		chenggong = 1
	end

	--[[local 
	Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		for i=1, 6 do
			map.ShowWarResult(MapID,InstanceID,chenggong)--结算界面 地图，副本，胜败
		end
	elseif TeamID ~= nil then]]
		map.ShowWarResult(MapID,InstanceID,chenggong)--结算界面
	--end
end
aux.RegisterCreatureEvent(6040041, 4, "jidi_OnDie")--基地死亡
aux.RegisterCreatureEvent(4020199, 4, "jidi_OnDie")--
function zhanchangjiangli(shuangxincishu)
	local pinzhi = {}
	pinzhi[1] = {}
	pinzhi[1][1]={3110019,1}
	pinzhi[1][2]={3110020,2}
	pinzhi[1][3]={3110021,3}
	pinzhi[2] = {}
	pinzhi[2][1]={3110022,1}
	pinzhi[2][2]={3110023,2}
	pinzhi[2][3]={3110024,3}
	pinzhi[3] = {}
	pinzhi[3][1]={3110025,1}
	pinzhi[3][2]={3110026,2}
	pinzhi[3][3]={3110027,3}
	pinzhi[4] = {}
	pinzhi[4][1]={3110028,1}
	pinzhi[4][2]={3110029,2}
	pinzhi[4][3]={3110030,3}
	pinzhi[5] = {}
	pinzhi[5][1]={3110031,1}
	pinzhi[5][2]={3110032,2}
	pinzhi[5][3]={3110033,3}
	pinzhi[6] = {}
	pinzhi[6][1]={3110034,1}
	pinzhi[6][2]={3110035,2}
	pinzhi[6][3]={3110036,3}
	local shuaxin ={}
	shuaxin[0] ={}
	shuaxin[0][1]={1,450}
	shuaxin[0][2]={2,300}
	shuaxin[0][3]={3,150}
	shuaxin[0][4]={4,100}
	shuaxin[1] ={}
	shuaxin[1][1]={1,450}
	shuaxin[1][2]={2,300}
	shuaxin[1][3]={3,120}
	shuaxin[1][4]={4,130}
	shuaxin[2] ={}
	shuaxin[2][1]={1,350}
	shuaxin[2][2]={2,300}
	shuaxin[2][3]={3,120}
	shuaxin[2][4]={4,220}
	shuaxin[2][5]={5,1}
	shuaxin[3] ={}
	shuaxin[3][1]={1,350}
	shuaxin[3][2]={2,300}
	shuaxin[3][3]={3,120}
	shuaxin[3][4]={4,220}
	shuaxin[3][5]={5,10}
	shuaxin[4] ={}
	shuaxin[4][1]={1,300}
	shuaxin[4][2]={2,250}
	shuaxin[4][3]={3,120}
	shuaxin[4][4]={4,305}
	shuaxin[4][5]={5,25}
	shuaxin[5] ={}
	shuaxin[5][1]={1,200}
	shuaxin[5][2]={2,200}
	shuaxin[5][3]={3,120}
	shuaxin[5][4]={4,435}
	shuaxin[5][5]={5,45}
	shuaxin[6] ={}
	shuaxin[6][1]={1,50}
	shuaxin[6][2]={2,150}
	shuaxin[6][3]={3,90}
	shuaxin[6][4]={4,623}
	shuaxin[6][5]={5,80}
	shuaxin[6][6]={5,7}
	shuaxin[7] ={}
	shuaxin[7][1]={2,90}
	shuaxin[7][2]={3,60}
	shuaxin[7][3]={4,680}
	shuaxin[7][4]={5,160}
	shuaxin[7][5]={6,10}
	shuaxin[8] ={}
	shuaxin[8][1]={3,50}
	shuaxin[8][2]={4,590}
	shuaxin[8][3]={5,345}
	shuaxin[8][4]={6,15}
	shuaxin[9] ={}
	shuaxin[9][1]={3,10}
	shuaxin[9][2]={4,410}
	shuaxin[9][3]={5,550}
	shuaxin[9][4]={6,30}
	shuaxin[10] ={}
	shuaxin[10][1]={4,150}
	shuaxin[10][2]={5,750}
	shuaxin[10][3]={6,100}
	shuaxin[11] ={}
	shuaxin[11][1]={4,100}
	shuaxin[11][2]={5,700}
	shuaxin[11][3]={6,200}
	shuaxin[12] ={}
	shuaxin[12][1]={4,50}
	shuaxin[12][2]={5,650}
	shuaxin[12][3]={6,300}
	shuaxin[13] ={}
	shuaxin[13][1]={5,500}
	shuaxin[13][2]={6,500}
	local item = 0 --箱子ID
	local r = math.random(1000000)
	local xw = math.random(3)
	local temp = 0
	local index1 = 1
	local index2 = 1
	local sx = 0 --箱子种类数
	local pz = 0 --箱子品质
	if shuangxincishu ==0 or shuangxincishu == 1 or shuangxincishu == 8 or shuangxincishu == 9 then
		sx = 4
	elseif huangxincishu >=10 and shuangxincishu <= 12 then
		sx = 3
	elseif huangxincishu == 6 then
		sx = 6
	elseif huangxincishu >= 13 then
		sx = 2
	else
		sx = 5
	end
	for i = 1,sx do
	    temp = temp + shuaxin[shuangxincishu][i][2]
		if temp >= r then
		    index1 = i
			break
		end
	end
	pz = shuaxin[shuangxincishu][index1][1]
	
	for a = 1,3 do
	    temp = temp + pinzhi[pz][a][2]
		if temp >= r then
		    index2 = i
			break
		end
	end
	item = pinzhi[pz][index2][1]
	return item
end

function shouhuditujifen(jifen)
	local x = 0 
	if jifen <= 226 then 
		x = 0
	elseif jifen > 226 or jifen <= 283 then
		x = 1
	elseif jifen > 283 or jifen <= 354 then
		x = 2
	elseif jifen > 354 or jifen <= 443 then
		x = 3
	elseif jifen > 443 or jifen <= 554 then
		x = 4
	elseif jifen > 554 or jifen <= 693 then
		x = 5
	elseif jifen > 693 or jifen <= 867 then
		x = 6
	elseif jifen > 867 or jifen <= 1084 then
		x = 7
	elseif jifen > 1084 or jifen <= 1355 then
		x = 8
	elseif jifen > 1355 or jifen <= 1694 then
		x = 9
	elseif jifen > 1694 or jifen <= 2117 then
		x = 10
	elseif jifen > 2117 or jifen <= 2647 then
		x = 11
	elseif jifen > 2647 or jifen <= 3300 then
		x = 12
	elseif jifen > 3301 then
		x = 13
	end
	local item = {}
	item[1] = {0}
	item[2] = {0}
	item[3] = {0}
	for i = 1,3 do
		item[i][1] = zhanchangjiangli(x)
	end
	return item[1][1],item[2][1],item[3][1]
end
aux.RegisterRoleEvent(90,"shouhuditujifen")