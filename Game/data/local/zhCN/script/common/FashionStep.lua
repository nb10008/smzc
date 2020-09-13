local function OnTalk()
		Quest.ClearNPCTalkOptions();                    --清除对话框选项				--设置对话选项
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,0,0,"进行时装精纺");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,1,0,"什么是时装精纺");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,2,0,"如何进行时装精纺");
		Quest.SetNPCTalkOption(EOT_ScenarioTalk,3,0,"时装精纺套装属性");
		Quest.NPCTalk(0,0,"时装精纺师：我是全世界最好的裁缝。");
end

local function OnScenarioTalk(id,step)
	if( id == 0 )then
		Quest.FashionStep();
	elseif( id == 1 )then --解散公会

		Quest.NPCTalk(0,0,"时装精纺师：\\n    1.游戏中获得的<color=0xffff6000>永久制<color=0xffffffff>时装都可进行时装精纺，部分特殊类型时装除外（如：<color=0xff00a2ff>新手时装、虎头帽<color=0xffffffff>等）；\\n    2.时装精纺可将时装的基础属性和装备等级提升，并附加新的套装属性；\\n    3.单件时装目前最多可进行<color=0xff05ff00>初级、中级、高级<color=0xffffffff>，三次精纺，时装在进行精纺后，精纺等级无法降低；\\n    4.进行<color=0xff05ff00>初级精纺<color=0xffffffff>后的时装，角色等级必须达到<color=0xffff402d>30<color=0xffffffff>级才可以使用；\\n    5.进行<color=0xff00a2ff>中级精纺<color=0xffffffff>后的时装，角色等级必须达到<color=0xffff402d>60<color=0xffffffff>级才可以使用；\\n    6.进行<color=0xffffae00>高级精纺<color=0xffffffff>后的时装，角色等级必须达到<color=0xffff402d>90<color=0xffffffff>级才可以使用。");
	elseif( id == 2 )then --解散公会

		Quest.NPCTalk(0,0,"时装精纺师：\\n    1. 时装精纺需要材料<color=0xffffae00>金丝线<color=0xffffffff>，该材料可通过部分副本及活动获得，也可在商城中购买；\\n    2. 当您有<color=0xffffae00>金丝线<color=0xffffffff>时，可在龙城及黄帝城NPC <color=0xff00a2ff>时装精纺人<color=0xffffffff>处进行精纺；\\n    3.精纺的成功率会随着精纺时放入<color=0xffffae00>金丝线<color=0xffffffff>数量的增加而增加；\\n    4. 精纺时装时，放入精纺所需的材料后，您可以再通过材料图标两旁的<color=0xffffae00> “+”“-” <color=0xffffffff>按钮调整材料数量；\\n    5. 若要精纺成功率为100%，则初级时装精纺所需<color=0xffffae00>金丝线<color=0xffffffff>数量大约为10个，中级时装精纺所需<color=0xffffae00>金丝线<color=0xffffffff>数量大约为30个，高级时装精纺所需<color=0xffffae00>金丝线<color=0xffffffff>数量大约为60个。\\n    6.未绑定的时装在精纺后会变为<color=0xffffae00>绑定<color=0xffffffff>时装，不可对其进行<color=0xffff00ff>出售、交易<color=0xffffffff>等行为；");
	elseif( id == 3 )then --解散公会

		Quest.NPCTalk(0,0,"时装精纺师：\\n    1.每次进行时装精纺时，可选择<color=0xff9933ff>勇猛之心、刚毅之躯、辉煌之气、纯洁之灵<color=0xffffffff>四类套装属性中的一种，附加于精纺后的时装上。\\n    2.这四种套装属性具有不同的属性偏向，具体如下：\\n       勇猛之心（外功伤害偏向）\\n       刚毅之躯（外功防御偏向）\\n       辉煌之气（内功伤害偏向）\\n       纯洁之灵（内功防御偏向）\\n    3.若同时装备3件、5件、6件套装属性相同的时装，对应的套装属性会被激活；\\n    4.当集齐<color=0xffff0000>6<color=0xffffffff>件套装属性和精纺等级相同的时装，则会出现绚丽的套装特效；\\n    5.不同的时装部件，若有相同的套装属性，套装属性也可以被激活。");
	end

end

FashionStep = {}
FashionStep["OnTalk"] = OnTalk
FashionStep["OnScenarioTalk"] = OnScenarioTalk
