#include <vector>
#include <optional>

#include "IBluetoothDevice.h"

/**
 * @class IBluetoothManager
 * @brief General interface for interacting with the Bluetooth
 *
 * This interface provides a library/framework-independent way for the
 * application logic to interact with the Bluetooth. It manages the
 * Bluetooth and the services it provides.
 */
class IBluetoothManager {
public:
    virtual ~IBluetoothManager() = default;

    /**
     * @brief returns the Bluetooth devices found by the component.
     *
     * All devices that are found by the Bluetooth component are
     * transformed into device objects and stored by the
     * Bluetooth manager. This function returns those devices in
     * the form of IBluetoothDevice.
     * @return a vector of pointers to the found Bluetooth devices.
     */
    virtual std::vector<std::shared_ptr<IBluetoothDevice>> getDevices() const = 0;

    /**
     * @brief returns the device matched by the name, if any
     *
     * @param The name of the device that should be returned.
     * @return an optional that contains a pointer to the device if found.
     */
    virtual std::optional<std::shared_ptr<IBluetoothDevice>> findDevice(std::string deviceName) const = 0;
};
