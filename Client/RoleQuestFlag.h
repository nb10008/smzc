#pragma once

enum EQuestFlag
{
	EQF_Null,
	EQF_Accept,					//可接受
	EQF_Finish,					//完成
	EQF_InAcceptability,		//不可接受
	EQF_NotFinish,				//没完成
    EQF_RepeatAccept,           //重复任务可接受
    EQF_RepeatFinish            //重复任务交
};


class RoleQuestFlag
{
public:
	RoleQuestFlag(void);
	~RoleQuestFlag(void);

	void Create();
	void Destroy();

	void Update();
	void Draw(POINT pt,float z,float scale,EQuestFlag qf);

private:
	int						m_curFrame;
	float					m_frameTime; 

	vector<ResTexture*>		m_picArray0;    //!< 接
	vector<ResTexture*>		m_picArray1;    //!< 交
    vector<ResTexture*>		m_picArray2;    //!< 重复任务交
    vector<ResTexture*>		m_picArray3;    //!< 重复任务接
};
