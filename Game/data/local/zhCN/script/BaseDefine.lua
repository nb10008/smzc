ScenarioTalkBegin=1000;		--情节对话从1000开始
ScenarioTalkEnd=1999;
QuestTalkBegin=0;			--任务对话从0开始--
QuestTalkEnd=999;

--选项类型
EOT_AcceptQuest			= 0		--可接任务选项
EOT_CompleteQuest		= 1		--可交任务选项
EOT_ScenarioTalk		= 2		--情景对话选项
EOT_ProduceSkillTalk	= 3		--
EOT_LearnProduceSkill	= 4		--
EOT_LeaveProduceSkill	= 5		--
EOT_CancelProduceSkill	= 6		--
EOT_ServerCtrl			= 7		--服务器控制选项
EOT_GuildCommerce	    = 8
EOT_SetTransportPoint   = 9
EOT_CityProductivity    = 10
EOT_CompleteQuestBySpecItem = 11
EOT_GodEvilRepute = 15



--类似于随身仓库界面的 选项类型
ESTOT_WalkWare			= 0		--随身仓库





--消息流中数据块的类型
EMUT_MsgID				= 1
EMUT_RoleID				= 2
EMUT_RoleName			= 3
EMUT_ItemTypeID			= 4
EMUT_NPCTypeID			= 5
EMUT_MapCrc				= 6
EMUT_TriggerID			= 7
EMUT_Pos				= 8
EMUT_Integer			= 9
EMUT_TotalLoop			= 10
EMUT_QuestID			= 11
EMUT_Num				= 12
EMUT_ChatInfo			= 13
EMUT_Score				= 14
EMUT_Rank				= 15
EMUT_Time				= 16
EMUT_RebornTime			= 17
EMUT_TeamID				= 18
EMUT_GuildID			= 19
EMUT_DlgWithSelOpt		= 20
EMUT_DlgOption			= 21
EMUT_DlgTarget_Quest	= 22
EMUT_DlgTarget_Item		= 23
EMUT_DlgTarget_Role		= 24
EMUT_DlgTarget_Activity	= 25
EMUT_ErrorCode			= 26
EMUT_DlgQuiz			= 40
EMUT_DlgArrange			= 50
EMUT_PVPActivity		= 60
EMUT_DlgPopup			= 71
EMUT_Boardcast			= 100
EMUT_BigBroadcast       = 101
EMUT_NewBroadcast       = 102
EMUT_UIEffect						= 110

