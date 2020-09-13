--任务描述：踢人任务
--任务作者：szj
--任务功能：若某个任务在规定的时间内未能完成，就将其踢掉
--使用频度：很少
----------------------------- KillUser_szj-------------------------------------------
function AddTask_KillUser( nRunID, nKillNo, nFirExeTime )
	var = {};
	var.m_uSortID	 	  = 0x09;						--不能改，要改就要相应的改程序
	var.m_uRunID		  = nRunID; 				--1;		--整个运行期间唯一，做配置时注意修改
	var.m_uExeProb		= 100;
	var.m_uExeTimes		= 1;
	var.m_uLogTimes		= 1;
	var.m_uTimeOut		= 255;		  			--255说明，不检查此任务的超时，也就是没有返回	
	var.m_uFirExeTime	= nFirExeTime;		--200;		--第一次执行时间，此种任务最好在一定范围内稍长一点，
									                    --以防网络或客户端运行快慢原因，导致时间很长。
	var.m_uExeInterval= 0;
	var.m_uWinSysLimit= 0x00;
	var.m_uLoadFlag		= 0x01;
	var.m_setOS       = {0x23,0x56,0x78}; 

	--踢人的错误号
	--以后再添加别的操作、配置	
	var.m_pPara = {};
	var.m_pPara.m_nKillNo = nKillNo; 		--63011;
	
	LuaData( "TaskQ", var );	
end;
------------------------------------------------------------
   AddTask_KillUser( 1005, 63012, 200000 );	--宋占军任务