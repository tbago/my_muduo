#include "channel.h"

#include <sys/epoll.h>

#include "base/logger.h"
#include "base/time_stamp.h"
#include "event_loop.h"

namespace net {

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;

Channel::Channel(EventLoop *event_loop, int fd) : event_loop_(event_loop), fd_(fd) {
    events_ = 0;
    return_events_ = 0;
    index_ = -1;
    tied_ = -1;
}

Channel::~Channel() {
}

void Channel::HandleEvent(base::TimeStamp receive_time) {
    if (tied_) {
        std::shared_ptr<void> guard = tie_.lock();
        if (guard) {
            HandleEventWithGuard(receive_time);
        }
        // 如果提升失败了 就不做任何处理 说明Channel的TcpConnection对象已经不存在了
    } else {
        HandleEventWithGuard(receive_time);
    }
}

// channel的tie方法什么时候调用过?  TcpConnection => channel
/**
 * TcpConnection中注册了Channel对应的回调函数，传入的回调函数均为TcpConnection
 * 对象的成员方法，因此可以说明一点就是：Channel的结束一定早于TcpConnection对象！
 * 此处用tie去解决TcoConnection和Channel的生命周期时长问题，从而保证了Channel对
 * 象能够在TcpConnection销毁前销毁。
 **/
void Channel::Tie(const std::shared_ptr<void> &object) {
    tie_ = object;
    tied_ = true;
}

void Channel::Remove() {
    event_loop_->ReomveChannel(this);
}

void Channel::Update() {
    event_loop_->UpdateChannel(this);
}

void Channel::HandleEventWithGuard(base::TimeStamp receive_time) {
    LOG_INFO("Channel handle event return events :%d", return_events_);

    if ((return_events_ & EPOLLHUP) && !(return_events_ & EPOLLIN)) {
        if (close_event_callback_) {
            close_event_callback_();
        }
    }
    if (return_events_ & EPOLLERR) {
        if (error_event_callback_) {
            error_event_callback_();
        }
    }
    if (return_events_ & (EPOLLIN | EPOLLPRI)) {
        if (read_event_callback_) {
            read_event_callback_();
        }
    }
    if (return_events_ & EPOLLOUT) {
        if (write_event_callback_) {
            write_event_callback_();
        }
    }
}

}  // namespace net
