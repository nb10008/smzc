local guild_pos = { 
										["GuildPos_Name0"] = "普通会员",
										["GuildPos_Name1"] = "精英会员",
										["GuildPos_Name2"] = "会长",
										["GuildPos_Name3"] = "副会长",
										["GuildPos_Name4"] = "骨干会员",
										--["GuildPos_Name5"] = "紫衫护法",
										--["GuildPos_Name00"] = "青龙堂主",
										--["GuildPos_Name01"] = "青龙香主",
										--["GuildPos_Name02"] = "青龙帮众",
										--["GuildPos_Name10"] = "朱雀堂主",
										--["GuildPos_Name11"] = "朱雀香主",
										--["GuildPos_Name12"] = "朱雀帮众",
										--["GuildPos_Name20"] = "白虎堂主",
										--["GuildPos_Name21"] = "白虎香主",
										--["GuildPos_Name22"] = "白虎帮众",
										--["GuildPos_Name30"] = "玄武堂主",
										--["GuildPos_Name31"] = "玄武香主",
										--["GuildPos_Name32"] = "玄武帮众"
									}
									
local guildpos_tips = {
	["GuildPos_Name0"] = "公会普通成员，可享受学习公会技能的权利，可接取公会任务。",
	["GuildPos_Name1"] = "公会精英成员，可由会长、副会长、骨干进行任命，可享受学习公会技能的权利，可接取公会任务。",
	["GuildPos_Name2"] = "公会之主，拥有公会中最高的权限，可以任命任何职位，可设置公会成员的公会仓库的使用权限，可设置公会技能的研究方向。",
	["GuildPos_Name3"] = "公会中第二把交椅，由会长任命，一个公会最多可拥有3个副会长，协助会长管理公会。",
	["GuildPos_Name4"] = "公会骨干，可招收和邀请他人加入公会。",
	--["GuildPos_Name5"] = "仅限女性担任的帮派职务，拥有一些与众不同的特殊权限。",
	--["GuildPos_Name00"] = "青龙堂乃是帮派中专责资材筹集的堂口，其成员可在帮派管事处接取[筹集资材]任务，青龙堂主为青龙堂的管理者，拥有青龙堂全部功能权限。",
	--["GuildPos_Name01"] = "青龙堂乃是帮派中专责资材筹集的堂口，其成员可在帮派管事处接取[筹集资材]任务，青龙香主乃是堂主的左膀右臂，协助堂主管理青龙堂。",
	--["GuildPos_Name02"] = "青龙堂乃是帮派中专责资材筹集的堂口，其成员可在帮派管事处接取[筹集资材]任务，青龙帮众乃是青龙堂普通成员，可在堂主和香主的带领下参与青龙堂功能。",
	--["GuildPos_Name10"] = "朱雀堂乃是帮派中专责筹措资金的堂口，其成员可在帮派管事处进行[跑商]，朱雀堂主为朱雀堂的管理者，拥有朱雀堂全部功能权限。",
	--["GuildPos_Name11"] = "朱雀堂乃是帮派中专责筹措资金的堂口，其成员可在帮派管事处进行[跑商]，朱雀香主乃是堂主的左膀右臂，协助堂主管理朱雀堂。",
	--["GuildPos_Name12"] = "朱雀堂乃是帮派中专责筹措资金的堂口，其成员可在帮派管事处进行[跑商]，朱雀帮众乃是朱雀堂普通成员，可在堂主和香主的带领下参与朱雀堂功能。",
	--["GuildPos_Name20"] = "白虎堂乃是帮派中专责维护帮派安定的堂口，其成员可在帮派管事处接取[维护安定]任务，白虎堂主为白虎堂的管理者，拥有白虎堂全部功能权限。",
	--["GuildPos_Name21"] = "白虎堂乃是帮派中专责维护帮派安定的堂口，其成员可在帮派管事处接取[维护安定]任务，白虎香主乃是堂主的左膀右臂，协助堂主管理白虎堂。",
	--["GuildPos_Name22"] = "白虎堂乃是帮派中专责维护帮派安定的堂口，其成员可在帮派管事处接取[维护安定]任务，白虎帮众乃是白虎堂普通成员，可在堂主和香主的带领下参与白虎堂功能",
	--["GuildPos_Name30"] = "玄武堂乃是帮派中专责帮派设施建设的堂口，其成员可在帮派管事处进行[升级设施]，玄武堂主为玄武堂的管理者，拥有玄武堂全部功能权限。",
	--["GuildPos_Name31"] = "玄武堂乃是帮派中专责帮派设施建设的堂口，其成员可在帮派管事处进行[升级设施]，玄武香主乃是堂主的左膀右臂，协助堂主管理玄武堂。",
	--["GuildPos_Name32"] = "玄武堂乃是帮派中专责帮派设施建设的堂口，其成员可在帮派管事处进行[升级设施]，玄武帮众乃是玄武堂普通成员，可在堂主和香主的带领下参与玄武堂功能"
	}
											
function GetStringTips(szName,szPos)

	local pos = "<font=龙字体,0,20,20>" .. guild_pos[szName] .. guild_pos[szPos]
	
	local tips = "<font=龙字体,0,16,20><color=0xffffffff>" .. guildpos_tips[pos]
	
	local newPos = "<color=0xff00ff00>" .. pos .. "\\n"

	newPos = newPos .. tips
	
	return newPos
end

function GetStringTips(szName)

  local pos = "<font=龙字体,0,20,20>" .. guild_pos[szName]

	local tips = "<font=龙字体,0,16,20><color=0xffffffff>" .. guildpos_tips[szName]
	
	local newPos = "<color=0xff00ff00>" .. pos .. "\\n"

	newPos = newPos .. tips
	
	return newPos
end

