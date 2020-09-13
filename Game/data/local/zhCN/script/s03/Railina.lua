
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010031);
	Quest.SetCompleteOptions(5010031);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n七花之海曾经是那么美丽，如今却被魔族破坏成这样，真是可惜。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20100)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20100,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n在坦伯特，你还是可以看到七花之海的美丽，但那些生物却已经被魔物侵蚀了。我需要一些材料做出抵御魔族的药水，那些<A>幻蝶</A>就是最好的药粉。");
        elseif( id == 20101)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20101,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n拿着这个药水，在这附近你可以找到<I>花海之根</I>，将这些药水倒在上面，动作要快。");
        elseif( id == 20102)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20102,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n接下来，你要通过悬赏任务将等级提升至<C>12</C>级，应该十分快的，以你现在的劲头来说。");
        elseif( id == 20103)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20103,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n我的助手<D>卡洛儿</D>在<N>喀布托林</N>，她正在那里调查狂暴的<A>幼食人龙</A>，快去帮帮她吧。");
        elseif( id == 20106)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20106,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n从这里一直往东走，你就可以到<N>萨斯林</N>。<D>法兰斯</D>是我最优秀的助手，我已经先行派他去调查一番了，先别急，等你将你的等级提升到<C>14</C>级，再到那里去找他吧。");
        elseif( id == 20109)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20109,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n现在你需要将你的等级提升至<C>16</C>级，然后就可以去拜见<D>野人的领袖</D>，太弱的人可得不到他们的认可。");
        elseif( id == 20114)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20114,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n七花之海基本已经平安了，还有最后一事，等你到达<C>18</C>级的时候再来找我，我会将此事告诉你。");
        elseif( id == 20115)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20115,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n<A>拉斯</A>确实是个恶魔，但是属于七十二魔子中较弱的那个。想对七花之海造成这样的影响，靠他是不太可能的。\\n<color=0xffff0000>维特拉<color=0xffffffff>，这个恶魔也在七花之海，因为拉斯的死让我感觉到了<color=0xffff0000>维特拉<color=0xffffffff>的存在，我们必须除掉那个恶魔。");
        elseif( id == 20116)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20116,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n刚才斗战天城传来消息，希望你试炼结束后速速回城，不过要等你等级提升至<C>20</C>级才可以。");
        elseif( id == 40001)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40001,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n七花之海，原本是天神的后花园。这里居住着三界之外的另一种生灵——<T>妖精</T>。<T>妖精</T>是圣魔之血相融的产物之一，可与天神或魔族签定契约，之后便终身与契主相伴。在你成为斗神的路上，你需要<T>妖精</T>的协助。\\n      只要按我说的做，我会赠送<T>妖精</T>给你。首先要帮我收集<C>2</C>朵<I>血之妖娆</I>，同时再从<A>迷蝶</A>身上获取<C>8</C>份<I>幻迷之粉</I>。");
        elseif( id == 40002)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40002,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n我首先向你说明<T>妖精</T>的基础功能，要注意几点：\\n    <N>一、妖精必须携带以后才会增加契主能力</N>\\n    <A>二、同时只可携带1名妖精</A>\\n    <D>三、可以在角色属性界面的妖精页中查看所携带的妖精的具体属性</D>\\n    <T>四、妖精分不同类型，携带后增加能力也不同</T>\\n    <M>五、妖精是可以成长的，但是成长的上限和契主的等级是对应的</M>\\n我这样说你是否了解？你可以接着去找<D>妖精嘉迪拉</D>，她会继续向你介绍 ");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20024)then
	      Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20024,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n你好，我是<D>幻神艾丽娜</D>。很高兴你能到这里来，<N>七花之海</N>如今被魔物侵袭，混乱不堪。");
	elseif( id == 20100)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20100,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n不能让七花之海这样腐坏下去。");
        elseif( id == 20101)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20101,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n真是帮了大忙，看来能休息一阵了。");
        elseif( id == 20102)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20102,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n看来你修炼的还是很快的么。"); 
        elseif( id == 20105)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20105,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n你说这是从野人身上找到的，这可难办了。这样吧，你准备一下去萨斯林调查一下。");
        elseif( id == 20108)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20108,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n红野人，居然也变成了那样，接下来的事情会非常棘手。");
        elseif( id == 20113)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20113,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n你居然杀死了那个<A>恶魔拉斯</A>，真是厉害。");  
        elseif( id == 20114)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20114,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n没想到还没多少工夫，你已经<C>18</C>级了。");  
	elseif( id == 20115)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20115,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n如今这个恶魔已死，七花之海真的是安全了，不知道该怎么感谢你才好。这个你就收下吧。");  
	elseif( id == 40001)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(40001,"<font=龙字体,0,19,20><color=0xfffcf157>幻神艾丽娜：<color=0xffffffff><font=龙字体,0,17,20>\\n辛苦了，既然你完全按我说的做到了，这里有两种不同的<T>妖精</T>，都送给你。获得以后，<T>在背包中右键点击妖精可以进行携带</T>，然后在<N>角色信息的妖精界面</N>中可以查看<T>妖精的属性</T>，快拿去吧！");        
       end


end

Railina = {}
Railina["OnTalk"] = OnTalk
Railina["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Railina["OnCompleteQuestTalk"] = OnCompleteQuestTalk