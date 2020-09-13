
local function OnTalk()

	Quest.ClearNPCTalkOptions();
	Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin, "蓝色装备兑换说明" );
	Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin, "橙色装备兑换说明" );
	Quest.SetNPCTalkOption( EOT_ScenarioTalk,2,ScenarioTalkBegin, "取消" );

	Quest.SetAcceptOptions(5011012);
	Quest.SetCompleteOptions(5011012);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n最近想要通过考验来到这里的新天神还真是多啊。");

end


local function OnScenarioTalk(id,step)
	if( id==0 ) then --如何进行耕种
		if( step == ScenarioTalkBegin ) then
			Quest.ClearNPCTalkOptions();                    --清除对话框选项
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+1, "40级兑换需求" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+2, "60级兑换需求" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+3, "80级兑换需求" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+4, "相关代币出处查询" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+5, "返回" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+10000, "离开" );
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>：\\n请选择要查询的信息：")
		elseif( step == ScenarioTalkBegin+1 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n  40级蓝色传说武器：勇者之证×4加8金\\n  40级蓝色传说防具：勇者之证×2加5金");
		elseif( step == ScenarioTalkBegin+2 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n  60级蓝色传说武器：王者之证×5加20金\\n  60级蓝色传说防具：王者之证×3加10金");
		elseif( step == ScenarioTalkBegin+3 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n  80级蓝色传说武器：霸者之证×10加50金\\n  80级蓝色传说防具：霸者之证×8加25金");
		elseif( step == ScenarioTalkBegin+4 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n  40级-55级：勇者之证\\n  55级-75级：王者之证\\n  75级以上：霸者之证\\n  百鬼炼狱：每次活动任务奖励1个“XX之证”\\n  勇闯魔窟：每次活动任务奖励1个“XX之证残页”\\n  三神传说：每次活动任务奖励3-5个“XX之证残页” \\n  知识竞猜：每次活动任务奖励3-5个“XX之证残页”");
		elseif( step == ScenarioTalkBegin+5 ) then --返回上级界面
			OnTalk();
		elseif( step == ScenarioTalkBegin+10000 ) then --关闭窗口
			Quest.CloseNPCTalk();
		end
	elseif( id==1 ) then
		if( step == ScenarioTalkBegin ) then
			Quest.ClearNPCTalkOptions();                    --清除对话框选项
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin+1, "40级兑换需求" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin+2, "60级兑换需求" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin+3, "80级兑换需求" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin+4, "相关代币出处查询" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin+5, "返回" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,1,ScenarioTalkBegin+10000, "离开" );
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n请选择要查询的信息：")
		elseif( step == ScenarioTalkBegin+1 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n  40级橙色传奇武器：40级蓝色传说装备+玄石×2+结晶×50\\n  40级橙色传奇防具：40级蓝色传说装备+玄石×1+结晶×20");
		elseif( step == ScenarioTalkBegin+2 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n  60级橙色伪魔神武器：60级蓝色传说武器+玄晶石×8+魔光碎片×6+神光碎片×12\\n  60级橙色伪魔神防具：60级蓝色传说防具+玄晶石×4+魔光碎片×3+神光碎片×6");
		elseif( step == ScenarioTalkBegin+3 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n  80级橙色伪魔神武器：80级蓝色传说武器+破魔石×10+魔光核×12+神光核×12\\n  80级橙色伪魔神防具：80级蓝色传说防具+破魔石×5+魔光核×6+神光核×6");
		elseif( step == ScenarioTalkBegin+4 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n  40级-55级：玄石\\n  55级-75级：玄晶石\\n  75级以上：破魔石\\n  百鬼炼狱：每次活动任务奖励1个“XX之证”\\n  副本、野外BOSS、百鬼炼狱、勇闯魔窟、魔族入侵的活动怪物均有几率掉落");
		elseif( step == ScenarioTalkBegin+5 ) then --返回上级界面
			OnTalk();
		elseif( step == ScenarioTalkBegin+10000 ) then --关闭窗口
			Quest.CloseNPCTalk();
		end
	elseif( id==2 ) then
		Quest.CloseNPCTalk();
	end
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 40045)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40045,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n啧啧啧，过来看看这个。知道什么叫做传说吗，当然就是传说了，在圣魔的世界里有一种传说的装备，其实想要获得的话也不难，首先你要拥有勇者之证，没错就是勇者之证！不过我要确定下你是不是有资格获得勇者之证，通过这个考验我就承认你是可以获得传说装备的勇者。");
        elseif( id == 40046)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40046,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n啧啧啧，过来看看这个。知道什么叫做传说吗，当然就是传说了，在圣魔的世界里有一种传说的装备，其实想要获得的话也不难，首先你要拥有玄石，没错就是玄石！不过我要确定下你是不是有资格获得玄石，通过这个考验我就承认你有获得传说装备的资格。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 40045)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40045,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n看来你确实是个勇者，有资格获得勇者之证和那些传说装备，这个勇者之证你拿去吧。");
        elseif( id == 40046)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40046,"<font=龙字体,0,19,20><color=0xfffcf157>传说装备使者：<color=0xffffffff>\\n看来你确实有资格获得那些传说装备，这个玄石你拿去吧。");
       end

end

RchuanshuoZB = {}
RchuanshuoZB["OnTalk"] = OnTalk
RchuanshuoZB["OnScenarioTalk"] = OnScenarioTalk
RchuanshuoZB["OnAcceptQuestTalk"] = OnAcceptQuestTalk
RchuanshuoZB["OnCompleteQuestTalk"] = OnCompleteQuestTalk
