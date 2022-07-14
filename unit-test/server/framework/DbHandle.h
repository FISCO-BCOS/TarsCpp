

#ifndef __DB_HANDLE_H__
#define __DB_HANDLE_H__

#include "RegistryDescriptor.h"
#include "jmem/jmem_hashmap.h"
#include "servant/EndpointF.h"
#include "servant/RemoteLogger.h"
#include "util/tc_common.h"
#include "util/tc_config.h"
#include "util/tc_file.h"
#include "util/tc_monitor.h"
#include "util/tc_readers_writer_data.h"
#include <set>

using namespace tars;
//////////////////////////////////////////////////////
//<servant, ObjectItem>
typedef std::map<std::string, ObjectItem> ObjectsCache;
//////////////////////////////////////////////////////
/**
 *  数据库操作类
 */
class CDbHandle {
private:
  struct GroupPriorityEntry {
    std::string sGroupID;
    std::string sStation;
    std::set<int> setGroupID;
  };

  enum GroupUseSelect { ENUM_USE_WORK_GROUPID, ENUM_USE_REAL_GROUPID };
  // set中服务的信息
  struct SetServerInfo {
    std::stringsSetId;
    std::stringsSetArea;
    bool bActive;
    EndpointF epf;
  };
  //<servant,setname,vector>
  typedef std::map<std::string,
                   std::map<std::string, std::vector<CDbHandle::SetServerInfo>>>
      SetDivisionCache;

public:
  /**
   * 构造函数
   */
  CDbHandle() {}

  /**
   * 初始化
   * @param pconf 配置文件
   * @return 0-成功 others-失败
   */
  int init(TC_Config *pconf);

  /** 根据id获取对象
   *
   * @param id 对象名称
   *
   * @return  返回所有该对象的活动endpoint列表
   */
  std::vector<EndpointF> findObjectById(const std::string &id);

  /** 根据id获取对象
   *
   * @param id 对象名称
   * @out param activeEp    存活的列表
   * @out param inactiveEp  非存活的列表
   *
   * @return 0-成功 others-失败
   */
  int findObjectById4All(const std::string &id,
                         std::vector<EndpointF> &activeEp,
                         std::vector<EndpointF> &inactiveEp);

  /** 根据id获取同组对象
   *
   * @param id 对象名称
   * @param ip
   * @out param activeEp    存活的列表
   * @out param inactiveEp  非存活的列表
   * @out param os          打印日志使用
   *
   * @return 0-成功 others-失败
   */
  int findObjectByIdInSameGroup(const std::string &id, const std::string &ip,
                                std::vector<EndpointF> &activeEp,
                                std::vector<EndpointF> &inactiveEp,
                                ostringstream &os);

  /** 根据id获取优先级序列中的对象
   *
   * @param id 对象名称
   * @param ip
   * @out param vecActive    存活的列表
   * @out param vecInactive  非存活的列表
   * @out param os          打印日志使用
   *
   * @return 0-成功 others-失败
   */
  int findObjectByIdInGroupPriority(const std::string &sID,
                                    const std::string &sIP,
                                    std::vector<EndpointF> &vecActive,
                                    std::vector<EndpointF> &vecInactive,
                                    std::ostringstream &os);

  /** 根据id和归属地获取全部对象
   *
   * @param id 对象名称
   * @param sStation 归属地
   * @out param vecActive    存活的列表
   * @out param vecInactive  非存活的列表
   * @out param os          打印日志使用
   *
   * @return 0-成功 others-失败
   */
  int findObjectByIdInSameStation(const std::string &sID,
                                  const std::string &sStation,
                                  std::vector<EndpointF> &vecActive,
                                  std::vector<EndpointF> &vecInactive,
                                  std::ostringstream &os);

  /** 根据id和set信息获取全部对象
   *
   * @param sID 对象名称
   * @param vtSetInfo set信息
   * @out param vecActive    存活的列表
   * @out param vecInactive  非存活的列表
   * @out param os          打印日志使用
   *
   * @return 0-成功 others-失败
   */
  int findObjectByIdInSameSet(const std::string &sID,
                              const std::vector<std::string> &vtSetInfo,
                              std::vector<EndpointF> &vecActive,
                              std::vector<EndpointF> &vecInactive,
                              std::ostringstream &os);

  /** 根据setId获取全部对象
   *
   * @param sSetId set名称
   * @param vSetServerInfo SetName下部署的服务信息
   * @out param vecActive    存活的列表
   * @out param vecInactive  非存活的列表
   * @out param os          打印日志使用
   *
   * @return 0-成功 others-失败
   */
  int findObjectByIdInSameSet(const std::string &sSetId,
                              const std::vector<SetServerInfo> &vSetServerInfo,
                              std::vector<EndpointF> &vecActive,
                              std::vector<EndpointF> &vecInactive,
                              std::ostringstream &os);
  /**
   * 根据ip获取组id
   * @return int <0 失败 其它正常
   */
  int getGroupId(const std::string &ip);
  /**
   * ip转换
   */
  static uint32_t stringIpToInt(const std::string &sip);

  /**
   * ip转换
   */
  static std::stringIp2Str(uint32_t ip);

  /**
   * ip转换
   */
  static std::stringIp2StarStr(uint32_t ip);

  static void updateObjectsCache(const ObjectsCache &objCache, bool updateAll);

  static void updateActiveObjectsCache(const ObjectsCache &objCache,
                                       bool updateAll);

  static void updateInactiveObjectsCache(const ObjectsCache &objCache,
                                         bool updateAll);

  static void updateDivisionCache(SetDivisionCache &setDivisionCache,
                                  bool updateAll);

  static void updateCpuLoadInfo(std::vector<EndpointF> &vEndpointF);

  static void
  updateCpuLoadInfo(std::vector<CDbHandle::SetServerInfo> &vSetServerInfo);

  static void InsertSetRecord(const std::string &objName,
                              const std::string &setName,
                              const std::string &setArea,
                              const std::string &setGroup, EndpointF epf);

  static void InsertSetRecord4Inactive(const std::string &objName,
                                       const std::string &setName,
                                       const std::string &setArea,
                                       const std::string &setGroup,
                                       EndpointF epf);

  static void addActiveEndPoint(const std::string &objName, const Int32 port,
                                const Int32 istcp);

  static void addInactiveEndPoint(const std::string &objName, const Int32 port,
                                  const Int32 istcp);

  static void addEndPointbySet(const std::string &objName, const Int32 port,
                               const Int32 istcp, const std::string &setName,
                               const std::string &setArea,
                               const std::string &setGroup);

  static void addActiveWeight1EndPoint(const std::string &objName,
                                       const Int32 port, const Int32 istcp,
                                       const std::string &setName = "");

  static void addInActiveWeight1EndPoint(const std::string &objName,
                                         const Int32 port, const Int32 istcp,
                                         const std::string &setName = "");

  static void addActiveWeight2EndPoint(const std::string &objName,
                                       const Int32 port, const Int32 istcp,
                                       const std::string &setName = "");

  static void cleanEndPoint();
  ;

protected:
  /**
   * 根据group id获取Endpoint
   */
  std::vector<EndpointF> getEpsByGroupId(const std::vector<EndpointF> &vecEps,
                                         const GroupUseSelect GroupSelect,
                                         int iGroupId, ostringstream &os);

  std::vector<EndpointF> getEpsByGroupId(const std::vector<EndpointF> &vecEps,
                                         const GroupUseSelect GroupSelect,
                                         const std::set<int> &setGroupID,
                                         ostringstream &os);

protected:
  static TC_ReadersWriterData<ObjectsCache> _objectsCache;
  // set划分缓存
  static TC_ReadersWriterData<SetDivisionCache> _setDivisionCache;
  //优先级的序列
  static TC_ReadersWriterData<std::map<int, GroupPriorityEntry>>
      _mapGroupPriority;
  //分组信息
  static TC_ReadersWriterData<map<std::string, int>> _groupIdMap;
  static TC_ReadersWriterData<map<std::string, int>> _groupNameMap;
};

#endif
