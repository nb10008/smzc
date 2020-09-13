
--任务武器挑选（ID：20002）完成后，自动弹出角色属性界面[C]
--任务初次战斗（ID：20003）完成后，自动弹出角色技能界面[K]
--任务赠予礼包（ID：20005）完成后，自动弹出背包界面
--任务讨伐魔灵（ID：20006）接受后，自动弹出任务界面
--任务更换武器（ID：20009）完成后，自动弹出背包界面和角色属性界面
--任务新的征途（ID：20017）完成后，自动弹出背包界面
--任务城市布局（ID：20021）完成后，自动弹出区域地图界面


function q20002_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 1)
end

function q20003_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 3)
end

function q20005_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 2)
end

function q20006_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 4)
end

function q20009_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 1)
	role.OpenFramePage(ownerID, 2)
end

function q20017_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.OpenFramePage(ownerID, 2)
end




--注册
aux.RegisterQuestEvent(20002, 1, "q20002_OnComplete")
aux.RegisterQuestEvent(20003, 1, "q20003_OnComplete")
aux.RegisterQuestEvent(20005, 1, "q20005_OnComplete")
aux.RegisterQuestEvent(20006, 1, "q20006_OnComplete")
aux.RegisterQuestEvent(20009, 1, "q20009_OnComplete")
aux.RegisterQuestEvent(20017, 1, "q20017_OnComplete")







