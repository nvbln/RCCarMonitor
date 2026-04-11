#pragma once

#include <functional>
#include <map>
#include <sdbus-c++/sdbus-c++.h>

/**
 * @class IDBusPropertiesProxy
 * @brief interface for wrapping between Properties and the SDBus library.
 *
 * The proxy implementation handles the communication with the SDBus library.
 * This interface facilitates the communication between the DBus implementation in
 * the application and the SDBus library specifics.
 */
class IDBusPropertiesProxy {
public:
  /**
   * A callback such that the application DBus implementation is updated on
   * properties changes, without having to inherit from the SDBus library.
   *
   * The callback is triggered every time a property changes.
   */
  using OnPropertiesChangedCallback =
      std::function<void(const sdbus::InterfaceName& interface,
                         const std::map<sdbus::MemberName, sdbus::Variant>& changedProperties,
                         const std::vector<sdbus::MemberName>& invalidatedProperties)>;

  /**
   * @brief subscribes the callback to updates on changed/invalidated properties.
   *
   * @param callback The function to notify of the changes.
   */
  virtual void subscribeToOnPropertiesChanged(const OnPropertiesChangedCallback callback) = 0;
};
