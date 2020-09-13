function I4700296_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(1000000)
	local temp = 0
	local index = 1
	local tbl = {}

	tbl[1]={3020030,1 ,-1,20000 }
	tbl[2]={4090011,1 ,-1,30000 }	
	tbl[3]={4090010,1 ,-1,30000 }	
	tbl[4]={3020011,1 ,-1,45000 }		
	tbl[5]={3020012,1 ,-1,19000 }	
	tbl[6]={4700229,1 ,-1,77000 }	
	tbl[7]={4700230,1 ,-1,69000 }	
	tbl[8]={4700231,1 ,-1,56000 }	
	tbl[9]={4820256,1 ,-1,25000 }	
	tbl[10]={4820239,1 ,-1,44000 }	
	tbl[11]={4820045,1 ,-1,30000 }	
	tbl[12]={4820049,1 ,-1,20000 }	
	tbl[13]={4530109,3 ,-1,59000 }	
	tbl[14]={4090027,1 ,-1,35000 }	
	tbl[15]={4090026,1 ,-1,35000 }	
	tbl[16]={4820061,15,-1,60000 }	
	tbl[17]={4820047,2 ,-1,60000 }	
	tbl[18]={4820046,2 ,-1,59900 }	
	tbl[19]={4500002,8 ,-1,64000 }	
	tbl[20]={3070051,8 ,-1,64000 }	
	tbl[21]={5600003,1 ,-1,59000 }	
	tbl[22]={3020047,1 ,-1,0   }	
	tbl[23]={3020042,1 ,-1,2}	
	tbl[24]={3020037,1 ,-1,100   }	
	tbl[25]={3020035,1 ,-1,498}	
	tbl[26]={3020089,1 ,-1,1000  }	
	tbl[27]={3020091,1 ,-1,400   }	
	tbl[28]={3020092,1 ,-1,300   }	
	tbl[29]={4222104,1 ,-1,400   }	
	tbl[30]={4211503,1 ,-1,400   }	
	tbl[31]={3020013,1 ,-1,5000  }	
	tbl[32]={3020008,1 ,-1,5000  }	
	tbl[33]={4820035,1 ,-1,20000 }	
	tbl[34]={4820180,1 ,-1,3000  }	
	tbl[35]={4820179,1 ,-1,3000  }	



	for i = 1,35 do
	    temp = temp + tbl[i][4]
		if temp >= r then
		    index = i
			break
		end
	end
	if index >= 22 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 280) --<p1>开启了<p1>获得了<p3>
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 4, TypeID)
		msg.AddMsgEvent(MsgID, 4, tbl[index][1])
		msg.DispatchWorldMsgEvent(MsgID)
	end
	role.AddRoleItem(MapID, InstanceID, TargetID, tbl[index][1], tbl[index][2], tbl[index][3], 8, 420)

end

function I4700296_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700296, 1, "I4700296_GiftBag")
aux.RegisterItemEvent(4700296, 0, "I4700296_CanUseGiftBag")
