#include "..\stdafx.h"
#include ".\MaxSkin.h"
#include "..\CSExport.h"

MaxSkin::MaxSkin(INode *pNode,TimeValue time):MaxMesh(pNode,time)
{
	//m_pModifier		= Max_FindPhysiqueModifier(pNode);
	m_pModifier			= NULL;
	m_maxVertBoneInf=0;
	m_pPhysiqueExport=NULL;
	m_pContextExport=NULL;

	m_pSkin				= NULL;
	m_pSkinMC			= NULL;
}

MaxSkin::~MaxSkin()
{

}

void MaxSkin::BeginPhysiqueExport()
{
	
	Modifier* skinModifier	= FindSkinModifier( m_pINode );
	Modifier* phyModifier	= FindPhysiqueModifier( m_pINode );

	if( skinModifier && phyModifier ) {
		g_msgBox.ShowMsg( _T("??????Skin?Physique??\r\n") );
		return;
	}

	if( !skinModifier && !phyModifier ) {
		g_msgBox.ShowMsg(_T("????\r\n"));
	}

	/*if(m_pModifier==NULL) {
		g_msgBox.ShowMsg(_T("WARNING:Modifier not found,node name=%s.\r\n"),m_pINode->GetName());
		return;
	}*/

	if( skinModifier ) {
		m_pModifier = skinModifier;

		m_pSkin = (ISkin*)m_pModifier->GetInterface(I_SKIN);
		m_pSkinMC = (ISkinContextData*)m_pSkin->GetContextInterface( m_pINode );

	} else if( phyModifier ) {
		
		m_pModifier = phyModifier;
		m_pPhysiqueExport = (IPhysiqueExport*)m_pModifier->GetInterface(I_PHYINTERFACE);
		m_pContextExport = (IPhyContextExport*)m_pPhysiqueExport->GetContextInterface( m_pINode );

		m_pContextExport->ConvertToRigid(TRUE);
		m_pContextExport->AllowBlending(TRUE);
	}


	// create a physique export interface
	/*m_pPhysiqueExport=(IPhysiqueExport *)m_pModifier->GetInterface(I_PHYINTERFACE);
	if(m_pPhysiqueExport==0) {
		g_msgBox.ShowMsg(_T("Physique modifier interface not found.\r\n"));
		return;
	}*/

	// create a context export interface
	/*m_pContextExport = (IPhyContextExport *)m_pPhysiqueExport->GetContextInterface(m_pINode);
	if(m_pContextExport == 0) {
		m_pModifier->ReleaseInterface(I_PHYINTERFACE, m_pPhysiqueExport);
		g_msgBox.ShowMsg(_T("Context export interface not found.\r\n"));
		return;
	}*/

	//// set the flags in the context export interface
	//m_pContextExport->ConvertToRigid(TRUE);
	//m_pContextExport->AllowBlending(TRUE);
}

void MaxSkin::EndPhysiqueExport(){
	// release all interfaces
	/*if(m_pPhysiqueExport!=NULL) {
		m_pPhysiqueExport->ReleaseContextInterface(m_pContextExport);
		m_pPhysiqueExport=NULL;
	}

	if(m_pModifier!=NULL) {
		m_pModifier->ReleaseInterface(I_PHYINTERFACE, m_pPhysiqueExport);
		m_pModifier=NULL;
	}*/
	if( m_pPhysiqueExport != NULL ) {
		m_pPhysiqueExport->ReleaseContextInterface( m_pContextExport );
		m_pModifier->ReleaseInterface( I_PHYINTERFACE, m_pPhysiqueExport );
		m_pContextExport = NULL;
		m_pPhysiqueExport = NULL;
	}
	if( m_pSkin ) {
		m_pModifier->ReleaseInterface( I_SKIN, m_pSkin );
		m_pSkinMC = NULL;
		m_pSkin = NULL;
	}
}

//void MaxSkin::Vert_GetBoneInf(int vertexId,BoneInfArray& binfArray) {
//	if(m_pPhysiqueExport==NULL || m_pContextExport==NULL) {
//		return;
//	}
//
//	//--
//	binfArray.clear();	
//
//	// get the vertex export interface
//	IPhyVertexExport *pVertexExport;
//	pVertexExport=(IPhyVertexExport *)m_pContextExport->GetVertexInterface(vertexId);
//	if(pVertexExport==0) {
//		m_pPhysiqueExport->ReleaseContextInterface(m_pContextExport);
//		m_pModifier->ReleaseInterface(I_PHYINTERFACE, m_pPhysiqueExport);
//		g_msgBox.ShowMsg(_T("Vertex export interface not found.\r\n"));
//		return;
//	}
//
//	// get the vertex type
//	int vertexType;
//	vertexType = pVertexExport->GetVertexType();
//	// handle the specific vertex type
//	if(vertexType==RIGID_TYPE) {
//		// typecast to rigid vertex
//		IPhyRigidVertex* pTypeVertex;
//		pTypeVertex = (IPhyRigidVertex *)pVertexExport;
//
//		BoneInfluence binf;
//		binf.weight=1.0f;
//		binf.szBoneName=pTypeVertex->GetNode()->GetName();
//		binf.offset=pTypeVertex->GetOffsetVector();
//
//		// ?????????? [12/19/2007 leili]
//		binfArray.push_back(binf);
//	} else if(vertexType==RIGID_BLENDED_TYPE) {
//		// typecast to blended vertex
//		IPhyBlendedRigidVertex *pTypeVertex;
//		pTypeVertex = (IPhyBlendedRigidVertex *)pVertexExport;
//
//		// loop through all influencing bones
//		int nodeId;
//		int numNodes=pTypeVertex->GetNumberNodes();
//		for(nodeId=0;nodeId<numNodes;nodeId++)
//		{
//			// add the influence to the vertex candidate
//			BoneInfluence binf;
//			binf.weight=pTypeVertex->GetWeight(nodeId);
//
//			//~ zrw changed GetNode?????
//			//binf.szBoneName=pTypeVertex->GetNode(nodeId)->GetName();
//			INode* node = pTypeVertex->GetNode( nodeId );
//			if( node ) {
//				binf.szBoneName = node->GetName();
//				binf.offset=pTypeVertex->GetOffsetVector(nodeId);
//				AddBoneInf(binfArray,binf);
//			}
//		}
//	}
//	else if(vertexType == DEFORMABLE_TYPE)
//	{
//		// ?????
//	}
//	else
//	{
//		g_msgBox.ShowMsg(_T("WARNING:Unknown physique vertex type.\r\n"));
//	}
//
//	if(binfArray.size()>m_maxVertBoneInf) {
//		m_maxVertBoneInf=binfArray.size();
//	}		
//}

void MaxSkin::AddBoneInf(BoneInfArray& binfArray,BoneInfluence& binf) {
	// ????????? [12/19/2007 leili]
	// offset ???????
	for( size_t i = 0; i<binfArray.size(); i++ ) {
		BoneInfluence& t = binfArray[i];
		if(t.szBoneName == binf.szBoneName) {
			t.weight += binf.weight;
			return;
		}
	}
	binfArray.push_back(binf);
}

void MaxSkin::GetSkinBoneInf( int vertID, ISkin* skin, ISkinContextData* skinMC, BoneInfArray& binfArray ) {
	int numAssignedBone = skinMC->GetNumAssignedBones(vertID);
	
	//if (numAssignedBone > 4) numAssignedBone = 4;

	for( int boneId = 0; boneId < numAssignedBone; ++boneId ) {
		// get bone node id and weight
		int nBoneIdx  = skinMC->GetAssignedBone(vertID, boneId);
		float fWeight = skinMC->GetBoneWeight(vertID, boneId);

		// skip invalid weight
		if (fWeight <= 0.f) {
			continue;
		}

		// get bone node by node id
		INode* pBoneNode = skin->GetBone(nBoneIdx);

		BoneInfluence binf;
		binf.weight		= fWeight;
		binf.szBoneName = pBoneNode->GetName();
		//! ???????INode????. ???vertID???????. ?????????offset
		binf.offset = Point3( 0.0f, 0.0f, 0.0f );

		binfArray.push_back(binf);
	}

}

void MaxSkin::GetPhyBoneInf( int vertID, IPhyContextExport* mcExport, BoneInfArray& binfArray ) {
	// get the vertex export interface
	IPhyVertexExport* pVertexExport = (IPhyVertexExport*)mcExport->GetVertexInterface(vertID);
	if( !pVertexExport ) {
		g_msgBox.ShowMsg(_T("Vertex export interface not found.\r\n"));
		return;
	}

	// get the vertex type
	int vertexType = pVertexExport->GetVertexType();
	
	// handle the specific vertex type
	if( vertexType == RIGID_TYPE ) {
		
		// typecast to rigid vertex
		IPhyRigidVertex* pTypeVertex = (IPhyRigidVertex *)pVertexExport;

		BoneInfluence binf;
		binf.weight		= 1.0f;
		binf.szBoneName = pTypeVertex->GetNode()->GetName();
		binf.offset		= pTypeVertex->GetOffsetVector();
		binfArray.push_back(binf);

	} else if( vertexType == RIGID_BLENDED_TYPE ) {
		
		// typecast to blended vertex
		IPhyBlendedRigidVertex* pTypeVertex = (IPhyBlendedRigidVertex *)pVertexExport;

		// loop through all influencing bones
		int numNodes = pTypeVertex->GetNumberNodes();
		for( int nodeId = 0; nodeId < numNodes; ++nodeId ) {
			// add the influence to the vertex candidate
			BoneInfluence binf;
			binf.weight	= pTypeVertex->GetWeight(nodeId);
			INode* node = pTypeVertex->GetNode( nodeId );
			if( node ) {
				binf.szBoneName = node->GetName();
				binf.offset		= pTypeVertex->GetOffsetVector(nodeId);
				AddBoneInf(binfArray,binf);
			}
		}
	} else {
		g_msgBox.ShowMsg(_T("??????????.\r\n"));
	}
}

void MaxSkin::Vert_GetBoneInf( int vertexId, BoneInfArray& binfArray) {
	if( !m_pModifier ) {
		return;
	}

	binfArray.resize(0);

	if( m_pSkin && m_pSkinMC ) {
		GetSkinBoneInf( vertexId, m_pSkin, m_pSkinMC, binfArray );
	} else if( m_pPhysiqueExport && m_pContextExport ) {
		GetPhyBoneInf( vertexId, m_pContextExport, binfArray );
	}

	if( binfArray.size() > m_maxVertBoneInf ) {
		m_maxVertBoneInf = binfArray.size();
	}	
}