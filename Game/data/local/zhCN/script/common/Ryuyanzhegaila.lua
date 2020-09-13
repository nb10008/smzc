--预言者盖拉

local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610001);
	Quest.SetCompleteOptions(5610001);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉：<color=0xffffffff>\\n        你听说过上古妖精庇护所吗？在西洲幻林，南州迷沼和东洲冥山，你可以偶尔看见通往上古妖精庇护所的传送门。不过这个传送门并不稳定，所有进入上古妖精庇护所的人都将在40分钟后返回传送门出现的地方。\\n只有等级达到50级以上的人才能进入上古妖精庇护所。")
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
--信仰之石
	if( id == 30308)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30308,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        信仰之石通过炼化可以转化为能够利用的信仰之力。去收集一些信仰之石，我会帮你把它们都转化为信仰。\\n<T>信仰之石可以通过完成悬赏榜中绿色以上等级的任务获得。</T>")
	end
	if( id == 30309)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30309,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        信仰之石通过炼化可以转化为能够利用的信仰之力。去收集一些信仰之石，我会帮你把它们都转化为信仰。\\n<T>信仰之石可以通过完成悬赏榜中绿色以上等级的任务获得。</T>")
	end
	if( id == 30310)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30310,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        信仰之石通过炼化可以转化为能够利用的信仰之力。去收集一些信仰之石，我会帮你把它们都转化为信仰。\\n<T>信仰之石可以通过完成悬赏榜中绿色以上等级的任务获得。</T>")
	end
	if( id == 30311)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30311,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        信仰之石通过炼化可以转化为能够利用的信仰之力。去收集一些信仰之石，我会帮你把它们都转化为信仰。\\n<T>信仰之石可以通过完成悬赏榜中绿色以上等级的任务获得。</T>")
	end
	if( id == 30312)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30312,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        信仰之石通过炼化可以转化为能够利用的信仰之力。去收集一些信仰之石，我会帮你把它们都转化为信仰。\\n<T>信仰之石可以通过完成悬赏榜中绿色以上等级的任务获得。</T>")
	end
	if( id == 30313)then
		Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30313,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        信仰之石通过炼化可以转化为能够利用的信仰之力。去收集一些信仰之石，我会帮你把它们都转化为信仰。\\n<T>信仰之石可以通过完成悬赏榜中绿色以上等级的任务获得。</T>")
	end
	if( id == 30314)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30314,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        信仰之石通过炼化可以转化为能够利用的信仰之力。去收集一些信仰之石，我会帮你把它们都转化为信仰。\\n<T>信仰之石可以通过完成悬赏榜中绿色以上等级的任务获得。</T>")
	end
	if( id == 30315)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30315,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        信仰之石通过炼化可以转化为能够利用的信仰之力。去收集一些信仰之石，我会帮你把它们都转化为信仰。\\n<T>信仰之石可以通过完成悬赏榜中绿色以上等级的任务获得。</T>")
	end
--纯净之石

	if( id == 30316)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30316,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        纯净之石是我炼制魔法的材料，据说使用净化药剂将诅咒之石净化后，就可以得到纯净之石。\\n<T>所有40级以上的怪物都有一定几率掉落诅咒之石。</T>")
	end
	if( id == 30317)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30317,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        纯净之石是我炼制魔法的材料，据说使用净化药剂将诅咒之石净化后，就可以得到纯净之石。\\n<T>所有40级以上的怪物都有一定几率掉落诅咒之石。</T>")
	end
	if( id == 30318)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30318,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        纯净之石是我炼制魔法的材料，据说使用净化药剂将诅咒之石净化后，就可以得到纯净之石。\\n<T>所有40级以上的怪物都有一定几率掉落诅咒之石。</T>")
	end
	if( id == 30319)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30319,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        纯净之石是我炼制魔法的材料，据说使用净化药剂将诅咒之石净化后，就可以得到纯净之石。\\n<T>所有40级以上的怪物都有一定几率掉落诅咒之石。</T>")
	end
	if( id == 30320)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30320,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        纯净之石是我炼制魔法的材料，据说使用净化药剂将诅咒之石净化后，就可以得到纯净之石。\\n<T>所有40级以上的怪物都有一定几率掉落诅咒之石。</T>")
	end
	if( id == 30321)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30321,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        纯净之石是我炼制魔法的材料，据说使用净化药剂将诅咒之石净化后，就可以得到纯净之石。\\n<T>所有40级以上的怪物都有一定几率掉落诅咒之石。</T>")
	end
end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30308)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30308,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你做的很好!众神以你为傲!")
	end
	if( id == 30309)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30309,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你做的很好!众神以你为傲!")
	end
	if( id == 30310)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30310,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你做的很好!众神以你为傲!")
	end
	if( id == 30311)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30311,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你做的很好!众神以你为傲!")
	end
	if( id == 30312)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30312,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你做的很好!众神以你为傲!")
	end
	if( id == 30313)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30313,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你做的很好!众神以你为傲!")
	end
	if( id == 30314)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30314,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你做的很好!众神以你为傲!")
	end
	if( id == 30315)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30315,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你做的很好!众神以你为傲!")
	end
	if( id == 30316)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30316,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你干得很好，年轻人！")
	end
	if( id == 30317)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30317,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你干得很好，年轻人！")
	end
	if( id == 30318)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30318,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你干得很好，年轻人！")
	end
	if( id == 30319)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30319,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你干得很好，年轻人！")
	end
	if( id == 30320)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30320,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你干得很好，年轻人！")
	end
	if( id == 30321)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30321,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你干得很好，年轻人！")
	elseif( id == 40066)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(40066,"<font=龙字体,0,19,20><color=0xfffcf157>预言者盖拉:<color=0xffffffff>\\n        你想获得更多的信仰吗？我可以帮你。")
	end

end

Ryuyanzhegaila = {}
Ryuyanzhegaila["OnTalk"] = OnTalk
Ryuyanzhegaila["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryuyanzhegaila["OnCompleteQuestTalk"] = OnCompleteQuestTalk
