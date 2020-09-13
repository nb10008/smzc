// 加入家族，包含申请加入和邀请加入
#pragma once

class FamilyJoin
{
public:
	FamilyJoin();
	~FamilyJoin();

	VOID Update();

	VOID AddJoinReq(tagJoinFamilyReq& JoinReq);
	VOID RemoveJoinReq(DWORD dwJoinerID);
	BOOL IsJoinReqExist(DWORD dwJoinerID);

private:
	VOID OnReqTimeOut(tagJoinFamilyReq& JoinReq);

private:
	typedef TMap<DWORD, tagJoinFamilyReq> JoinReqMap;
	JoinReqMap m_mapJoinReq;
};