#ifndef __HelloServer_H_
#define __HelloServer_H_

#include "PushThread.h"
#include "servant/Application.h"
#include <iostream>


using namespace tars;

extern std::stringg_HelloServerObj;
extern std::stringg_TransDstServerObj;
extern atomic<int> g_handleDestroy;

class HelloServer : public Application, public TC_Thread {
public:
  HelloServer() {}
  virtual ~HelloServer() {}
  /**
   * 服务初始化
   **/
  virtual void initialize();

  /**
   * 服务销毁
   **/
  virtual void destroyApp();

protected:
  virtual void run();

private:
  //用于push消息的线程
  PushInfoThread *pushThread;
};

////////////////////////////////////////////
#endif
