#pragma once

#include <memory>
#include <string>

#include "interfaces/IGattCharacteristic.h"
#include "proxies/IDBusCharacteristicProxy.h"

/**
 * @class DBusGattCharacteristic
 * @brief DBus implementation of the GATT Characteristic.
 *
 * Brings together the general interface for GATT Characteristics,
 * as well as Characteristic as defined by the DBus XML object.
 * Interacts with the GATT Characteristic through the DBus API.
 */
class DBusGattCharacteristic : public IGattCharacteristic {
public:
  /**
   * @brief creates an instance representing the DBus GATT Characteristic.
   *
   * @param proxy the proxy through which the characteristic communicates with SDBus.
   */
  DBusGattCharacteristic(std::unique_ptr<IDBusCharacteristicProxy> proxy)
      : mProxy(std::move(proxy)) {}

  /**
   * @brief returns the complete path to the Characteristic.
   *
   * @return the DBus path to (and including) the Characteristic.
   */
  std::string objectPath() { return mProxy->objectPath(); }

  /**
   * @see IGattCharacteristic::uuid()
   */
  std::string uuid() override { return mProxy->uuid(); }

  /**
   * @see IGattCharacteristic::read()
   */
  std::vector<uint8_t> read() override { return mProxy->read(); }

  /**
   * @see IGattCharacteristic::write()
   */
  void write(const std::vector<uint8_t>& value) override { return mProxy->write(value); }

private:
  std::unique_ptr<IDBusCharacteristicProxy> mProxy;
};
