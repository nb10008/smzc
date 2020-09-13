
--NPCid:5510604
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项

		r = Quest.GetRandom(7);
		if ( r==0 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神守卫：<color=0xffffffff>\\n我们奉命在此守卫<N>天神冢</N>，此处是天神的起源之所");
		elseif ( r==1 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神守卫：<color=0xffffffff>\\n好好努力！争取顺利通过天神试炼");
		elseif ( r==2 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神守卫：<color=0xffffffff>\\n如果身上没有任务了，可以找到<D>悬赏榜</D>接取悬赏任务进行升级");
		elseif ( r==3 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神守卫：<color=0xffffffff>\\n您可以按下键盘上的<A>A</A>、<A>S</A>、<A>D</A>、<A>W</A>键进行移动，按下<A>空格</A>可以进行跳跃");
		elseif ( r==4 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神守卫：<color=0xffffffff>\\n按下<T>TAB</T>可以进行自动选怪，可以简化您的战斗操作。");
		elseif ( r==5 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神守卫：<color=0xffffffff>\\n键盘上的<T>“~”</T>键可以进行拾取，不需要频繁地点击鼠标来拾取物品");
		elseif ( r==6 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神守卫：<color=0xffffffff>\\n当您跟随着主线任务，达到10级后，便可以离开天神冢前往斗战天城了，可以在<T>东南方</T>找到传送点");
	end
end

Rtianshenshouwei2 = {}
Rtianshenshouwei2["OnTalk"] = OnTalk