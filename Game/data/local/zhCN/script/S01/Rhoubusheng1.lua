
--NPCid:5510608
local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项

		r = Quest.GetRandom(7);
		if ( r==0 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神候补生：<color=0xffffffff>\\n马上就要进行试炼了，好紧张！");
		elseif ( r==1 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神候补生：<color=0xffffffff>\\n听说天神冢里的<A>六天神魔</A>很厉害，我很担心我自己是否能应付得了。");
		elseif ( r==2 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神候补生：<color=0xffffffff>\\n千万不要忘记找<D>军需官艾拉</D>领取武器！有了武器我们才能够与魔物对抗！");
		elseif ( r==3 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神候补生：<color=0xffffffff>\\n您可以按下键盘上的<A>A</A>、<A>S</A>、<A>D</A>、<A>W</A>键进行移动，按下<A>空格</A>可以进行跳跃");
		elseif ( r==4 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神候补生：<color=0xffffffff>\\n按下<T>TAB</T>可以进行自动选怪，可以简化您的战斗操作。");
		elseif ( r==5 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神候补生：<color=0xffffffff>\\n键盘上的<T>“~”</T>键可以进行拾取，不需要频繁地点击鼠标来拾取物品");
		elseif ( r==6 ) then
		Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>天神候补生：<color=0xffffffff>\\n当您跟随着主线任务，达到10级后，便可以离开天神冢前往斗战天城了，可以在<T>东南方</T>找到传送点");
	end
end

Rhoubusheng1 = {}
Rhoubusheng1["OnTalk"] = OnTalk