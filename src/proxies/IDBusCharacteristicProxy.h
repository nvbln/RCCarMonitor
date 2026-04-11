#pragma once

#include <cstdint>
#include <string>
#include <vector>

/**
 * @class IDBusCharacteristicProxy
 * @brief interface for the wrapper between the DBusGattCharacteristic and SDBus.
 *
 * The proxy implementation handles the communication with the SDBus library.
 * This interface facilitates the communication between the DBus implementation in
 * the application and the SDBus library specifics.
 */
class IDBusCharacteristicProxy {
public:
  /**
   * @brief the path to the characteristic.
   *
   * @return the path as a string.
   */
  virtual std::string objectPath() = 0;

  /**
   * @brief the unique UUID of the characteristic.
   *
   * @return the UUID as a string.
   */
  virtual std::string uuid() = 0;

  /**
   * @brief retrieves the current value of the characteristic.
   *
   * @return a byte vector with the value of the characteristic.
   */
  virtual std::vector<uint8_t> read() = 0;

  /**
   * @brief sets the given value as the characteristic value.
   *
   * @param the byte vector to write to the characteristic.
   */
  virtual void write(const std::vector<uint8_t>& value) = 0;
};
