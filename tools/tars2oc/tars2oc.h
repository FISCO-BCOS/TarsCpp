/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */
#ifndef _TARS2OC_H
#define _TARS2OC_H

#include "parse.h"

#include <cassert>
#include <string>

using namespace tars;

/**
 * ����tars����h��m�ļ�
 */
class Tars2OC
{

public:

    /**
     * ����
     * @param file
     */
    void createFile(const std::string &file);


    /**
    * ���������ļ���Ŀ¼
    *
    */
    void setBaseDir(const std::string & sPath) { m_sBaseDir = sPath; }


	/**
	 * �����Ƿ���Ĭ��ֵ
	 *
	 * @param bCheck
	 */
	void setArc(const bool bArc) { m_bARC = bArc; }
	/**
	 * ���ø�����Ա�����Ƿ�������ֿռ�ǰ׺
	 */
	void setNeedNS(bool bNeed) { m_bNeedNS = bNeed;}
    /**
	 * �����û��Զ������ֿռ�
	 */
	void setNS(const std::string& sNS) {m_sNamespace = sNS;}
    //����������������Դ������
protected:
	/**
	 * ��ȡö�ٻ��߽ṹ������ǰ׺�ַ���
	 * @param sTarsNS  tars�ļ��ж�������ֿռ�
	 * @param bEnum �Ƿ���ö������
	 *
	 * @return �������ǰ׺�ַ���
	 */
	string getNamePrix(const std::string& sTarsNS) const;
	
	/**
	 * ��ȡ���Ϲ淶�ĳ�Ա����
	 */
	string getPropertyName(const std::string& sId) const;

    /**
	 *
	 * ��ȡ�Զ���setter������
	 */
	string getSetterName(const std::string& sId) const;
    /**
     * ����ĳ���͵��ַ�������Դ��
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostr(const TypePtr &pPtr) const;

    /**
     * �����ڽ����͵��ַ���Դ��
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostrBuiltin(const BuiltinPtr &pPtr) const;
    /**
     * ����vector���ַ�������
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostrVector(const VectorPtr &pPtr) const;

    /**
     * ����map���ַ�������
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostrMap(const MapPtr &pPtr) const;

    /**
     * ����ĳ�ֽṹ�ķ�������
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostrStruct(const StructPtr &pPtr) const;


    /**
     * ����ĳ��ö�ٵķ�������
     * @param pPtr
     *
     * @return std::string
     */
    std::string tostrEnum(const EnumPtr &pPtr) const;

	/**
	 * ����OBJC �汾2�����Զ���
	 * @param pPtr
	 *
	 * @return std::string
	 */
	string toTarsV2Procstr(const TypeIdPtr &pPtr) const;

	/**
	 * ����OBJC�汾2�ĸ�����Ϣ����
	 * @param pPtr
	 *
	 * @return std::string
	 */
	string toAddtionalClassName(const TypePtr &pPtr) const;

	/**
	 * ���ɽṹ�����Ե�synthesize����
	 * @param vMember
	 *
	 * @return std::string
	 */
    std::string writesynthesize(const vector<TypeIdPtr>& vMember) const;

	/**
	 * ����Initialize����
	 * @param pPtr
	 *
	 * @return std::string
	 */
    std::string writeInitialize(const StructPtr &pPtr) const;

	/**
	 * ���ɳ�ʼ�����庯��init
	 * @param vMember
	 *
	 * @return std::string
	 */
	string writeInit(const vector<TypeIdPtr>& vMember) const;

	/**
	 * ������������
	 * @param vMember
	 *
	 * @return std::string
	 */
	string writedealloc(const vector<TypeIdPtr>& vMember) const;

	/**
	 * ���ɽṹ���ƶ���
	 * @param pPtr
	 *
	 * @return std::string
	 */
	string writeTarsType(const StructPtr &pPtr) const;

	/**
	 * �������ͱ�ʾ��
	 * @param pPtr
	 *
	 * @return std::string
	 */
    std::string toClassName(const TypePtr &pPtr) const;
protected:
    /**
	 * ��ȡtars�ж�����������ͣ�require��optional
	 * @param pPtr
	 *
	 */
	string getReqOptionType(const TypeIdPtr &pPtr) const;
	/**
	 * �ж������Ƿ�����retain���ͣ�string�Ͷ������Ͷ�����retain
	 * @param pPtr
	 *
	 */
	bool IsRetainType(const TypePtr &pPtr) const;

    bool isNumberType(const TypePtr &pPtr) const;
	/**
	 * ��ȡ�ṹ���������������͵ı�ʶ�����ƺ����ͣ�Ŀǰ�ǽṹ���ö����������
	 *
	 * @param pPtr
	 * @param namespaceId
	 *
	 * @return
	 */
	map<string,int> getReferences(const StructPtr &pPtr, const std::string& namespaceId="") const;

	/**
	 * ��ȡ��Ա�������������ͱ�ʶ������,
	 * ����, stHello hello;stHelloΪ���ͱ�ʶ������,����Ϊ�ṹ��
	 *
	 * @param pPtr
	 * @param mRefs
	 */
	void toIncludeName(const TypePtr &pPtr,map<string,int>& mRefs) const;

    /**
     * ���ɽṹ��ͷ�ļ�����
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateH(const StructPtr &pPtr, const std::string& namespaceId) const;

    /**
     * ���ɽṹ��m�ļ�����
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateM(const StructPtr &pPtr, const std::string& namespaceId) const;

    /**
     * ����ö�ٵ�m�ļ�����
     * @param pPtr
     *
     * @return std::string
     */
	string generateM(const EnumPtr &pPtr, const std::string& namespaceId) const;
    /**
     * ����������ͷ�ļ�Դ��
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateH(const ContainerPtr &pPtr) const;

    /**
     * ����ö�ٵ�ͷ�ļ�Դ��
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateH(const EnumPtr &pPtr, const std::string& namespaceId) const;

    /**
     * �������ֿռ��ͷ�ļ���ʵ��Դ��
     * @param pPtr
     *
     * @return std::string
     */
    std::string generate(const NamespacePtr &pPtr) const;

    /**
     * �������ֿռ�m�ļ�Դ��
     * @param pPtr
     *
     * @return std::string
     */
    std::string generateM(const NamespacePtr &pPtr) const;

    /**
     * �������ֿռ��ͷ�ļ�Դ��
     * @param pPtr
     *
     * @return std::string
     */
    void generate(const ContextPtr &pPtr) const;

    /**
     * �������ֿռ�m�ļ�Դ��
     * @param pPtr
     *
     * @return std::string
     */
    void generateM(const ContextPtr &pPtr) const;

private:
	enum EM_TYPE
	{
		EM_STRUCT_TYPE = 1,
		EM_ENUM_TYPE = 2
	};
    std::string m_sBaseDir;

    //<modulename,EM_TYPE>, 2:enum,1,stuct;
    mutable map<string,int> m_mIncludes;
	/**
	 * �Ƿ�֧��arc�汾
	 *
	 */
	bool m_bARC;
	/**
	 * �Ƿ�֧���������ֿռ�ǰ׺
	 */
	bool m_bNeedNS;
	/**
	 *�û��Զ�������ֿռ�ǰ׺�������ֵΪ�գ�Ĭ����ȡtars�ļ��ж����ֵ
	 */
	string m_sNamespace;
};

#endif


