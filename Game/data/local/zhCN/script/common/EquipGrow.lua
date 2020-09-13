local function OnTalk()
		Quest.ClearNPCTalkOptions();                    --清除对话框选项
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"装备成长");

		Quest.NPCTalk(0,0,"器魂锻造师：\\n    具有成长属性的装备可以在我这里进行一些相关的操作。\\n    降低装备成长等级的道具可以在达到<color=0xff00a2ff>80<color=0xffffffff>级之后进入古神陵墓与憎恨魔窟击杀怪物获得；成长属性继承符可以在<color=0xff00ddbb>天界商人<color=0xffffffff>处购买。\\n    成长装备上的星星所表示的含义是：灰色为获得了基础值的<color=0xffff0000>50%<color=0xffffffff>，绿色是获得了基础值的<color=0xffff0000>80%<color=0xffffffff>，蓝色是获得了基础值的<color=0xffff0000>100%<color=0xffffffff>，紫色是获得了基础值的<color=0xffff0000>150%<color=0xffffffff>。");
end

local function OnScenarioTalk(id,step)


	if( id == 0 )then --装备成长
		Quest.OpenEquipGrow();
		Quest.CloseNPCTalk();
	end

end

EquipGrow = {}
EquipGrow["OnTalk"] = OnTalk
EquipGrow["OnScenarioTalk"] = OnScenarioTalk
