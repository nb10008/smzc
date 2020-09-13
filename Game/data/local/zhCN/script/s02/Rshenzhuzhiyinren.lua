
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5010048);
	Quest.SetCompleteOptions(5010048);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神铸指引人：<font=龙字体,0,16,20><color=0xffffffff>\\n普通的装备强化到20级就会达到瓶颈，无法再继续强化。只有那些非凡的绝世装备才可能突破极限，达到更高的层次。\\n关于神铸有以下几点需要注意:\\n<S>一.</S>神铸分为10级，每神铸1级，原装备的强化上限就加1。神铸最高到10级，即强化最高可达30级\\n<S>二.</S>只有紫色和紫色以上的装备可以神铸\\n<S>三.</S>辅助装备需要大于70级，并且最少是绿色品质，品质越高经验值获得越多，两件装备的部位必须一致\\n<S>四.</S>一件装备每天只能升级一次，但是神铸经验的获取没有限制");

end


local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if(id == 40044) then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(40044,"<font=龙字体,0,19,20><color=0xfffcf157>神铸指引人：<color=0xffffffff>\\n     如果你已经对神铸有了一定的了解，并且有神铸的需求，那么就从我这里拿走这把神奇的锤子吧。\\n不过这可不是白给你的，一把锤子500金，得到后就可以永久使用了。丢失了也不怕，不过那就要再花一次钱了。");
	end
end


--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if(id == 40044)then
		if( step == QuestTalkBegin )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40044,QuestTalkBegin+1,"确定！" );
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40044,QuestTalkBegin+2,"再考虑一下" );
			Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>神铸指引人：<color=0xffffffff>\\n请注意：完成此任务需要花费您500金，获得神铸锤一个。确定吗？");

		elseif( step == QuestTalkBegin+1 )then
			Quest.CloseNPCTalk();
			Quest.CompleteQuestTalk(40044,"<font=龙字体,0,19,20><color=0xfffcf157>神铸指引人：<color=0xffffffff>\\n看来你已经做好了神铸的准备，这把神铸锤拿去吧!");
		elseif( step == QuestTalkBegin+2 )then
			Quest.ClearNPCTalkOptions();
			Quest.SetNPCTalkOption( EOT_CompleteQuest,40044,QuestTalkBegin,"离开" );
			Quest.NPCTalk(0,0,"还没准备好");
		end
	end
end

Rshenzhuzhiyinren = {}
Rshenzhuzhiyinren["OnTalk"] = OnTalk
Rshenzhuzhiyinren["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Rshenzhuzhiyinren["OnCompleteQuestTalk"] = OnCompleteQuestTalk
