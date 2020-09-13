
--NPCid:5510608

local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5510608);
	Quest.SetCompleteOptions(5510608);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>罗斯特：<color=0xffffffff><font=龙字体,0,17,20>\\n我负责看守这里，因为这里是通向斗战天城的唯一通路，不可有一丝马虎。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 20017)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20017,"<font=龙字体,0,19,20><color=0xfffcf157>罗斯特：<color=0xffffffff><font=龙字体,0,17,20>\\n    当你的等级达到<A>10</A>级，再过来找我吧，到时你变可以前往我们的主城——<N>斗战天城</N>，在那里，你会真正的开始你的战神之旅。");
        elseif( id == 20018)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20018,"<font=龙字体,0,19,20><color=0xfffcf157>罗斯特：<color=0xffffffff><font=龙字体,0,17,20>\\n    你可前往<N>传送门</N>，踏上这个传送门之后，你便可以前往恢弘的<N>斗战天城</N>。以后我们可能再无机会相见，希望你能够更加精进，在你到达<color=0xffff0000>20级<color=0xffffffff>的时候，若通过了就职任务，那么你便可以成为一名真正的战神。\\n    在抵达斗战天城以后，你可以找到凤羽，她将为你进行下一步的指引。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 20016)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin+1,"“K”键[正确答案]" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin+2,"“V”键" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin+3,"“P”键" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>罗斯特：<color=0xffffffff><font=龙字体,0,17,20>\\n问题一：技能的快捷键是哪个？");


		elseif( step == QuestTalkBegin+1 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin+4,"“F”键[正确答案]" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin+5,"“Q”键" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin+6,"“H”键" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>罗斯特：<color=0xffffffff><font=龙字体,0,17,20>\\n问题二：打开好友列表的快捷键是哪个？");

		elseif( step == QuestTalkBegin+4 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin+7,"体力" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin+8,"武力[正确答案]" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin+9,"真元" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>罗斯特：<color=0xffffffff><font=龙字体,0,17,20>\\n问题三：角色增加那个属性点，可以增加物理攻击？");


		elseif( step == (QuestTalkBegin+2))then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+3))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+5))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+6))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+7))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == (QuestTalkBegin+9))then
			Quest.ClearNPCTalkOptions();
		  	Quest.SetNPCTalkOption( EOT_CompleteQuest,20016,QuestTalkBegin,"确定" );
			Quest.NPCTalk(0,0,"答错了，请你再仔细回忆一下");
		elseif( step == QuestTalkBegin+8 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(20016,"<font=龙字体,0,19,20><color=0xfffcf157>罗斯特：<color=0xffffffff><font=龙字体,0,17,20>\\n你很棒！本次通过考验的候补寥寥无几，能够脱颖而出，你要继续努力啊！当你获得了更好的装备时，别忘记更换，能够对你的实力有极大的提升。记住，装备的品质是以白、<color=0xffffff00>黄<color=0xffffffff>、<color=0xff05ff00>绿<color=0xffffffff>、<color=0xff00a2ff>蓝<color=0xffffffff>、<color=0xffffae00>橙<color=0xffffffff>、<color=0xff9933ff>紫<color=0xffffffff>等进行区分的！");
		end
        elseif( id == 20017)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20017,"<font=龙字体,0,19,20><color=0xfffcf157>罗斯特：<color=0xffffffff><font=龙字体,0,17,20>\\n    我就知道你没有问题，看，我已经为你准备好了，这些物件全都是送你的。");
       end

end

luosite = {}
luosite["OnTalk"] = OnTalk
luosite["OnAcceptQuestTalk"] = OnAcceptQuestTalk
luosite["OnCompleteQuestTalk"] = OnCompleteQuestTalk