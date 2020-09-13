
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010094);
	Quest.SetCompleteOptions(5010094);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n天下河流都由我来掌握。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20324)then
                Quest.CloseNPCTalk();
				Quest.AcceptQuestTalk(20324,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n    福临镇的北边有一叫做冷心洞的洞窟，里面是蛇妖的巢穴，这个洞穴妖气冲天，留着肯定后患无穷。");
        elseif( id == 20325)then
                Quest.CloseNPCTalk();
				Quest.AcceptQuestTalk(20325,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n    原来是那个恶魔，蛇妖杜莎梅和杜莎拉！这两姐妹可都不是省油的灯，不过作为天神，消灭她们这些魔族就是我们的责任。");
        elseif( id == 20326)then
                Quest.CloseNPCTalk();
				Quest.AcceptQuestTalk(20326,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n    巨熊威尔说发生了一些让他十分头痛的事情，需要一个靠得住的人去帮忙。我想你将等级提升到42级的话，应该可以胜任。");
        end
        if( id == 29019)then
		    Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(29019,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n    白银魔窟的傀儡们近日十分猖獗，为我去杀掉他们的傀儡头子艾尔特斯。");
        end
         if( id == 29020)then
			Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(29020,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n    魔窟里的罗纳德是人人愤恨的对象，去将他的首级取来。");
        end
         if( id == 29021)then
		     Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(29021,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n    请将白银魔窟中的独裁者爪牙阿尔特鲁击杀。");
        end
         if( id == 29022)then
		    Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(29022,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n    奥德是白银魔窟的独裁者，如果能将其打败，那么自然会还这片土地一阵安宁。");

        end
	if( id == 29025)then
		    Quest.CloseNPCTalk();
						Quest.AcceptQuestTalk(29025,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n    最近白银魔窟里的魔族很猖獗，去把那个地方清理一下吧。即使不能斩草除根，起码也要把里面的头目都消灭。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20323)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(20323,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n        看来这次的收获不小吧，那么接下来看看这个吧。");
        elseif( id == 20324)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(20324,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n        我们只是消灭了一下小喽啰而已，我觉得这洞中还有魔族在其中。");
        elseif( id == 20325)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(20325,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n        没想到这些魔族居然都来到了这里。");
		    elseif( id == 30338)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(30338,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n        你的速度比我预想的要快很多。");

		end
		if( id == 29019)then
			Quest.CloseNPCTalk();
	      	Quest.CompleteQuestTalk(29019,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n        做的好，这对你来说应该不难。");
		end
		if( id == 29020)then
			Quest.CloseNPCTalk();
	      	Quest.CompleteQuestTalk(29020,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n        哈哈，他终于得到了应有的惩罚。");
		end
		if( id == 29021)then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(29021,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n         很好，我们距离光明又迈进了一步。");
		end
		if( id == 29022)then
			Quest.CloseNPCTalk();
	      	Quest.CompleteQuestTalk(29022,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n        你太厉害了，我竟然不敢相信独裁者的世界已经被彻底摧毁。");
		end
		if( id == 29025)then
			Quest.CloseNPCTalk();
	      	Quest.CompleteQuestTalk(29025,"<font=龙字体,0,19,20><color=0xfffcf157>河神巴克：<color=0xffffffff>\\n        有了这次教训，他们会收敛很多！");
		end
end

Rbake = {}
Rbake["OnTalk"] = OnTalk
Rbake["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rbake["OnCompleteQuestTalk"] = OnCompleteQuestTalk
