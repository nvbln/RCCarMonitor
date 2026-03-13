#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <vector>

/**
 * @class IGattCharacteristicHandler
 * @brief Handles the protocol and safety logic of the IGattCharacteristic.
 */
class IGattCharacteristicHandler {
public:
  /**
   * @brief Signals to the subscriber that the characteristic is available on the device.
   */
  using OnAvailableCallback = std::function<void()>;

  /**
   * @brief Signals that the characteristic used to be available but is not anymore.
   */
  using OnUnavailableCallback = std::function<void()>;

  /**
   * @brief Indicates whether the characteristic is available on the device.
   *
   * @return true if available, false if not.
   */
  virtual bool isAvailable() const = 0;

  /**
   * @brief Signals that the characteristic is available on the device.
   *
   * @param callback Empty callback used for signaling that the characteristic is available.
   */
  virtual void onAvailable(OnAvailableCallback callback) = 0;
  /**
   * @brief Signals that the characteristic used to be available, but is not anymore.
   *
   * @param callback Empty callback for signaling that the characteristic is not available anymore.
   */
  virtual void onUnavailable(OnUnavailableCallback callback) = 0;

  /**
   * @brief Safely reads from the characteristic by wrapping it on an optional.
   *
   * @see IGattCharacteristic::read() as well for the non-safe operation.
   *
   * @return std::nullopt if no value, otherwise a byte-vector with the value.
   */
  virtual std::optional<std::vector<uint8_t>> read() = 0;

  /**
   * @brief Safely writes to the characteristic by implementing safety-checks.
   *
   * @see IGattCharacteristic::write() as well for the non-safe operation.
   *
   * @param data The data to be written to the characteristic.
   * @return whether the writing operation was successful.
   */
  virtual bool write(const std::vector<uint8_t> &data) = 0;
};
