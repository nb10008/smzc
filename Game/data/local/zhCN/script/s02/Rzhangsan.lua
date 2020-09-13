
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610053);
	Quest.SetCompleteOptions(5610053);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>张三：<color=0xffffffff>\\n今年的黑丝会不会很多呢...");
end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 30793)then
			Quest.CloseNPCTalk();
		    Quest.AcceptQuestTalk(30793,"<font=龙字体,0,19,20><color=0xfffcf157>张三:<color=0xffffffff>\\n是“长腿妹子”让你来找我的吗，我昨天心血来潮，掐指一算，又要到CHINA-JOY的盛会了。去年我亲手为妹子特制的三点式贴身小内衣，看来非常的符合妹子的口味。今年是不是又要来麻烦我了。好吧，既然是妹子的事儿，我自然不宜推辞，你就先给我找来一些材料吧。由于本人十分的重口，所以需要的材料自然也是多种多样。可能对于现在的你来说需要花些心思呦！");
        elseif( id == 30795)then
			Quest.CloseNPCTalk();
		    Quest.AcceptQuestTalk(30795,"<font=龙字体,0,19,20><color=0xfffcf157>张三:<color=0xffffffff>\\n小子，怎么样，这妹子的身段儿可够火爆吧？哈哈哈，既然有了标准的尺寸，那么我就开工了，等待2分钟，就可以完成了！");
		elseif( id == 30816)then
			Quest.CloseNPCTalk();
		    Quest.AcceptQuestTalk(30816,"<font=龙字体,0,19,20><color=0xfffcf157>张三:<color=0xffffffff>\\n虽然我答应帮忙改衣服，可是现在没心情，前几天我路过东洲冥山的时候，被一伙强盗洗劫了，虽然我不在乎那点钱，可是心里始终不痛快，你先去帮我出口恶气，然后咱们在说正事。");
		elseif( id == 30817)then
			Quest.CloseNPCTalk();
		    Quest.AcceptQuestTalk(30817,"<font=龙字体,0,19,20><color=0xfffcf157>张三:<color=0xffffffff>\\n还是老规矩，我需要一些特殊材料，帮我先搞定这些材料，我才能动手制作。我需要“大兽牙”5个（可在东洲冥山的奇货商处买到）、“沼润石”5个（可在南洲迷沼的奇货商处买到）、“冥山炎石”30个（80-84级怪物掉落）。");
		elseif( id == 30818)then
			Quest.CloseNPCTalk();
		    Quest.AcceptQuestTalk(30818,"<font=龙字体,0,19,20><color=0xfffcf157>张三:<color=0xffffffff>\\n我想起来了，你还需要给我一个高级凿石，因为这次的材料中有一些需要打磨的地方。");

	   end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 30793)then
              Quest.CloseNPCTalk();
	          Quest.CompleteQuestTalk(30793,"<font=龙字体,0,19,20><color=0xfffcf157>张三：<color=0xffffffff>\\n年轻人你的动作很快嘛，不过光有材料还是远远不够的。制作一件衣服哪有你想的那么简单，我还需要你回去找妹子要她的各项数据才能做出得体的衣服。虽然去年我已经做过一次了，但谁知道一年没见妹子的身材有没有改变。");
		elseif( id == 30795)then
		      if( not Quest.QuestCanComplete(30795) ) then
                    if( step == QuestTalkBegin )then
						Quest.ClearNPCTalkOptions();
						Quest.SetNPCTalkOption( EOT_CompleteQuest,30795,QuestTalkBegin+1,"新衣服好了吗？" );
			            Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>张三：<color=0xffffffff>\\n    新衣服马上就好了！");
					elseif( step == QuestTalkBegin + 1) then
					    Quest.UpdateQuest(30795);
					end
			  else
					Quest.CloseNPCTalk();
	                Quest.CompleteQuestTalk(30795,"<font=龙字体,0,19,20><color=0xfffcf157>张三：<color=0xffffffff>\\n好了，终于OK了，完美的杰作啊！快拿去给妹子看看吧！");
			  end

	   elseif( id == 30815)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30815,"<font=龙字体,0,19,20><color=0xfffcf157>张三：<color=0xffffffff>\\n小子，你又来了，有什么事吗？什么！衣服要改，我的天呐，好吧，看在妹子火爆身材的份上，就再帮一次。");
       elseif( id == 30816)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30816,"<font=龙字体,0,19,20><color=0xfffcf157>张三：<color=0xffffffff>\\n现在心情好多了，不过如果是妹子站在我面前的话会更好，我来想想这衣服怎么改。");
       elseif( id == 30817)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30817,"<font=龙字体,0,19,20><color=0xfffcf157>张三：<color=0xffffffff>\\n嗯，除了这些材料外，我得想想还需要什么东西。");
       elseif( id == 30818)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(30818,"<font=龙字体,0,19,20><color=0xfffcf157>张三：<color=0xffffffff>\\n好了，这次真的是一切OK了，应该是符合妹子的要求了！你快去拿给她看看吧！");

	   end
end

Rzhangsan = {}
Rzhangsan["OnTalk"] = OnTalk
Rzhangsan["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rzhangsan["OnCompleteQuestTalk"] = OnCompleteQuestTalk
