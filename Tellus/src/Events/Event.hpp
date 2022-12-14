#pragma once
#include "tspch.h"

namespace ts {

// clang-format off
enum class EventType {
    None = 0,
    WindowClosed, WindowResized, WindowFocus, WindowLostFocus, WindowMoved,
    KeyPressed, KeyReleased,
    MousePressed, MouseReleased, MouseMoved, MouseScrolled
};
// clang-format on

#define EVENT_CLASS_TYPE(type)                                                  \
    static EventType GetStaticType() { return EventType::##type; }              \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char* GetName() const override { return #type; }

class Event {
   public:
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    virtual std::string ToString() const { return GetName(); }

   public:
    bool Handled = false;
};

class EventDispatcher {
    template <typename T>
    using EventFn = std::function<bool(T&)>;

   public:
    EventDispatcher(Event& event) : m_Event(event) {}

    template <typename T>
    bool Dispatch(EventFn<T> func) {
        if (m_Event.GetEventType() == T::GetStaticType()) {
            m_Event.Handled = func(*(T*)&m_Event);
            return true;
        }
        return false;
    }

   private:
    Event& m_Event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.ToString(); }

};  // namespace ts