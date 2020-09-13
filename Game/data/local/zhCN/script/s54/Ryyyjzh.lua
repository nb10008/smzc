--幽怨妖精之魂

local function OnTalk()
	Quest.ClearNPCTalkOptions();                    --清除对话框选项
	Quest.SetAcceptOptions(5610002);
	Quest.SetCompleteOptions(5610002);
	Quest.NPCTalk(0,0,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂：<color=0xffffffff>\\n        这里是被诅咒的幽怨之地，盖拉正在为净化这里而做巨大的努力。")
end

local function OnAcceptQuestTalk(id,step)--如果删除任务，可重新手动接取
	if( id == 30306)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30306,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        当你拥有足够的动物代币之后，我可以用一个忠实的小宠物和你交换它们。")
	end
	if( id == 30322)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30322,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        黑暗力量即将来袭，我要编制一座梦境牢笼，将肮脏的恶魔锁在它们自己的梦境深处，梦境粉尘散落在庇护所的四处，用你的耐心和毅力去寻找它们吧。")
	end
	if( id == 30323)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30323,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        黑暗力量即将来袭，我要编制一座梦境牢笼，将肮脏的恶魔锁在它们自己的梦境深处，梦境粉尘散落在庇护所的四处，用你的耐心和毅力去寻找它们吧。")
	end
	if( id == 30324)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30324,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        黑暗力量即将来袭，我要编制一座梦境牢笼，将肮脏的恶魔锁在它们自己的梦境深处，梦境粉尘散落在庇护所的四处，用你的耐心和毅力去寻找它们吧。")
	end
	if( id == 30325)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30325,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        黑暗力量即将来袭，我要编制一座梦境牢笼，将肮脏的恶魔锁在它们自己的梦境深处，梦境粉尘散落在庇护所的四处，用你的耐心和毅力去寻找它们吧。")
	end
	if( id == 30326)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30326,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        黑暗力量即将来袭，我要编制一座梦境牢笼，将肮脏的恶魔锁在它们自己的梦境深处，梦境粉尘散落在庇护所的四处，用你的耐心和毅力去寻找它们吧。")
	end
	if( id == 30327)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30327,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        黑暗力量即将来袭，我要编制一座梦境牢笼，将肮脏的恶魔锁在它们自己的梦境深处，梦境粉尘散落在庇护所的四处，用你的耐心和毅力去寻找它们吧。")
	end
	if( id == 30328)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30328,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        黑暗力量即将来袭，我要编制一座梦境牢笼，将肮脏的恶魔锁在它们自己的梦境深处，梦境粉尘散落在庇护所的四处，用你的耐心和毅力去寻找它们吧。")
	end
	if( id == 30329)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30329,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        黑暗力量即将来袭，我要编制一座梦境牢笼，将肮脏的恶魔锁在它们自己的梦境深处，梦境粉尘散落在庇护所的四处，用你的耐心和毅力去寻找它们吧。")
	end
	if( id == 30330)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30330,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        黑暗力量即将来袭，我要编制一座梦境牢笼，将肮脏的恶魔锁在它们自己的梦境深处，梦境粉尘散落在庇护所的四处，用你的耐心和毅力去寻找它们吧。")
	end
	if( id == 30331)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30331,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        黑暗力量即将来袭，我要编制一座梦境牢笼，将肮脏的恶魔锁在它们自己的梦境深处，梦境粉尘散落在庇护所的四处，用你的耐心和毅力去寻找它们吧。")
	end
	if( id == 30332)then
	    Quest.CloseNPCTalk();
		Quest.AcceptQuestTalk(30332,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        黑暗力量即将来袭，我要编制一座梦境牢笼，将肮脏的恶魔锁在它们自己的梦境深处，梦境粉尘散落在庇护所的四处，用你的耐心和毅力去寻找它们吧。")
	end

end

--点击交任务选项后
local function OnCompleteQuestTalk(id,step)
	if( id == 30306)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30306,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        其他的小动物们还在等待着你呢。")
	elseif( id == 30322)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30322,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这一点点成效还不值得你为此骄傲，继续努力吧，我的朋友。")
	elseif( id == 30323)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30323,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这一点点成效还不值得你为此骄傲，继续努力吧，我的朋友。")
	elseif( id == 30324)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30324,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这一点点成效还不值得你为此骄傲，继续努力吧，我的朋友。")
	elseif( id == 30325)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30325,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这一点点成效还不值得你为此骄傲，继续努力吧，我的朋友。")
	elseif( id == 30326)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30326,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这一点点成效还不值得你为此骄傲，继续努力吧，我的朋友。")
	elseif( id == 30327)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30327,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这一点点成效还不值得你为此骄傲，继续努力吧，我的朋友。")
	elseif( id == 30328)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30328,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这一点点成效还不值得你为此骄傲，继续努力吧，我的朋友。")
	elseif( id == 30329)then
		Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30329,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这一点点成效还不值得你为此骄傲，继续努力吧，我的朋友。")
	elseif( id == 30330)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30330,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这一点点成效还不值得你为此骄傲，继续努力吧，我的朋友。")
	elseif( id == 30331)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30331,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这一点点成效还不值得你为此骄傲，继续努力吧，我的朋友。")
	elseif( id == 30332)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(30332,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这一点点成效还不值得你为此骄傲，继续努力吧，我的朋友。")
	elseif( id == 40062)then
	    Quest.CloseNPCTalk();
		Quest.CompleteQuestTalk(40062,"<font=龙字体,0,19,20><color=0xfffcf157>幽怨妖精之魂:<color=0xffffffff>\\n        这里并不像你想象的那么简单，我的朋友。")
	end
end

Ryyyjzh = {}
Ryyyjzh["OnTalk"] = OnTalk
Ryyyjzh["OnAcceptQuestTalk"] = OnAcceptQuestTalk
Ryyyjzh["OnCompleteQuestTalk"] = OnCompleteQuestTalk
