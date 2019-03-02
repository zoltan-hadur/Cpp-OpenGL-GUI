#pragma once

#include "FunctionInfo.h"
#include "Delegate.h"

#include <atomic>
#include <mutex>
#include <map>
#include <vector>

namespace OpenGLUI::Foundation::Test
{
  class EventTest;
}

namespace OpenGLUI::Foundation
{
  // Unique ID for managing event subscriptions
  using EventToken = uint64_t;

  // Implements C++/WinRT like event handling.
  // https://docs.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/author-events
  template<typename Delegate> class Event;

  // Class to provide unique ID upon event subscription
  class EventCommon final
  {
  private:
    friend class ::OpenGLUI::Foundation::Test::EventTest;
    template<typename Delegate> friend class Event;
    static std::atomic<EventToken> _id;
  };
  std::atomic<EventToken> EventCommon::_id = 0;

  // Implements C++/WinRT like event handling.
  // https://docs.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/author-events
  template<template<typename...> typename Delegate, typename ...Args>
  class Event<Delegate<void(Args...)>> final
  {
  private:
    friend class ::OpenGLUI::Foundation::Test::EventTest;
    mutable std::mutex _mutex;
    std::map<EventToken, Delegate<void(Args...)>> _handlers;

  public:
    Event() = default;

    Event(Event const& event)
    {
      *this = event;
    }

    Event& operator=(Event const& event)
    {
      if (this != &event)
      {
        std::unique_lock<std::mutex> thisLock(_mutex, std::defer_lock), eventLock(event._mutex, std::defer_lock);
        std::lock(thisLock, eventLock);
        _handlers = event._handlers;
      }
      return *this;
    }

    // Adds an event handler.
    EventToken Add(Delegate<void(Args...)> handler)
    {
      auto lock = std::lock_guard(_mutex);
      return _handlers.insert({ EventCommon::_id++, handler }).first->first;  // Adds the event handler with _id, increments _id, then returns with _id - 1
    }

    // Removes an event handler.
    void Remove(EventToken token)
    {
      auto lock = std::lock_guard(_mutex);
      if (_handlers.count(token))
      {
        _handlers.erase(token);
      }
    }

    // Removes all added event handlers.
    void Clear()
    {
      auto lock = std::lock_guard(_mutex);
      _handlers.clear();
    }

    // Invokes all added event handlers with the given arguments.
    void operator()(Args... args)
    {
      auto lock = std::lock_guard(_mutex);
      if (_handlers.size() == 0) return;

      auto tokensToRemove = std::vector<EventToken>();
      for (auto& [token, handler] : _handlers)
      {
        try { handler(args...); }
        catch (...) { tokensToRemove.push_back(token); }
      }

      for (auto& token : tokensToRemove)
      {
        _handlers.erase(token);
      }
    }
  };
}