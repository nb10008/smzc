
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510627);
	Quest.SetCompleteOptions(5510627);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n你每天都可以在我这里领取一次日常任务，会有丰厚的奖励和回报！如果今天已完成，记得明天来找我。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 50010)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50010,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n你只要完成<C>2</C>次<T>悬赏任务</T>，便可以在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
        elseif( id == 50011)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50011,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>20</C>级开始，我对你的要求提升为完成<C>5</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
        elseif( id == 50012)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50012,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>20</C>级开始，我对你的要求提升为完成<C>5</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50013)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50013,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>30</C>级开始，我对你的要求提升为完成<C>6</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50014)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50014,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>30</C>级开始，我对你的要求提升为完成<C>6</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50015)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50015,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>40</C>级开始，我对你的要求提升为完成<C>8</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50016)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50016,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>40</C>级开始，我对你的要求提升为完成<C>8</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50017)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50017,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>50</C>级开始，我对你的要求提升为完成<C>10</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50018)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50018,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>50</C>级开始，我对你的要求提升为完成<C>10</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50019)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50019,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>60</C>级开始，我对你的要求提升为完成<C>15</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50020)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50020,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>60</C>级开始，我对你的要求提升为完成<C>15</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50021)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50021,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>60</C>级开始，我对你的要求提升为完成<C>15</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50022)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50022,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n自<C>60</C>级开始，我对你的要求提升为完成<C>15</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50023)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50023,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n<C>80</C>级以后，需要你完成<C>20</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50024)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50024,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n<C>80</C>级以后，需要你完成<C>20</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50025)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50025,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n<C>80</C>级以后，需要你完成<C>20</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50026)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50026,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>悬赏任务</T>为天神发布的各种形式的任务，随着你的等级变化，你可接取到不同条件的悬赏任务。\\n<T>悬赏任务</T>需要在各地的<D>悬赏榜</D>处进行接取和交付，每天不限制完成次数，可有效地提升升级的效率\\n<C>80</C>级以后，需要你完成<C>20</C>次<T>悬赏任务</T>，达成后可在我这里领取丰厚的奖励\\n \\n<T>该任务每日可完成1次</T>");
	elseif( id == 50035)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(50035,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<T>天之行会</T>是个特殊的行会，经常帮助人界处理一些棘手的问题。如果想要得到天之行会的任务，就需要这个<T>天之徽记</T>，祝你好运吧。");
  elseif( id == 30717)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30717,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n在<C>29</C>级之前，你每天可以从我这里领取一个前往<N>滚石洞穴</N>的任务。由于任务的难度较高，你需要找一个<C>50</C>级以上的玩家和你一同前往。");
  elseif( id == 30718)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30718,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n在<C>49</C>级之前，你每天可以从我这里领取一个前往<N>碎石洞穴</N>的任务。由于任务的难度较高，你需要找一个<C>50</C>级以上的玩家和你一同前往。");
  elseif( id == 30719)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30719,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n在<C>50</C>级之后，你每天可以从我这里领取一个前往<N>滚石洞穴</N>的任务。这个任务虽然并不困难，但是需要你和一个等级在<C>10</C>至<C>29</C>级之间的玩家合作完成。祝你们合作愉快！");
  elseif( id == 30720)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30720,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n在<C>50</C>级之后，你每天可以从我这里领取一个前往<N>碎石洞穴</N>的任务。这个任务虽然并不困难，但是需要你和一个等级在<C>30</C>至<C>49</C>级之间的玩家合作完成。祝你们合作愉快！");
  elseif( id == 30721)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30721,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n每个人都是从菜鸟成长为高手的，在圣魔之血的世界中也是这样。作为一个高等级的玩家，你不妨去帮帮那些低等级的玩家，我相信好心总会有好报的。");
  elseif( id == 30722)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30722,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n每个人都是从菜鸟成长为高手的，在圣魔之血的世界中也是这样。作为一个高等级的玩家，你不妨去帮帮那些低等级的玩家，我相信好心总会有好报的。");

	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 50010)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50010,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
        elseif( id == 50011)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50011,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
        elseif( id == 50012)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50012,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50013)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50013,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50014)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50014,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50015)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50015,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50016)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50016,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50017)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50017,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50018)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50018,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50019)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50019,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50020)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50020,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50021)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50021,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50022)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50022,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50023)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50023,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50024)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50024,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50025)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50025,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50026)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50026,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n看来你十分有干劲！请收下奖励，<T>别忘记明日再来找我</T>。随着你等级的提升，我对你的要求会越来越高，同时奖励也会越来越丰富");
	elseif( id == 50035)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(50035,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n<I>天之徽记</I>每天可领取<A>5</A>个，不过<T>每天可在天之行会任务榜接取10次任务</T>，希望你能找到更多的<I>天之徽记</I>。");
	elseif( id == 30717)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30717,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n你是否已经把你的队友们加为好友了呢？如果你们的合作还算愉快的话以后就跟他保持联系吧。");
	elseif( id == 30718)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30718,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n你是否已经把你的队友们加为好友了呢？如果你们的合作还算愉快的话以后就跟他保持联系吧。");
	elseif( id == 30719)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30719,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n我相信这个奖励会让你满意的。");
	elseif( id == 30720)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30720,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n我相信这个奖励会让你满意的。");
	elseif( id == 30721)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30721,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n这个任务真的很简单，不是吗？");
	elseif( id == 30722)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30722,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n这个任务真的很简单，不是吗？");
  elseif( id == 30724)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30724,"<font=龙字体,0,19,20><color=0xfffcf157>悬赏使者：<color=0xffffffff>\\n这个任务真的很简单，不是吗？");
	end

end

Rxuanshangshizhe = {}
Rxuanshangshizhe["OnTalk"] = OnTalk
Rxuanshangshizhe["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxuanshangshizhe["OnCompleteQuestTalk"] = OnCompleteQuestTalk
