
#ifndef __QUERY_IMP_H__
#define __QUERY_IMP_H__

#include "DbHandle.h"
#include "servant/QueryF.h"

using namespace tars;

//////////////////////////////////////////////////////

enum FUNID {
  FUNID_findObjectById = 0,
  FUNID_findObjectById4Any = 1,
  FUNID_findObjectById4All = 2,
  FUNID_findObjectByIdInSameGroup = 3,
  FUNID_findObjectByIdInSameStation = 4,
  FUNID_findObjectByIdInSameSet = 5
};

//////////////////////////////////////////////////////
/**
 * 对象查询接口类
 */
class QueryImp : public QueryF {
public:
  /**
   * 构造函数
   */
  QueryImp(){};

  /**
   * 初始化
   */
  virtual void initialize();

  /**
   ** 退出
   */
  virtual void destroy(){};

  /**
   * 根据id获取所有该对象的活动endpoint列表
   */
  virtual std::vector<EndpointF> findObjectById(const std::string &id,
                                                tars::CurrentPtr current);

  /**
   * 根据id获取所有对象,包括活动和非活动对象
   */
  virtual tars::Int32 findObjectById4Any(
      const std::string &id, std::vector<tars::EndpointF> &activeEp,
      std::vector<tars::EndpointF> &inactiveEp, tars::CurrentPtr current);

  /**
   * 根据id获取对象所有endpoint列表
   */
  Int32 findObjectById4All(const std::string &id,
                           std::vector<tars::EndpointF> &activeEp,
                           std::vector<tars::EndpointF> &inactiveEp,
                           tars::CurrentPtr current);

  /**
   * 根据id获取对象同组endpoint列表
   */
  Int32 findObjectByIdInSameGroup(const std::string &id,
                                  std::vector<tars::EndpointF> &activeEp,
                                  std::vector<tars::EndpointF> &inactiveEp,
                                  tars::CurrentPtr current);

  /**
   * 根据id获取对象指定归属地的endpoint列表
   */
  Int32 findObjectByIdInSameStation(const std::string &id,
                                    const std::string &sStation,
                                    std::vector<tars::EndpointF> &activeEp,
                                    std::vector<tars::EndpointF> &inactiveEp,
                                    tars::CurrentPtr current);

  /**
   * 根据id获取对象同set endpoint列表
   */
  Int32 findObjectByIdInSameSet(const std::string &id, const std::string &setId,
                                vector<tars::EndpointF> &activeEp,
                                vector<tars::EndpointF> &inactiveEp,
                                tars::CurrentPtr current);

private:
  /**
   * 打印按天日志
   */
  void doDaylog(const FUNID eFnId, const std::string &id,
                const std::vector<tars::EndpointF> &activeEp,
                const std::vector<tars::EndpointF> &inactiveEp,
                const tars::CurrentPtr &current, const std::ostringstream &os,
                const std::string &sSetid = "");

  /**
   * 转化成字符串
   */
  std::stringeFunTostr(const FUNID eFnId);

protected:
  //数据库操作
  CDbHandle _db;
};

#endif
