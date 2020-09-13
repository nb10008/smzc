
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010087);
	Quest.SetCompleteOptions(5010087);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>雪神艾琳：<color=0xffffffff>\\n雪舞冰晶，绮丽纷扬！你好！我是雪神。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20301)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20301,"<font=龙字体,0,19,20><color=0xfffcf157>雪神艾琳：<color=0xffffffff>\\n    记住一点，我们是天神，和那些人界的人不同。我们可以用神力击败恶魔，但是人类却永远不能杀死魔族。他们只能禁锢魔族的力量，包括天神的。到<N>天雪镇</N>去吧，万事都要小心，到了那里肯定会有人来迎接你的。");

        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20300)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20300,"<font=龙字体,0,19,20><color=0xfffcf157>雪神艾琳：<color=0xffffffff>\\n        你就是斗神之王玛斯说的新神吧，这里的文化与天界差异明显。你可能要适应一阵子，既然是来除魔的，就到山下的天雪镇去吧。");

       end

end

Railin = {}
Railin["OnTalk"] = OnTalk
Railin["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Railin["OnCompleteQuestTalk"] = OnCompleteQuestTalk