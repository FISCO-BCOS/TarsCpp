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

#ifndef __TC_CGI_H
#define __TC_CGI_H

#include <sstream>
#include <istream>
#include <map>
#include <vector>
#include "util/tc_ex.h"

namespace tars
{
/////////////////////////////////////////////////
/** 
* @file tc_cgi.h
* @brief CGI处理类
* @brief CGI Processing Class

*/            
/////////////////////////////////////////////////
class TC_Cgi;
class TC_Cgi_Upload;
class TC_HttpRequest;

/**
* @brief 配置文件异常类
* @brief Profile Exception Class
*/
struct TC_Cgi_Exception : public TC_Exception
{
    TC_Cgi_Exception(const std::string &buffer) : TC_Exception(buffer){};
    TC_Cgi_Exception(const std::string &buffer, int err) : TC_Exception(buffer, err){};
    ~TC_Cgi_Exception() throw(){};
};

/**
* @brief 全局的友元函数，定义该函数, 
*        则可以用TC_Common::tostr对 std::vector<TC_Cgi_Upload>进行输出操作
* @brief Global Friend. 
*   By defining this function, you can use TC_Common::tostr to output std::vector<TC_Cgi_Upload> 
*/
std::ostream &operator<<(std::ostream &os, const TC_Cgi_Upload &tcCgiUpload);

/**
* @brief cgi上传文件操作，通过该类获取cgi上传的文件信息 
* @brief CGI Uploaded File Operation, you can get cgi uploaded file info from this class.
*/
class TC_Cgi_Upload
{
public:
    friend std::ostream &operator<<(std::ostream &os, const TC_Cgi_Upload &tcCgiUpload);

    /**
    * @brief 构造函数
    * @brief Constructor
    */
    TC_Cgi_Upload()
    :_sFileName("")
    , _sRealFileName("")
    , _sServerFileName("")
    , _iSize(0)
    , _bOverSize(false)
    {
    }

    /**
    * @brief 拷贝构造函数. 
    * @brief Copy Constructor
    */

    TC_Cgi_Upload(const TC_Cgi_Upload &tcCgiUpload);

    /**
    * @brief 赋值构造函数
    * @brief Assignment Constructor
    */
    TC_Cgi_Upload & operator=(const TC_Cgi_Upload &tcCgiUpload);

    /**
    * @brief 获取上传的信息. 
    * @brief Get Uploaded Info 
    * @return 上传文件的信息
    * @return Uploaded File Info
    */
    std::string tostr() const;

    /**
    * @brief 获取客户端IE INPUT上传控件的名称. 
    * @brief Get the Client IE ; Input the name of the upload control
    * @return INPUT上传控件名称
    * @return INPUT the name of the upload control
    */
    std::string getFormFileName() const
    {
        return _sFileName;
    }

    /**
    * @brief 返回INPUT控件用户输入的名称,及客户端真实的文件名称. 
    * @brief Return the name from the imput control and the real file name from the Client. 
    * @return  客户端真实的文件名称
    * @return the real file name from the Client
    */
    std::string retRealFileName() const
    {
        return _sRealFileName;
    }

    /**
    * @brief 上传到服务器后,服务器端文件名称. 
    * @brief The server file name after uploading to the server.
    * @return 服务器端文件名称
    * @return the file name from the Server.
    */
    std::string getSeverFileName() const
    {
        return _sServerFileName;
    }

    /**
    * @brief 获取上传的文件大小. 
    * @brief Get the size of the uploaded file. 
    * @return size_t类型，上传的文件大小
    * @return size_t : the size of the uploaded file
    */
    size_t getFileSize() const
    {
        return _iSize;
    }

    /**
    * @brief 上传的文件是否超过大小. 
    * @brief Determine whether the size of the uploaded file is oversize or not. 
    * @return 超过大小返回true，否则返回false
    * @return bool: oversize, true ; not oversize, false
    */
    bool isOverSize() const
    {
        return _bOverSize;
    }

protected:

    /**
    * 上传文件,浏览器file控件名称
    * Upload files,the name of the file control on the browser.
    */
    std::string  _sFileName;

    /**
    * 上传文件真实名称,客户端的文件名称
    * the real name of the uploaded file, the name of the file on the Client.
    */
    std::string  _sRealFileName;

    /**
    * 上传文件服务器端名称
    * the name of the server which uploaded the file
    */
    std::string  _sServerFileName;

    /**
    * 上传文件大小,字节数
    * the size of the uploaded file, Bytes
     */
    size_t  _iSize;

    /**
    * 上传文件是否超过大小
    * whether the uploaded file is oversize or not
    */
    bool    _bOverSize;

    friend class TC_Cgi;
};

/**
* @brief cgi操作相关类. 
* @brief CGI Operation Related Classes
*  
* 主要操作包括： 
* Main includes: 
* 1 支持参数解析 
* 1 Support parameter resolution 
* 2 支持cookies解析 
* 2 Support cookies resolution 
* 3 支持文件上传,设置上传文件的最大个数,文件的最大大小 
* 3 Support file uploading, setting the max number of uploaded files and the max size of a file. 
* 4 上传文件时, 需要检查文件是否超过最大大小 
* 4 When uploading files, you need to check whether the files are oversize or not. 
* 5 上传文件时, 需要检查上传文件个数是否限制
* 5 When uploading files, you need to check 
*       whether the number of the uploaded files is limited. 
* 说明:多个文件同时上传时,浏览器的file控件必须取不同name,否则将无法正确的上传文件 
* Notes : When multiple files are uploaded at the same time, 
*   the browser's file control must take a different name, 
*   otherwise the file will not be uploaded correctly.
* 注意:调用parseCgi解析标准输入,
* Atttention :  When calling parseCgi to parse the standard input, 
* 如果有文件上传需要调用setUpload, 并且需要在parseCgi之前调用
* setUpload, which is called by the uploaded files, needs to be called before parseCgi.  
*/
class TC_Cgi
{
public:

    /**
    * @brief TC_Cgi构造函数
    * @brief TC_Cgi Constructor
    */
    TC_Cgi();

    /**
    * @brief 析构函数
    * @brief Destructor
    */
    virtual ~TC_Cgi();

    /**
    * @brief 定义环境变量
    * @brief Define the environment variables
    */
    enum
    {
        ENM_SERVER_SOFTWARE,
        ENM_SERVER_NAME,
        ENM_GATEWAY_INTERFACE,
        ENM_SERVER_PROTOCOL,
        ENM_SERVER_PORT,
        ENM_REQUEST_METHOD,
        ENM_PATH_INFO,
        ENM_PATH_TRANSLATED,
        ENM_SCRIPT_NAME,
        ENM_HTTP_COOKIE,
        ENM_QUERY_STRING,
        ENM_REMOTE_HOST,
        ENM_REMOTE_ADDR,
        ENM_AUTH_TYPE,
        ENM_REMOTE_USER,
        ENM_REMOTE_IDENT,
        ENM_CONTENT_TYPE,
        ENM_CONTENT_LENGTH,
        ENM_HTTP_USER_AGENT
    };

    /**
     * @brief 设置上传文件.
     * @brief Set the uploaded file. 
     *  
     * @param sUploadFilePrefix, 文件前缀(包含路径), 如果有文件上传,则文件保存在以该前缀为名称的路径下
     *                           如果有多个文件上传,则文件名称以次在后面加"_序号"
     *  The file prefix (includes its path). If a file is uploaded, the file is saved in the path with the prefix as the name.
     *  If more than one file is uploaded, then the file name will be followed by "_ No."
     * 
     * @param iMaxUploadFiles    最多上传文件个数,<0:没有限制
     * Maximum number of uploaded files, if iMaxUploadFiles < 0 : no limit
     * 
     * @param iUploadMaxSize     每个文件上传的最大大小(字节)
     * Maximum size uploaded per file (Bytes)
     */
    void setUpload(const std::string &sUploadFilePrefix, int iMaxUploadFiles = 5, size_t iUploadMaxSize = 1024*1024*10, size_t iMaxContentLength = 1024*1024*10);

    /**
     * @brief 从标准输入解析cgi. 
     * @brief Analyze CGI from standard input
     */
    void parseCgi();

    /**
     * @brief 直接从http请求解析. 
     * @brief Request parsing directly from http.
     *  
     * @param request http请求
     * @param request http request
     */
    void parseCgi(const TC_HttpRequest &request);

    /**
    * @brief 获取cgi的url参数multimap. 
    * @brief Get the URL parameter of CGI : Multimap
    *  
    * @return std::multimap<std::string, std::string> cgi的url参数
    * @return std::multimap<std::string, std::string> the URL parameter of CGI
    */
    const std::multimap<std::string, std::string> &getParamMap() const;

    /**
     * @brief 获取cgi环境变量map.
     * @brief Get CGI environment variable: Map
     *
     * @return std::map<std::string,string> cgi的环境变量
     * @return std::map<std::string,string> the environment variable of CGI
     */
    std::map<std::string, std::string> getEnvMap() const { return _env; }

    /**
    * @brief 获取cgi的参数map, 将multimap转换成map返回 
    *        , 对于一个参数名称对应多个参数值的情况, 只取其中一个值.
    * @brief Get the CGI parameter, map, and convert Multimap to Map as return.
    *   If a parameter name corresponds to more than one parameter value, only one value is taken.
    * 
    * @return std::map<std::string, std::string>
    */
    std::map<std::string, std::string> getParamMapEx() const;

    /**
    * @brief 获取cookies的参数map. 
    * @brief Get cookies' parameter : map
    *  
    * @return std::map<std::string, std::string>
    */
    const std::map<std::string, std::string> &getCookiesMap() const;

    /**
    * @brief 获取cgi的某个参数. 
    * @brief Get a certain parameter from CGI
    *  
    * @param sName  参数名称 parameter name
    * @return       
    */
    std::string &operator[](const std::string &sName);

    /**
    * @brief 获取cgi的某个参数. 
    * @brief Get a certain parameter from CGI
    *  
    * @param sName 参数名称 parameter name 
    * @return      参数的值 the value of the parameter
    */
    std::string getValue(const std::string& sName) const;

    /**
    * @brief 获取某一名称的参数的多个值. 
    * @brief Get multiple values of a parameter with a name.
    *  
    * 用于解析checkbox这类控件的值( 一个参数名,多个参数值)
    * This is used to parse the value of a control such as a checkbox (one parameter name, multiple parameter values)
    * @param sName   参数名称 / the parameter name
    * @param vtValue 该名称的参数值组成的vector / vector composed of the parameter value of the name 
    * @return        std::vector<std::string>, 该名称的参数值组成的vector 
    * @return        std::vector<std::string>  vector composed of the parameter value of the name
    */
    const std::vector<std::string> &getMultiValue(const std::string& sName, std::vector<std::string> &vtValue) const;

    /**
    * @brief 获取cookie值. 
    * @brief Get the cookie value.
    *  
    * @param sName cookie名称 / the name of the cookie
    * @return      string类型的cookie值 / Cookie value of type string
    */
    std::string getCookie(const std::string& sName) const;

    /**
    * @brief 设置cookie值. 
    * @brief Set cookie value.
    *  
    * @param sName    cookie名称 / cookie name
    * @param sValue   cookie值 / cookie value
    * @param sExpires 过期日期 / the expire date
    * @param sPath    cookie有效路径 / cookie valid path
    * @param sDomain  cookie有效域 / cookie valid domain
    * @param bSecure  是否安全(ssl时有效) / Whether it is secure (valid in SSL)
    * @return         返回字符串，代表cookie值 / returns a std::string which representing the cookie value
    */
    std::string setCookie(const std::string &sName, const std::string &sValue, const std::string &sExpires="", const std::string &sPath="/", const std::string &sDomain = "", bool bSecure = false);

    /**
    * @brief 参数链表是否为空. 
    * @brief Whether the parameter list is empty.
    *  
    * @return 参数链表为空返回true，否则返回false
    * @return bool : true, the parameter list is empty ; false, the parameter list is not empty 
    */
    bool isParamEmpty() const;

    /**
    * @brief 参数是否存在. 
    *        Wether the parameter exists or not.
    *  
    * @param sName 参数名称 / the parameter name
    * @return      存在返回true，否则返回false / if exists retrun true, else return false.
    */
    bool isParamExist(const std::string& sName) const;

    /**
    * @brief 上传文件是否超过大小，多个文件上传时, 
    *        只要有一个文件超过大小,则超过
    *        Wheter the size of the uploaded file is scale out.
    *        When uploading multiple files, it is scale out as long as there's one file exceeds the size.
    * @return 按照以上标准超过大小的返回true，否则返回false
    *        According to the standard above, if it is oversize, return true, else return false.
    */
    bool  isUploadOverSize() const;

    /**
    * @brief 上传文件是否超过大小,多个文件上传时, 
    *        只要有一个文件超过大小,则超过
    *        Wheter the size of the uploaded file is scale out.
    *        When uploading multiple files, it is scale out as long as there's one file exceeds the size. 
    * @param vtUploads 返回超过大小的文件名称(浏览器file控件的名称)
    *                  Returns the name of a file larger than the size (the name of the browser file control)
    * @return          按照以上标准超过大小的返回true，否则返回false
    *                  According to the standard above, if it is oversize, return true, else return fals.
    */
    bool  isUploadOverSize(std::vector<TC_Cgi_Upload> &vtUploads) const;

    /**
     * @brief 是否超过上传文件个数.
     *        Wheter the number of uploaded files exceeds.
     *
     * @return 超过上传个数返回true，否则返回false
     *         bool : true, the number of the uploaded files exceeds ; false, else
     */
    bool isOverUploadFiles() const { return _bOverUploadFiles; }

    /**
    * @brief 获取上传文件个数. 
    *         Get the number of uploaded files
    *  
    * @return size_t上传文件的个数
    *         size_t the number of uploaded files
    */
    size_t getUploadFilesCount() const;

    /**
    * @brief 获取上传文件的相关信息
    *         Get the related info about the uploaded files
    *  
    * @return std::map<std::string,TC_Cgi_Upload>结构中， 
    *         保存文件名和文件相关信息的map
    *         In the structure std::map<std::string,TC_Cgi_Upload>, 
    *         the map which stored the file name and  the file's related information.
    */
    const std::map<std::string, TC_Cgi_Upload> &getUploadFilesMap() const;

    /**
    * @brief 获取环境变量. 
    *        Get the environment variables
    *  
    * @param iEnv  枚举变量 / Enumerating variables
    * @return      环境变量 / Environment variables
    */
    std::string getCgiEnv(int iEnv);

    /**
    * @brief 获取环境变量. 
    *        Get the environment variables
    *  
    * @param sEnv 环境变量名称 / the environment variable name
    * @return     环境变量的值 / the envitonment variable name
    */
    std::string getCgiEnv(const std::string& sEnv);

    /**
     * @brief 设置环境变量. 
     *         Set environment variales
     *  
     * @param sName 环境变量名称 / environment variable name
     * @param sValue 环境变量的值 / environment variable value
     */
    void setCgiEnv(const std::string &sName, const std::string &sValue);

    /**
    * @brief 返回html头，content-type .
    *        Return HTML header, content type 
    *  
    * @param sHeader 缺省值为"text/html" 
    *                The default value is "text / HTML"
    * @return 
    */

    static std::string htmlHeader(const std::string &sHeader = "text/html");

    /**
    * @brief http请求的url解码, %后面的换成字符. 
    *        The URL of the HTTP request is decoded and the one after% is replaced with a character.
    * 
    * @param sUrl http请求url / HTTP request URL
    * @return    解码后的字符串 / the encoded string
    */
    static std::string decodeURL(const std::string &sUrl);

    /**
     * @brief 对url进行编码, 非数字和字母用%XX代替. 
     *        Encode the URL with% XX instead of numbers and letters.
     *         
     * @param sUrl http请求url / HTTP request URL
     * @return     编码后的url / the encoded string
     */
    static std::string encodeURL(const std::string &sUrl);

    /**
     * @brief 对源字符串进行HTML编码(<>"&) 
     *        HTML encoding of the source std::string (< > "& ').
     * 
     * the source std::string 
     * @param src         源字符串
     * 
     * Whether encoding the 'space' (space, \t, \r\n, \n)
     * @param blankEncode 是否对空格也编码(空格, \t, \r\n, \n)
     * 
     * HTML encoded std::string                    
     * @return            HTML编码后的字符串
     */
    static std::string encodeHTML(const std::string &src, bool blankEncode = false);

    /**
     * @brief 对源字符串进行XML编码(<>"&'). 
     *        XML encoding of the source std::string (< > "& ').
     * @param src  源字符串 / Source string
     * @return     XML编码后的字符串 / XML encoded string
     */
    static std::string encodeXML(const std::string &src);

protected:

    /**
    * @brief 申明,但是不定义,保证这个函数不会被使用
    *        Declare, but do not define, to ensure that this function will not be used.
    */
    TC_Cgi &operator=(const TC_Cgi &tcCgi);

    /**
    * @brief GET method. 
    *  
    * @param sBuffer GET的QueryString / the QueryString of GET
    * return 
    */
    void getGET(std::string &sBuffer);

    /**
    * @brief POST method. 
    *  
    * @param sBuffer   POST的QueryString / the QueryString of POST
    * return 
    */
    void getPOST(std::string &sBuffer);

    /**
    * @brief 解析文件上传.
    *        Parsing file upload
    *  
    * @param mmpParams  [out]输出参数multimap / output std::multimap
    * return 
    */
    void parseUpload(std::multimap<std::string, std::string> &mmpParams);

    /**
     * @brief 解析form数据
     *        Parsing form data
     */
    void parseFormData(std::multimap<std::string, std::string> &mmpParams, const std::string &sBoundary);

    /**
     * @brief  忽略空行
     *          Ignore empty lines
     */
    void ignoreLine();

    /**
     * @brief 写文件. 
     *        Write File
     * @param sFileName 文件名称 / file name
     * @param sBuffer   要写入的内容 / contents needs to be wrote.
     */
    bool writeFile(FILE*fp, const std::string &sFileName, const std::string &sBuffer, size_t &iTotalWrite);

    /**
    * @brief 非上传模式下解析. 
    *        Analysis in non upload mode
    * @param mmpParams  [out]输出参数multimap / output std::multimap
    * @param sBuffer    [in]输入QueryString / output QueryString
    * return 
    */
    void parseNormal(std::multimap<std::string, std::string> &mmpParams, const std::string& sBuffer);

    /**
    * @brief 解析cookies. 
    *        Parsing cookies
    *  
    * @param mpCooies [out]输出cookiesmap / output mpCookies
    * @param sBuffer  [in]输入Cookies字符串/ input std::string Cookies
    * return
    */
    void parseCookies(std::map<std::string, std::string> &mpCooies, const std::string& sBuffer);

    /**
    * @brief 控制解析cgi input的基本流程. 
    *        Control the basic process of parsing CGI input
    *  
    * @param mmpParams [out]输出参数 std::multimap / output std::multimap
    * @param mpCooies [out]输出cookies / output std::multimap
    * return 
    */
    void readCgiInput(std::multimap<std::string, std::string> &mmpParams, std::map<std::string, std::string> &mpCooies);

protected:

    /**
     * buffer
     */
    std::string                      _buffer;

    /**
     * 流
     * Stream
     */
    std::istringstream               _iss;

    /**
     * 读入
     * Read in
     */
    std::istream                     *_is;

    /**
     * 环境变量
     * environmet variable
     */
    std::map<std::string, std::string>         _env;

    /**
    * cgi参数
    * CGI parameter
    */
    std::multimap<std::string, std::string>    _mmpParams;

    /**
    * cookies
    */
    std::map<std::string, std::string>         _mpCookies;

    /**
    * 上传文件名称前缀
    * Upload file name prefix
    */
    std::string                      _sUploadFilePrefix;

    /**
    * 上传文件的最大个数,<0:无限制
    * Maximum number of uploaded files, < 0: Unlimited
    */
    int                         _iMaxUploadFiles;

    /**
    * 上传文件的最大大小
    * Maximum size of uploaded file
    */
    size_t                      _iUploadMaxSize;

    /**
     * 是否超过上传文件个数
     * Whether the number of the uploaded file is exceeded.
     */
    bool                        _bOverUploadFiles;

    /**
     * 最大的content-length
     * the max content-length
     */
    size_t                      _iMaxContentLength;

    /**
    * 是否超过大小,有一个文件超过则超过
    * Determine whether the file is oversize or not, as long as one file is oversize that _bUploadFileOverSize is ture 
    */
    bool                        _bUploadFileOverSize;

    /**
    * 上传文件相关信息保存在该map中
    * the related infomation about the uploaded files is stored in this map
    */
    std::map<std::string, TC_Cgi_Upload>  _mpUpload;
};

}

#endif
