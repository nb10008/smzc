
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010085);
	Quest.SetCompleteOptions(5010085);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n若想镶嵌宝石，首先装备上得有镶嵌孔,1件装备最多可以开5个孔。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20323)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20323,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n怎么样，对宝石的了解已经差不多了吧。北洲天雪还是有很多问题要解决的，你还是尽快解决这些问题吧。<D>河神巴克</D>在<N>福临镇</N>的北边等着你呢。");
        elseif( id == 40031)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40031,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n欢迎欢迎！接下来是一堂华丽的宝石课程，你可要用心听！\\n<I>宝石</I>可以镶嵌在装备上，提升装备的属性。宝石有很多种，加的属性也不一样，不过说这么多也没用，因为首先你要弄到1颗<I>宝石原石</I>。\\n    <I>宝石原石</I>的获得途径也很多，比如击杀巨型魔物和魔神等，采矿时也有几率获得宝石原石，这次便宜你，你只要帮我杀死<C>30</C>个<A>黑心人贩</A>，我便赠送宝石原石给你。");
        elseif( id == 40032)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40032,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n拿到宝石原石，却发现无法镶嵌，是吧？因为你还缺少一道工序——<T>宝石切割</T>。想要切割的话，你得先拥有<I>割刀</I>，<I>割刀</I>可以在<D>杂货商</D>那里购买，这次为了方便你，你只要交给我<C>20</C>块<I>原矿</I>，我免费送你1把普通割刀。");
        elseif( id == 40033)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40033,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n拿到宝石了没？\\n光拿到宝石也没用，哈哈，我是不是很烦？若想镶嵌宝石，首先装备上得有<T>镶嵌孔</T>。有些装备初始就会有几个<T>镶嵌孔</T>，我们也可以通过<I>凿石</I>来扩充镶嵌孔数量，1件装备最多可以开<A>5</A>个孔。\\n<I>凿石</I>的获得途径则是来自<T>铸造技能</T>，而且只有<A>2</A>级以上的铸造技能才可生产凿石。所以……你再用<C>20</C>块<I>原矿</I>和我来交换初级凿石吧！\\n我可真是好心加热情……");
        elseif( id == 40034)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40034,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n整个的宝石课程做个总结，你不要嫌我烦……\\n    <T>1、不同装备，能镶嵌的宝石类型是不同的\\n    2、宝石也分了很多等级，你可以在宝石升级师处升级宝石，如果升级失败，会碎掉1颗\\n    3、宝石镶嵌上了，还能拆下来，具体的你就要找到宝石拆除师了\\n    4、宝石碎片的用处也很大，积攒了一定数量可以在宝石碎片兑换人处换各种成品宝石，另外制作高级饰品的时候也是需要宝石碎片的</T>\\n看你一脸痛苦，有没有耐心听我最后一句？");
	elseif( id == 20348)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20348,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n游戏这么久了，也该休息一下了。来点趣味问答吧~~那就开始了~~");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20322)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20322,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n居然都到达<A>40</A>级了，真是厉害，我给你带来了一些新的东西。");
        elseif( id == 40031)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40031,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n你还可以，这里有2种宝石原石，你选一种吧！");
        elseif( id == 40032)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40032,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n这把割刀你收好了，之后你便可以进行宝石切割，对宝石原石使用割刀即可。\\n需要注意的是，不同等级的宝石原石，所能进行切割的割刀不同。\\n至于你能切割出什么样的宝石，那全凭你运气了，若不小心切割失败，你只能得到1块<I>宝石碎片</I>");
        elseif( id == 40033)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40033,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n开凿也有几率失败，不过失败了只是白废一个凿石，不会对装备有其他负面影响。\\n镶嵌孔数量越多，越难以进行下一步的开凿，你自己努力吧！");
        elseif( id == 40034)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40034,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n告诉你个秘密吧，妖精也是可以镶嵌宝石的哦！");

       elseif( id == 20348)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+1,"冷心洞" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+2,"白龙洞" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+3,"武祖洞" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n问题一：小白突然间听不见声音，然后躲进一个山洞里，他给那个山洞取了个什么名字？");


		elseif( step == QuestTalkBegin+2 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+4,"蜘蛛有钱" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+5,"公幻蝶死了" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+6,"母幻蝶爱上网" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n问题二：母幻蝶没有嫁给公幻蝶，却嫁给了蜘蛛为什么？");

		elseif( step == QuestTalkBegin+6 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+7,"母幻蝶不爱蜘蛛了" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+8,"公幻蝶复活了" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+9,"她妈说整天在网上混的没有好人" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n问题三：为什么最终母幻蝶没有嫁给蜘蛛？");
			
		elseif( step == QuestTalkBegin+9 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+10,"麻雀" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+11,"白熊" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin+12,"雪地猪" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n问题四：你压谁会没有声？");

		elseif( step == (QuestTalkBegin+1))then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细思考下");
		elseif( step == (QuestTalkBegin+3))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细思考下");
		elseif( step == (QuestTalkBegin+4))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细思考下");
		elseif( step == (QuestTalkBegin+5))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细思考下");
		elseif( step == (QuestTalkBegin+7))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细思考下");
		elseif( step == (QuestTalkBegin+8))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细思考下");
		elseif( step == (QuestTalkBegin+11))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细思考下");
		elseif( step == (QuestTalkBegin+12))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20348,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细思考下");

		elseif( step == QuestTalkBegin+10 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(20348,"<font=龙字体,0,19,20><color=0xfffcf157>七彩之海谢拉：<color=0xffffffff><font=龙字体,0,17,20>\\n恩，休息够了，继续你的旅程吧。");
		end
       end

end

Rxiela = {}
Rxiela["OnTalk"] = OnTalk
Rxiela["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rxiela["OnCompleteQuestTalk"] = OnCompleteQuestTalk