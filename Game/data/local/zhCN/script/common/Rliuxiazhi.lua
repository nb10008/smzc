 
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(3011014);			
	Quest.SetCompleteOptions(3011014);
	if ( Quest.QuestIsCompleted(68) ) then
		Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin, "帮派介绍" );
		if ( Quest.GetPlayerLevel()<30 ) then
			Quest.NPCTalk(0,0,"柳下跖：\\n    尽早加入帮派对初入江湖之人的帮助不可估量，少侠还需早做决断。\\n    若对帮派之事有任何不解之处，尽可来找老夫询问。老夫必知无不言，言无不尽。");
		else
			Quest.NPCTalk(0,0,"柳下跖：\\n    盗亦有道乎？何适而无有道邪！夫妄意室中之藏，圣也！入先，勇也！出后，义也！知可否，知也！分均，仁也！");
		end
	else
		Quest.NPCTalk(0,0,"柳下跖：\\n    盗亦有道乎？何适而无有道邪！夫妄意室中之藏，圣也！入先，勇也！出后，义也！知可否，知也！分均，仁也。");	
	end
end 

local function OnScenarioTalk(id,step)
	if( id==0 ) then --帮派介绍
		if( step == ScenarioTalkBegin ) then
			Quest.ClearNPCTalkOptions();                    --清除对话框选项
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+1, "创建与解散" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+2, "加入帮派" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+3, "帮派任务" );
			Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+4, "帮派职务" );
			Quest.NPCTalk(0,0,"柳下跖：\\n    不知你想了解哪方面的内容？");

		elseif( step == ScenarioTalkBegin+1 ) then --创建与解散
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+10000,"确定" );
		       Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin,"返回" );
	               Quest.NPCTalk(0,0,"柳下跖：\\n    普通玩家等级达到<A>20</A>级后即可在<N>帮派官员</N>处选择创建自己的帮派，创建帮派需花费<A>10</A>金。\\n    当帮派中除帮主外再无其他任何帮派成员、且帮派不处于<M>征战</M>状态时，帮主可在<N>帮派官员</N>处选择解散帮派，解散帮派没有任何额外花费，但原有帮派属性会被<C>清空</C>。\\n    <N>帮派官员</N>在各大<P>主城</P>中均有设立。");

		elseif( step == ScenarioTalkBegin+2 ) then --加入帮派
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+10000,"确定" );
		       Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin,"返回" );
	               Quest.NPCTalk(0,0,"柳下跖：\\n    普通玩家无法主动申请加入帮派，若想加入帮派需要帮派中有相关权限的成员发出<D>邀请</D>方可。在帮派中，<N>帮主</N>、<N>副帮主</N>、<N>各堂堂主</N>、<N>各堂香主</N>均有权限邀请其他玩家加入帮派。\\n    在帮派中，除<N>帮主</N>外的其他帮派成员随时都可在<S>帮派界面</S>中选择脱离帮派，帮派成员脱离帮派后除<T>帮派技能</T>和<T>帮派阅历</T>外的其他所有帮派属性都会被清空。<N>帮主</N>若要脱离帮派需首先将帮主职位<D>移交</D>给其他帮派成员，当帮派不处于<T>征战</T>状态时即可通过<S>帮派成员列表界面</S>移交帮主职位。");		 

		elseif( step == ScenarioTalkBegin+3 ) then --帮派任务
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+10000,"确定" );
		       Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin,"返回" );
	               Quest.NPCTalk(0,0,"柳下跖：\\n    帮派任务主要分为<I>帮派历练</I>、<I>筹集资材</I>、<I>维护安定</I>。\\n    <I>帮派历练</I>：所有帮派成员均可接取的基本帮派任务，完成后可获得经验与帮派贡献。帮派成员在接取帮派历练任务时会根据帮派贡献度的不同随机遇到不同等级的任务，分别为<T>普通</T>、<T>名师指点</T>、<T>醍醐灌顶</T>、<T>仙人指路</T>，越高级别的帮派历练任务获得奖励越丰富，<S>帮派贡献</S>度越高的帮派成员接取到高等级任务的几率也会越高。\\n    <I>筹集资材</I>：只有帮主、副帮主及<C>青龙堂</C>成员可接取的帮派任务，完成获得经验与帮派贡献度的奖励，同时可为帮派提升<S>帮派资材</S>。该任务每<A>10</A>环为<A>1</A>轮，在完成每轮的第<A>5</A>环、第<A>8</A>环及第<A>10</A>环任务后将会获得<I>帮派功劳贴</I>，帮派成员持帮派功劳贴可在黄帝城<N>墨翟</N>处兑换高级道具。\\n    <I>维护安定</I>：只有帮主、副帮主及<C>白虎堂</C>成员可接取，完成后获得经验与帮派贡献度的奖励，同时可提升帮派的<S>安定度</S>。该任务每<A>10</A>环为<A>1</A>轮，在完成每轮的第<A>5</A>环、第<A>8</A>环及第<A>10</A>环任务后将会获得<I>帮派功劳贴</I>，帮派成员持帮派功劳贴可在黄帝城<N>墨翟</N>处兑换高级道具。\\n    所有帮派任务均在<N>帮派管事</N>处接取，帮派管事在各大主城均有设立。");

		elseif( step == ScenarioTalkBegin+4 ) then --帮派职务
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin+10000,"确定" );
		       Quest.SetNPCTalkOption( EOT_ScenarioTalk,0,ScenarioTalkBegin,"返回" );
	               Quest.NPCTalk(0,0,"柳下跖：\\n    最初创建帮派的玩家会自动成为帮派的第一任帮主，每个帮派仅有<A>1</A>名<C>帮主</C>，自帮主之下分别为：副帮主<A>3</A>名；红衣护法及紫杉护法各<A>1</A>名；青龙堂堂主、香主各<A>1</A>名；白虎堂堂主、香主各<A>1</A>名；朱雀堂堂主、香主各<A>1</A>名；玄武堂堂主、香主各<A>1</A>名；四堂帮众、精英帮众及普通帮众若干。其中四堂帮众、精英帮众及普通帮众的数量取决于<C>帮派等级</C>，普通玩家加入帮派后默认为普通帮众，而后由拥有相关权限的帮派成员为其提升帮派职务。\\n    四堂之中，<C>青龙堂</C>成员可在帮派管事处接取帮派任务<T>筹集资材</T>；<C>白虎堂</C>成员可在帮派管事处接取帮派任务<T>维护安定</T>；<C>朱雀堂</C>成员可在帮派管事处进行帮派<T>跑商</T>；<C>玄武堂</C>成员可在帮派管事处进行<T>升级设施</T>；\\n    除此之外，帮主与副帮主还可在帮派管事处<T>发布帮务</T>，帮务发布后可为全体在线的帮派成员添加一定时间的增益状态。");

		elseif( step == ScenarioTalkBegin+10000 ) then --关闭窗口
			Quest.CloseNPCTalk();
                end
	end
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 68)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(68,"柳下跖：\\n    日后你行走江湖之时，在<S>各大场景主城之中</S>均可看到一名<N>帮派官员</N>，所有等级达到<A>20</A>级的人皆可在<N>帮派官员</N>处花费<A>10</A>金<S>创建自己的帮派</S>，而<D>已有帮派的帮主</D>则可在<N>帮派官员</N>处选择<S>解散自己的帮派</S>。\\n    若想要加入别人的帮派，则<S>不论等级</S>多少，只要有该<D>帮派中拥有相应权限的成员</D>邀请，便可加入该帮派。\\n    \\n    加入帮派后，可在<N>帮派管事</N>处参与<S>帮派日常事务</S>，如帮主发布帮务、帮派成员进行帮派任务、参与帮派建设等等。\\n    <N>帮派管事</N>与<N>帮派官员</N>一样，在各大主城均有设立，玄天龙城的<N>帮派管事</N>与<N>帮派官员</N>就在我旁边不远处，你可先去与他们打个招呼、混个脸熟，也好方便日后行事。");
        elseif( id == 69)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(69,"柳下跖：\\n    帮派之中极重长幼尊卑，最忌以下犯上之事，故而初入帮门，需得先把这帮门中的上下关节弄个明白才行。\\n    在帮派之中，自然是帮主最大，帮主之下为副帮主，再下便是青龙、朱雀、玄武、白虎四堂，这四堂各有司职，分管帮派中不同事宜。其中：\\n    <S>青龙堂</S>负责帮派技能的研究及筹集帮派资材，青龙堂成员均可在<N>帮派管事</N>处接取任务<T>筹集资材</T>；\\n    <S>朱雀堂</S>负责筹措帮派资金，朱雀堂成员均可在<N>帮派管事</N>处领取商银，进行<T>帮派跑商</T>；\\n    <S>玄武堂</S>负责建设帮派设施，玄武堂成员均可在<N>帮派管事</N>处上缴建设帮派设施所需的材料，为帮派<T>升级设施</T>；\\n    <S>白虎堂</S>负责维护帮派安定，白虎堂成员均可在<N>帮派管事</N>处接取任务<T>维护安定</T>。\\n    在帮派之中，只有各堂的堂主和香主才有权限从普通帮众与精英帮众中遴选各堂成员，此事即便是帮主本人亦无权插手。\\n    说了这么多，你还是去<N>帮派管事</N>那里转转，与他聊上几句混个脸熟，以后也方便行事。");
        elseif( id == 70)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(70,"柳下跖：\\n    各帮派中均有不同的技能帮助帮派成员修行，不过这帮派技能却不是凭空得来的，需要帮派慢慢研究才行。<N>帮主</N>、<N>副帮主</N>及<N>青龙堂堂主</N>可在<S>帮派界面</S>中选择当前帮派所要研究的技能，所要研究的技能选定后，该技能便会每半个时辰自动消耗一定的<S>帮派资金</S>与<S>帮派资材</S>来提升研究进度，当研究进度满后，便会自动提升一级。另外青龙堂成员也可通过提交<I>典籍</I>来获得帮派技能研究进度的额外提升。\\n    帮派技能的研究级别上限与<S>帮派书院的等级</S>息息相关，而其日常研究的快慢则直接受到<S>帮派安定度</S>的影响，所以只有白虎堂成员将帮派安定度维持在一个较高水平的时候，帮派技能的日常研究进度才能被加快。\\n    所有帮派中的成员都可通过消耗一定的<T>金钱</T>与<T>帮派贡献度</T>来学习帮派中已经研究出的技能，帮派贡献度则可通过完成帮派任务获得。\\n    好了，关于帮派事务，老夫该说的也都说的差不多了，如今倒该考校你一番了，你可准备好了？");
        end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
     if( id == 67)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(67,"柳下跖：\\n    <N>盘古</N>让你来的？那老夫便提点你一番吧。\\n    自古一人力短十人力长，行走江湖之人，若无帮派相依，终难成大器。说起这加入帮派的方法，也是各有不同。若你对自己颇有信心，则可招兵买马自立门户，<S>建立自己的帮派</S>。若不成，做一只泽木而栖的良禽，<S>加入他人帮派</S>也未尝不可。\\n    详情如何，且待我详细道来。"); 
     elseif( id == 68)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(68,"柳下跖：\\n    各帮派中均有不同的技能帮助帮派成员修行，不过这帮派技能却不是凭空得来的，需要帮派慢慢研究才行。<N>帮主</N>、<N>副帮主</N>及<N>青龙堂堂主</N>可在<S>帮派界面</S>中选择当前帮派所要研究的技能，所要研究的技能选定后，该技能便会每半个时辰自动消耗一定的<S>帮派资金</S>与<S>帮派资材</S>来提升研究进度，当研究进度满后，便会自动提升一级。另外青龙堂成员也可通过提交<I>典籍</I>来获得帮派技能研究进度的额外提升。\\n    帮派技能的研究级别上限与<S>帮派书院的等级</S>息息相关，而其日常研究的快慢则直接受到<S>帮派安定度</S>的影响，所以只有白虎堂成员将帮派安定度维持在一个较高水平的时候，帮派技能的日常研究进度才能被加快。\\n    所有帮派中的成员都可通过消耗一定的<S>金钱</S>与<S>帮派贡献度</S>来学习帮派中已经研究出的技能，帮派贡献度则可通过完成帮派任务获得。\\n    有关帮派之事千头万绪，实非三言两语可以说清，一方面需要少侠在日后行走江湖之时多多自行摸索，另外若有任何不解之处，可随时来向我询问。"); 
     elseif( id == 69)then
              Quest.CloseNPCTalk();	
	      Quest.CompleteQuestTalk(69,"柳下跖：\\n    很好，如今你已算入门了。"); 
     elseif ( id == 70 )then

		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+1,"一、帮派官员" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+2,"二、帮派管事" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+3,"三、司马龙季" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+4,"四、柳下跖" );
			Quest.NPCTalk(0,0,"柳下跖：\\n    你听好，若要建立或解散帮会需找谁办理相关事项？");
		elseif( step == QuestTalkBegin+2 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+3 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+4 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+1 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+5,"一、青龙堂" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+6,"二、白虎堂" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+7,"三、朱雀堂" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+8,"四、玄武堂" );
			Quest.NPCTalk(0,0,"柳下跖：\\n    帮门四堂之中，哪一堂负责维护帮派安定？");
		elseif( step == QuestTalkBegin+5 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+7 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+8 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+6 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+9,"一、通过帮派成员募捐" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10,"二、通过完成帮派任务获得" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+11,"三、用元宝换取" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+12,"四、通过朱雀堂成员跑商获得" );
			Quest.NPCTalk(0,0,"柳下跖：\\n    帮派资金可通过什么方式获得？");
		elseif( step == QuestTalkBegin+9 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+10 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+11 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+12 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+13,"一、帮派资金" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+14,"二、帮派资材" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+15,"三、帮派安定" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+16,"四、帮派贡献" );
			Quest.NPCTalk(0,0,"柳下跖：\\n    帮派中的何种属性有助于提升帮派技能的日常研究进度？");
		elseif( step == QuestTalkBegin+13 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+14 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+16 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+15 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+17,"一、帮派资金" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+18,"二、帮派资材" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+19,"三、天资点数" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+20,"四、帮派贡献" );
			Quest.NPCTalk(0,0,"柳下跖：\\n    帮派成员若要学习帮派技能，下列哪种属性不会被消耗？");
		elseif( step == QuestTalkBegin+17 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+18 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
		elseif( step == QuestTalkBegin+20 )then
	               Quest.ClearNPCTalkOptions();
		       Quest.SetNPCTalkOption( EOT_CompleteQuest,70,QuestTalkBegin+10000,"确定" );
	               Quest.NPCTalk(0,0,"您答错了，请再仔细想一想。");
                elseif( step == QuestTalkBegin+19 )then
	                Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(70,"柳下跖：\\n    很好，看来你已掌握了帮门生活的基本法则，不过帮门之事博大精深，还需你在日后修行中自行多多摸索才好。");
		elseif( step == QuestTalkBegin+10000 ) then --关闭窗口
			Quest.CloseNPCTalk();
                end
     end

end
Rliuxiazhi = {}
Rliuxiazhi["OnTalk"] = OnTalk
Rliuxiazhi["OnScenarioTalk"] = OnScenarioTalk
Rliuxiazhi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rliuxiazhi["OnCompleteQuestTalk"] = OnCompleteQuestTalk