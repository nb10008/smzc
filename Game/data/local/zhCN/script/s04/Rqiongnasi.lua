
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010076);
	Quest.SetCompleteOptions(5010076);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n看看这些魔族干的好事吧，这里都变成什么样子了。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20201)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20201,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n    这奔雷裂谷，曾经是一片绿意盎然，如今仅剩下这一块绿洲。因为魔族打开了“通路”，地形骤变，这里很多地方已经化为焦土。到绿洲去，收集一些树种给我吧，以防万一这最后的绿洲也……");
        elseif( id == 20202)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20202,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n    这里也有很多悬赏任务，当你通过这里的悬赏任务到达<C>22</C>级的时候，我会给你新的任务。");
        elseif( id == 20203)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20203,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n    有一件很紧急的事情需要你马上处理，你看看这附近被破坏的这么严重，都是因为那些可恶的石头，没错，就是那些石头，还有，顺便找到我的徒弟<D>纳德</D>，这小子又偷偷跑出去了。");
        elseif( id == 20206)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20206,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n    我的朋友<D>岩神奥利佛</D>在<N>普兰多</N>等着你呢，不过在此之前，你要把等级提升至<C>24</C>级。");
	elseif( id == 20219)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20219,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n    这里出现了很多裂谷魔灵，我需要你去帮我把它们消灭，别忘了带着魔灵之物回来。");
	elseif( id == 20222)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20222,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n    现在你要赶快去帮助夏尔消灭那些裂谷魔龙，然后把得到的龙心交给他。");
        elseif( id == 40042)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40042,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n    妖精在孵化的时候会需要大量的灵力，此时若是有人能为它们提供获取灵力的来源，便可以跟新生的妖精缔结契约，让其成为自己的伙伴。如果你能找来几样蕴含强大灵力的稀有物品，我就把与妖精缔结契约的方法告诉你。");
	elseif( id == 40043)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40043,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n    这契约之术可不是光靠看就能学会的。我知道一处有妖精出没的秘密洞穴，你去帮我弄个妖精精华回来，顺便练习一下这契约之术吧。");
        elseif( id == 40047)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40047,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n        了解一下便捷组队功能吧，这样可以很方便的帮助你组到队伍，完成任务参加活动或者打副本，都可以使用便捷组队功能来组成队伍。");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20200)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20200,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n        七花之海的事情我也听说了，看来斗神之王玛斯对你很期待啊。");
        elseif( id == 20201)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20201,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n        太好了，这是很宝贵的树种，要好好保存起来。");
        elseif( id == 20202)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20202,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n        看来你已经适应了这里的环境。");
        elseif( id == 20205)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20205,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n        这个是从黑岭找到的么，看来纳德说的没错，魔族也来到了这个地方了。");
	elseif( id == 20219)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20219,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n        看来你完成的不错，要继续努力。");
	elseif( id == 20221)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(20221,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n        绿谷的情况我已经知道了。");
	elseif( id == 40042)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40042,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n        这便是契约之术的手抄本，天神可不要小看它啊。");
        elseif( id == 40043)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40043,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff>\\n        其它地方的妖精灵力强大，可不是这么容易就能缔结契约的。你以后要多多收集契约道具，免得碰到妖精而无法得到哦。");
	elseif( id == 40047)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+1,"屏幕右上角（正确答案）" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+2,"屏幕右下角" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+3,"屏幕左上角" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff><font=龙字体,0,17,20>\\n问题一：便捷组队的按钮在屏幕的什么地方？");

		elseif( step == QuestTalkBegin+1 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+4,"组队模式" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+5,"个人模式" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+6,"等待模式（正确答案）" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff><font=龙字体,0,17,20>\\n问题二：哪个模式不是便捷组队平台的模式？");

		elseif( step == QuestTalkBegin+6 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+7,"在组队模式中，选中队伍申请加入（正确答案）" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+8,"在个人模式中，选中玩家邀请入队" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+9,"在活动/副本中，选择个人求组" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff><font=龙字体,0,17,20>\\n问题三：启动便捷组队如何加入已有队伍？");
			
		elseif( step == QuestTalkBegin+7 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+10,"希望玩家职业（正确答案）" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+11,"希望申请人的等级" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+12,"任务类型选择和场景选择" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff><font=龙字体,0,17,20>\\n问题四：个人模式中个人求组不需要添加什么信息？");
		
		elseif( step == QuestTalkBegin+10 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+13,"组队人数（正确答案）" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+14,"备注" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin+15,"怪物/活动/副本/其他" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff><font=龙字体,0,17,20>\\n问题五：组队模式中创建组队比个人模式中多了一个什么信息？");

		elseif( step == (QuestTalkBegin+2))then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+3))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+4))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+5))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+8))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+9))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+11))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin12))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+14))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+15))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,40047,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		

		elseif( step == QuestTalkBegin+13 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40047,"<font=龙字体,0,19,20><color=0xfffcf157>翼神琼纳斯：<color=0xffffffff><font=龙字体,0,17,20>\\n    看来你已经了解的差不多了。");
		end
       end

end

Rqiongnasi= {}
Rqiongnasi["OnTalk"] = OnTalk
Rqiongnasi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rqiongnasi["OnCompleteQuestTalk"] = OnCompleteQuestTalk