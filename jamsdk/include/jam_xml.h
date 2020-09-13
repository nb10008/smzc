
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			XML Parse Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_XML_H__
#define __JAM_XML_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_XML_FILE = 1,
		JAM_XML_MEMORY
	}xml_type_t;

	typedef enum
	{
		JAM_XML_NODE_UNKNOWN,
		JAM_XML_NODE_HEAD,
		JAM_XML_NODE_TAG,
		JAM_XML_NODE_COMMENT,
		JAM_XML_NODE_DATA,
		JAM_XML_NODE_DOCUMENT
	}xml_node_type_t;

	typedef handle_t						xml_doc_t;
	typedef handle_t						xml_node_t;
	typedef handle_t						xml_attr_t;

	typedef void (*jam_xml_sax_on_head)(int layer, const char* name, const char* data);
	typedef void (*jam_xml_sax_on_comment)(int layer, const char* data);
	typedef void (*jam_xml_sax_on_tag_begin)(int layer, const char* name, const char* data, xml_attr_t attr);
	typedef void (*jam_xml_sax_on_tag_data)(int layer, const char* data);
	typedef void (*jam_xml_sax_on_tag_end)(int layer, const char* name);

	typedef struct
	{
		jam_xml_sax_on_head			on_head;
		jam_xml_sax_on_comment		on_comment;
		jam_xml_sax_on_tag_begin	on_tag_begin;
		jam_xml_sax_on_tag_data		on_tag_data;
		jam_xml_sax_on_tag_end		on_tag_end;
	}xml_content_t;


	/** 读取 XML 文档 */
	JAM_API xml_doc_t jam_xml_doc_alloc(const char* str, xml_type_t type);


	/** 关闭 XML 句柄 */
	JAM_API void jam_xml_doc_free(xml_doc_t doc);


	/** 获取 XML 根节点 */
	JAM_API xml_node_t jam_xml_doc_root_node(xml_doc_t doc);


	/** 节点属性 */
	JAM_API xml_node_type_t jam_xml_node_type(xml_node_t node);


	/** 节点名称 */
	JAM_API const char* jam_xml_node_name(xml_node_t node);


	/** 节点数据 */
	JAM_API const char* jam_xml_node_data(xml_node_t node);


	/** 节点指定属性数据 */
	JAM_API const char* jam_xml_node_attr_data(xml_node_t node, const char* attr);


	/** 下一个兄弟节点 */
	JAM_API xml_node_t jam_xml_node_next(xml_node_t node);


	/** 上一个兄弟节点 */
	JAM_API xml_node_t jam_xml_node_prev(xml_node_t node);


	/** 第一个子节点 */
	JAM_API xml_node_t jam_xml_node_first_child(xml_node_t node);


	/** 最后一个子节点 */
	JAM_API xml_node_t jam_xml_node_last_child(xml_node_t node);


	/** 获取节点属性 */
	JAM_API xml_attr_t jam_xml_node_attr(xml_node_t node);


	/** 下一个属性 */
	JAM_API xml_attr_t jam_xml_attr_next(xml_attr_t attr);


	/** 属性名称 */
	JAM_API const char* jam_xml_attr_name(xml_attr_t attr);


	/** 属性数据 */
	JAM_API const char* jam_xml_attr_data(xml_attr_t attr);


	/** SAX 方式解析 XML */
	JAM_API bool jam_xml_sax_parse(const char* str, xml_type_t type, const xml_content_t* content);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_XML_H__ */
