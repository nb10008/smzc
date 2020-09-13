
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010209);
	Quest.SetCompleteOptions(5010209);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n想做我的朋友吗？我们的友情是要经过考验的！");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20631)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20631,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n我在找<color=0xffff0000>大地割裂之王<color=0xffffffff>的脚印！很珍贵的！听说这种亚龙只有魔界的王族还能拥有！我只要能得到他的脚印拓本就满足了！");
        elseif( id == 20632)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20632,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n不过据说<color=0xffff0000>大地割裂之王<color=0xffffffff>的身旁常有<color=0xffff6000>精英嗜月狼<color=0xffffffff>守护，哎呀！很棘手啊！怎样？愿不愿意帮我？嘿嘿！既然你答应了，便顺路从非人系怪物那里找些魔晶来吧！");
        elseif( id == 20633)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20633,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n既然是朋友那就再帮我跑一趟如何？不难不难！不过就是找来<C>1</C>支<I>大地割裂之王的手爪</I>和<C>30</C>支<I>精英嗜月狼的手爪</I>，你问在哪里找？当然是他们身上了！");
	elseif( id == 20634)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20634,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n我叫罗特！你们老大玛斯让我来这里的做事！其实很简单，让我来调查魔族的动静，看来……哼哼！魔族果然不老实。");	
	elseif( id == 20635)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20635,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n别犹豫，去看看，或许会另有收获也说不定！");
        elseif( id == 20636)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20636,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n这回还是<N>肃界沙漠</N>，不过猎物换了！被魔族收买的魔盗们，不但在沙漠中劫掠旅人，更曾将一部分低阶天神打伤！罪无可恕啊！还愣着干啥？动手！");
	elseif( id == 20637)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20637,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n<color=0xffff0000>鬼箭<color=0xffffffff>和他手下的得力打手<color=0xffff6000>疯狂魔徒<color=0xffffffff>很是厉害！寻常天神见了他们都要绕路，你要是没有信心就直说，我不会笑话你的！");
	elseif( id == 20638)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20638,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n看把你急得！来大干一场吧！去把<N>狂沙穴</N>里的<color=0xffff0000>岩浆精华之心<color=0xffffffff>一系的怪物都剿灭！");
        elseif( id == 20639)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20639,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n好！这次再挑战下？把<N>狂沙穴</N>里<color=0xffff0000>冥山山王<color=0xffffffff>一系的怪物全部清除！");
	elseif( id == 20640)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20640,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n看你这么辛苦，这次来些简单的！挖来<C>30</C>条<I>沙龙</I>，我给你做道好菜！");	
	elseif( id == 20641)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20641,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n朋友！你不会想看我活活渴死吧？到南方的小树林，帮我摘些<I>果子</I>，这鬼天气真是太热了！");       
        elseif( id == 20642)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20642,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n什么？你真的发现原神教的人了？他们好像在寻宝？恩……抓来一名<A>觅宝者</A>，让我问问！");
        elseif( id == 20643)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20643,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n等等！先把原神教的<color=0xffff0000>护法黑魅姬<color=0xffffffff>抓来，兄弟我要亲自教导她！");
	elseif( id == 20644)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20644,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n这下可以放心地动手了！");	
	elseif( id == 20645)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20645,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n把<I>信号烟花</I>点燃就可以了！");
        elseif( id == 20646)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20646,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n<N>翠玉乡</N>那边的蛮人中，最近好像来了个陌生人，看上去很可疑！去查查他的底细！");
	elseif( id == 20647)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20647,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n这些蛮人相信已经被魔族收买，灭了他们吧！");
	elseif( id == 20648)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20648,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n你厌恶了我的作为？呵呵！没关系，人间处处都在争斗，以前和我同村的邻居现在作为魔族的使者就在月牙泉，他会告诉你，我们是为了什么……");
        elseif( id == 20662)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20662,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n快去提升自己的等级吧。");
	elseif( id == 20663)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20663,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n你若想要继续征途，则需要更高的等级。");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20630)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20630,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n在这里！就在这里！怎么没了？");
        elseif( id == 20631)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20631,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n什么？你没听说过？");
	elseif( id == 20632)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20632,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n梦想成真啦！哈哈！你这天神真够朋友，嘿嘿！");
	elseif( id == 20633)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20633,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n恩！好久没有吃到了！嘿嘿，好朋友，正式认识一下！");
        elseif( id == 20634)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20634,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n看到那块不同寻常的岩石了吗？我怀疑那下面藏着魔族！");
	elseif( id == 20635)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20635,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n怎样？很刺激吧！哈哈！开个玩笑而已，这些魔族为了捕捉<color=0xffff0000>大地割裂之王<color=0xffffffff>而来，看来被你捷足先登了。");
	elseif( id == 20636)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20636,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n很顺利嘛！怎么？没找到他们的首领吗？");
	elseif( id == 20637)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20637,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n嘿嘿！还不感谢我？要不是激将法给你鼓起勇气，你……你不要这么凶的看着我啊！大不了算你厉害！");
	elseif( id == 20638)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20638,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n恩，很强嘛，如此看来，拿下<color=0xffff0000>冥山山王<color=0xffffffff>应该也不在话下了？");
	elseif( id == 20639)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20639,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n这下我算是超额完成任务了，哈哈！多谢！你们老大那边我会为你多多美言啦！");
	elseif( id == 20640)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20640,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n别看这沙龙长得像蚯蚓，可这味道嘛！！哈哈，那真是绝了！也像蚯蚓……");
	elseif( id == 20641)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20641,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n恩……怎么一股原神教的味道？");
	elseif( id == 20642)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20642,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n竟然咬舌自尽了？好！是条汉子！好朋友，你就成全这群汉子吧！");
	elseif( id == 20643)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20643,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n这群教众想背叛神王的教诲！嘿嘿！好！等我带她回府，再好好管教于她！");
	elseif( id == 20644)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20644,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n很好！看来我们应该向神王汇报一下进展呢！");
	elseif( id == 20645)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20645,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n你说神王大人会怎么夸我呢？");
	elseif( id == 20646)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20646,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n我太聪明了！既然如此……");
	elseif( id == 20647)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20647,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n干的漂亮！投奔魔族就该当受死！");
	elseif( id == 20662)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20662,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n已经到了93级了，后面会更加艰险。");
	elseif( id == 20663)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20663,"<font=龙字体,0,19,20><color=0xfffcf157>罗特：<color=0xffffffff>\\n继续努力吧，后面还有很多的事情要做呢。");
       end

end

Rluote = {}
Rluote["OnTalk"] = OnTalk
Rluote["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rluote["OnCompleteQuestTalk"] = OnCompleteQuestTalk