
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510502);
	Quest.NPCTalk(0,0,"魔窟使者：看来是新来的天兵，在这里好好接受训练吧。");
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30301)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30301,"魔窟使者：若想证明可完成勇创魔窟活动，你需要击杀魔窟首领，魔窟首领之后在所有的怪物后才会出现，你是否愿意接受这个任务？");
	end
end

RMoKuShiZhe = {}
RMoKuShiZhe["OnTalk"] = OnTalk
RMoKuShiZhe["OnAcceptQuestTalk"] = OnAcceptQuestTalk
