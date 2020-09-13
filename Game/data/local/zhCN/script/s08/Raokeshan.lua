
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010205);
	Quest.SetCompleteOptions(5010205);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n小朋友！智慧的力量才是最大的！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20601)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20601,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n曾经的一切都不再重要，你只需知道天神与魔族都切莫轻信！无辜的人类，为何要让他们受苦？看那些狂暴的<A>驯熊者</A>，曾经也只是善良的猎人，超度他们吧，哎！对了，小心那些<A>熊</A>！");
        elseif( id == 20602)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20602,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n喝下神圣之水的人类都是这般模样，更有体质特异者，变得极为狂暴，了结他们悲惨的命运吧！");
        elseif( id == 20603)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20603,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n<D>幻神</D>的所作所为你很清楚，那么，你猜她怎么回答我的质问？她说这都是为了天神一族！");
	elseif( id == 20604)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20604,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n<N>青岭沟</N>的路被狂风贼阻挡，看他们神情呆滞的样子，必是中了某种邪术，从他们身上采集些血样来，咱们一看便知。");	
	elseif( id == 20605)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20605,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n<color=0xffff6000>狂风精英劫匪<color=0xffffffff>似乎尚有神智，快！先找来<C>30</C>名，我试试还能否让他们还魂。");
        elseif( id == 20606)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20606,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n点燃<I>烟花</I>，以此祭奠游走在此的亡灵们！");
	elseif( id == 20607)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20607,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n这块<I>墓碑</I>上刻有我身为神族对无辜枉死者深深的愧疚，竖立在我身边的土地上就可以了。");
	elseif( id == 20608)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20608,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n好了，收起哀痛吧！<N>天兆谷</N>中遍地是魔族种的<A>魔花</A>，不要以为魔族为人豪爽就不奸诈，这些魔花不也是为了争夺人间的领地？去消灭它们！一个不留！");
        elseif( id == 20609)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20609,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n听说<A>原神教教众</A>又在附近惹是生非，那边<N>山隐秘窟</N>中最近总有奇怪的声响传出，去帮我看看。");
	elseif( id == 20610)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20610,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n去<N>秘窟</N>深处看看，如果有<A>原神教徒</A>，把他们身上的<I>控魂纹章</I>收回来，这本应是天神保管的禁物！顺便灭了他们的头目，否则还有会更多的亡灵变异。");	
	elseif( id == 20611)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20611,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n<N>东岭村</N>里有位主张和平的魔族，他那边也遇到了不少麻烦，去看看吧，或许他那里有你想知道的！");
        elseif( id == 20661)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20661,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n你需要向上升至82级。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20600)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20600,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n斗神之王让你来找我？呵呵，看来他还以为我是效忠于他的！也好，就让我告诉你真相中的真相吧！");
        elseif( id == 20601)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20601,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n看到了？肮脏的权利欲把他们的心都遮蔽了。你已经猜到他们是谁了？");
	elseif( id == 20602)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20602,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n看来你除了正义感也具备一些慈悲心肠，既然这样，我也可以放心告诉你一些事了。");
	elseif( id == 20603)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20603,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n他们这是何苦呢？天界已经很好了，他们为什么还不知足？还要霸占这人间……很抱歉，我只能保证自己依旧清白……");
        elseif( id == 20604)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20604,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n果然，又是原神教干的好事，这群该……哎！他们其实也是受害者啊！");
	elseif( id == 20605)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20605,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n看来是无药可救了，安心的去吧，用不了多久便会有神陨落为你们陪葬，所以，冤屈的亡灵们！还请暂平怒火！");
	elseif( id == 20606)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20606,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n恩！很好，我已经感觉到周遭的亡灵不再如刚才那般狂躁了！");
	elseif( id == 20607)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20607,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n让我们一起为他们默默祈福吧……");
	elseif( id == 20608)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20608,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n好好好！至少这片地区能稍微干净些了。");
	elseif( id == 20609)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20609,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n怎么？竟然有变异的亡灵？没想到原神教已经掌握了这种神术！");
	elseif( id == 20610)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20610,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n有个实力很强的杀手叫米雪？那不是斗神之王的婢女吗？难道……");
	elseif( id == 20661)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20661,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n恭喜你升到了82级。");
  elseif( id == 30346)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30346,"<font=龙字体,0,19,20><color=0xfffcf157>奥克山：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");

       end

end

Raokeshan = {}
Raokeshan["OnTalk"] = OnTalk
Raokeshan["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Raokeshan["OnCompleteQuestTalk"] = OnCompleteQuestTalk