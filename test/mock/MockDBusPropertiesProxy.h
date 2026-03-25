#pragma once

#include "proxies/IDBusPropertiesProxy.h"

#include <gmock/gmock.h>

class MockDBusPropertiesProxy : public IDBusPropertiesProxy {
public:
  MOCK_METHOD(void, subscribeToOnPropertiesChanged, (const OnPropertiesChangedCallback callback),
              (override));
};
