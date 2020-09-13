
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010013);
	Quest.SetCompleteOptions(5010013);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天隐：<color=0xffffffff>\\n升级后可对技能进行升级加点，技能的快捷键为<color=0xffff00ff>K<color=0xffffffff>。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20004)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20004,"<font=龙字体,0,19,20><color=0xfffcf157>天隐：<color=0xffffffff>\\n    目前你只可学习和升级新手技能，达到<color=0xff00ddfe>20<color=0xffffffff>级就职以后可学习<color=0xff80ff00>职业技能<color=0xffffffff>。\\n接下来找到<color=0xff00ddbb>新手教官罗拉<color=0xffffffff>继续进行教学。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20003)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20003,"<font=龙字体,0,19,20><color=0xfffcf157>天隐：<color=0xffffffff>\\n    第一次的战斗看来做的不错。完成任务后将会<color=0xffffff00>升级<color=0xffffffff>，升级后可获得<color=0xffffff00>技能点<color=0xffffffff>，别忘记同时升级你的技能<color=0xffff00ff>[技能快捷键K]<color=0xffffffff>。");

       end

end

Rtianyin = {}
Rtianyin["OnTalk"] = OnTalk
Rtianyin["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rtianyin["OnCompleteQuestTalk"] = OnCompleteQuestTalk