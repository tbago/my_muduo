#pragma once

#include <functional>
#include <memory>

#include "base/none_copyable.h"
#include "base/time_stamp.h"

namespace net {

class EventLoop;

/**
 * Channel 封装了sockfd和其相关的event ，例如EPOLLIN, EPOLLOUT。还绑定了poller返回的具体事件。
 **/
class Channel {
public:
    Channel(EventLoop *event_loop, int fd);
    ~Channel();

public:
    using ReadEventCallback = std::function<void(base::TimeStamp)>;  ///< 读时间回调
    using EventCallback = std::function<void()>;                     ///< 其他事件回调

    void SetReadEventCallback(ReadEventCallback callback) { read_event_callback_ = std::move(callback); }
    void SetWriteEventCallback(EventCallback callback) { write_event_callback_ = std::move(callback); }
    void SetCloseEventCallback(EventCallback callback) { close_event_callback_ = std::move(callback); }
    void SetErrorEventCallback(EventCallback callback) { error_event_callback_ = std::move(callback); }

    // fd得到Poller通知以后 处理事件 handleEvent在EventLoop::loop()中调用
    void HandleEvent(base::TimeStamp receive_time);
public:
    // 设置fd相应的时间状态，相当于epoll_ctl add delete
    void EnableReading() { events_ |= kReadEvent; Update(); }
    void DisableReading() { events_ &= ~kReadEvent; Update(); }
    void EnableWritting() { events_ |= kWriteEvent; Update(); }
    void DisableWritting() { events_ &= ~kWriteEvent; Update(); }
    void DisableAll() { events_ = kNoneEvent; Update(); }
    // 获取当前fd的时间状态
    bool IsNoneEvent() const { return events_ == kNoneEvent; }
    bool CanWritting() const { return events_ & kWriteEvent; }
    bool CanReading() const { return events_ & kReadEvent; }
public:
    int Fd() const { return fd_; }
    int Events() const { return events_; }
    void SetReturnEvents(int return_events) { return_events_ = return_events; }
    int Index() const { return index_; }
    void SetIndex(int index) {index_ = index; }
    EventLoop *GetOwnerLoop() const { return event_loop_; }
    void Remove();
public:
    void Tie(const std::shared_ptr<void> &);  // 防止当channel被手动remove掉时，channel还在执行回调操作
private:
    void Update();
    void HandleEventWithGuard(base::TimeStamp receive_time);
private:
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

private:
    ReadEventCallback read_event_callback_;
    EventCallback write_event_callback_;
    EventCallback close_event_callback_;
    EventCallback error_event_callback_;

    EventLoop *event_loop_;  // 时间循环
    const int fd_;           // fd Poller 监听的对象
    int events_;
    int return_events_;
    int index_;

    std::weak_ptr<void> tie_;
    bool tied_;
};

}  // namespace net
