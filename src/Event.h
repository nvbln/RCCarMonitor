#pragma once

#include <vector>

template <typename CallbackType, typename... Args> class Event {
public:
  void subscribe(CallbackType callback) { mCallbacks.push_back(callback); }

  void notify(Args... args) {
    for (auto callback : mCallbacks) {
      callback(args...);
    }
  }

private:
  std::vector<CallbackType> mCallbacks;
};
