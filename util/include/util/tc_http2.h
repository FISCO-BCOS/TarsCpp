#ifndef __TC_HTTP2_H__
#define __TC_HTTP2_H__

#include "util/tc_http.h"
#include "util/tc_network_buffer.h"
#include "util/tc_spin_lock.h"

typedef struct nghttp2_session nghttp2_session;

namespace tars {

class TC_Http2 {
public:
  /**
   * constructor
   */
  TC_Http2();

  /**
   * deconstructor
   */
  virtual ~TC_Http2();

  /**
   * data pack
   */
  struct DataPack {
    DataPack(const char *data, size_t length)
        : _dataBuf(data), _length(length) {}

    const char *_dataBuf;
    size_t _length;
    size_t _readPos = 0;
  };

  /**
   * @brief setting
   */
  int settings(unsigned int maxCurrentStreams = 2000);

  /**
   * @brief  buffer
   */
  std::vector<char> &buffer() { return _buff; }

  /**
   * buffer
   * @return
   */
  const std::vector<char> &buffer() const { return _buff; }

  /**
   * swap buff
   * @param buff
   */
  void swap(std::vector<char> &buff) { _buff.swap(buff); }

  /**
   * insert buff
   * @param buff
   */
  void insertBuff(const char *buff, size_t length) {
    _buff.insert(_buff.end(), buff, buff + length);
  }

  /**
   * @brief  session
   */
  nghttp2_session *session() const { return _session; }

  /**
   *
   * @return
   */
  const char *getErrMsg();

protected:
  /**
   * error code
   */
  int _err = 0;

  /**
   * session
   */
  nghttp2_session *_session;

  /**
   * data buff
   */
  std::vector<char> _buff;
};

class TC_Http2Server : public TC_Http2 {
public:
  /**
   * constructor
   */
  TC_Http2Server();

  /**
   * deconstructor
   */
  ~TC_Http2Server();

  /**
   * context
   */
  struct Http2Context {
    Http2Context(int32_t id) : reqId(id) {}

    int32_t reqId;
    TC_HttpRequest request;
    TC_HttpResponse response;
  };

  /**
   * parse all request
   * @param request
   * @param std::vector<std::shared_ptr<TC_HttpRequest>>
   * @return
   */
  std::vector<std::shared_ptr<Http2Context>> decodeRequest();

  /**
   *
   * @param reqid
   * @param response
   * @param out
   * @return
   */
  int encodeResponse(const std::shared_ptr<Http2Context> &context,
                     std::vector<char> &out);

  /**
   * http2
   * @param in
   * @param out
   * @return
   */
  TC_NetWorkBuffer::PACKET_TYPE parse(TC_NetWorkBuffer &in,
                                      std::vector<char> &out);

  void onHeaderCallback(int32_t streamId);
  void onHeaderCallback(int32_t streamId, const std::string &skey,
                        const std::string &svalue);
  void onFrameRecvCallback(int32_t streamId);
  void onDataChunkRecvCallback(int32_t streamId, const char *data, size_t len);
  void onStreamCloseCallback(int32_t streamId);

protected:
  std::shared_ptr<Http2Context> getContext(int32_t streamId);

  void deleteContext(int32_t streamId);

protected:
  TC_ThreadMutex _nghttpLock;

  std::unordered_map<int32_t, std::shared_ptr<Http2Context>> _context;

  std::vector<std::shared_ptr<Http2Context>> _contextFinished;

  std::vector<char> _reqout;
};

/////////////////////////////////////////////////////////////////////////////////

class TC_Http2Client : public TC_Http2 {
public:
  /**
   * constructor
   */
  TC_Http2Client();

  /**
   * deconstructor
   */
  ~TC_Http2Client();

  /**
   * parse response
   * @param in
   */
  TC_NetWorkBuffer::PACKET_TYPE
  parseResponse(TC_NetWorkBuffer &in,
                std::pair<int, std::shared_ptr<TC_HttpResponse>> &out);

  /**
   * submit, get net buffer to send
   * @param method
   * @param path
   * @param header
   * @param buff
   * @return
   */
  //    int submit(const std::string&method, const std::string&path, const
  //    std::map<string, string> &header, const std::vector<char> &buff);
  int submit(const TC_HttpRequest &request);

  /**
   * @brief response
   */
  std::unordered_map<int, std::shared_ptr<TC_HttpResponse>> &responses() {
    return _responses;
  }

  /**
   * @brief response finished
   */
  std::unordered_map<int, std::shared_ptr<TC_HttpResponse>> &doneResponses() {
    return _doneResponses;
  }

private:
  /**
   * 收到的响应
   * Responses received
   */
  std::unordered_map<int, std::shared_ptr<TC_HttpResponse>> _responses;

  /**
   * 收到的完整响应
   * Complete response received
   */
  std::unordered_map<int, std::shared_ptr<TC_HttpResponse>> _doneResponses;
};

} // namespace tars

//#endif

#endif
