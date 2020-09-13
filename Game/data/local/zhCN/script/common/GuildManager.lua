local function OnTalk()
	Quest.ClearNPCTalkOptions()
	Quest.SetAcceptOptions(5010053)
	Quest.SetCompleteOptions(5010053)
	Quest.NPCTalk(0,0,"公会管事：您好，我这里可以接取公会任务。")
end

local function OnAcceptQuestTalk(id,step)
	if( id == 30001)then
        if (Quest.IsInGuild() == false) then
            return;
        end
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30001,"<font=龙字体,0,19,20><color=0xfffcf157>公会管事：\\n    <color=0xffffffff>要发展公会，资源自然是必不可缺的。英雄是否愿意为公会收集一些必要的资源？这可是为公会出力最为简单直接的办法。");
	elseif (id == 30002)then
        if (Quest.IsInGuild() == false) then
            return;
        end
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30002,"<font=龙字体,0,19,20><color=0xfffcf157>公会管事：\\n    <color=0xffffffff>要为公会作出更多贡献，首先就要让自己变得更强。请接受公会任务的考验以提升自己的实力！");
	elseif (id == 30003)then
        if (Quest.IsInGuild() == false) then
            return;
        end
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30003,"<font=龙字体,0,19,20><color=0xfffcf157>公会管事：\\n    <color=0xffffffff>我想是时候交给你一个具有挑战性的任务了，圣魔的世界上有许多号称以一当百的角色，公会需要你去将他们击败，一来可平息魔乱，二来可扬名于世。");
	end
end


local function OnCompleteQuestTalk(id,step)

	if( id == 30001)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30001,"<font=龙字体,0,19,20><color=0xfffcf157>公会管事：\\n    <color=0xffffffff>看来这个任务对你来说还是很轻松的。等你到达40级和60级，能在我这里领到更高级任务，那时请务必考虑为公会做更多贡献！");
	elseif (id == 30002)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30002,"<font=龙字体,0,19,20><color=0xfffcf157>公会管事：\\n    <color=0xffffffff>很高兴英雄依然在为公会和大家努力，公会的繁荣离不开你我的努力！");
	elseif (id == 30003)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30003,"<font=龙字体,0,19,20><color=0xfffcf157>公会管事：\\n    <color=0xffffffff>英雄这次可是立下大功，公会内外已经开始传颂你的事迹！");

	end
end

GuildManager = {}
GuildManager["OnTalk"] = OnTalk
GuildManager["OnAcceptQuestTalk"] = OnAcceptQuestTalk
GuildManager["OnCompleteQuestTalk"] = OnCompleteQuestTalk
