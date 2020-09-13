
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010014);
	Quest.SetCompleteOptions(5010014);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神教官艾斯：<color=0xffffffff>\\n升级后可对技能进行升级加点，技能的快捷键为<color=0xffff00ff>K<color=0xffffffff>。");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
        if( id == 20005)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20005,"<font=龙字体,0,19,20><color=0xfffcf157>神教官艾斯：<color=0xffffffff>\\n    接下来，这里还有个<I>新手礼包</I>要送给你。<N>鼠标右键</N>点击新手礼包后可获得礼包内的物品。");
        elseif( id == 20006)then
                Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(20006,"<font=龙字体,0,19,20><color=0xfffcf157>神教官艾斯：<color=0xffffffff>\\n    请前往前方讨伐<color=0xffffe289>白灵<color=0xffffffff>，获得<color=0xff00a2ff>白灵之石<color=0xffffffff>后，将这些<color=0xff00a2ff>白灵之石<color=0xffffffff>交给<color=0xff00ddbb>神教官达蒙<color=0xffffffff>。");
        end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
        if( id == 20004)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20004,"<font=龙字体,0,19,20><color=0xfffcf157>神教官艾斯：<color=0xffffffff>\\n    接下来的试炼会更加艰苦，请先收下<I>新手试用圣药</I>和<I>新手试用圣水</I>，在体力或真气耗尽前记得使用！");
        elseif( id == 20005)then
              Quest.CloseNPCTalk();
	      Quest.CompleteQuestTalk(20005,"<font=龙字体,0,19,20><color=0xfffcf157>神教官艾斯：<color=0xffffffff>\\n    每次使用礼包后，都会获得新的礼包。在你到达<A>6</A>、<A>11</A>、<A>16</A>……<A>56</A>、<A>61</A>级的时候，都可以打开新的礼包获得奖励。");
       end

end

aisi = {}
aisi["OnTalk"] = OnTalk
aisi["OnAcceptQuestTalk"] = OnAcceptQuestTalk
aisi["OnCompleteQuestTalk"] = OnCompleteQuestTalk