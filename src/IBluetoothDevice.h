#ifndef IBLUETOOTHDEVICE_H
#define IBLUETOOTHDEVICE_H

#include <optional>
#include "IGattCharacteristic.h"

/**
 * @class IBluetoothDevice
 * @brief Representation of a Bluetooth device.
 *
 * This interface is a representation of a generic Bluetooth
 * device and enables generic interactions with said device.
 */
class IBluetoothDevice {
public:
    virtual ~IBluetoothDevice() = default;

    /**
     * @brief Connect with the Bluetooth device.
     */
    virtual void connect() = 0;

    /**
     * @brief Disconnect from the Bluetooth device.
     */
    virtual void disconnect() = 0;

    /**
     * @brief Check if a connection is established with the device.
     *
     * @return Whether a connection is established with the device.
     */
    virtual bool isConnected() = 0;
    
    /**
     * @brief Returns the name of the device.
     *
     * @return The name of the device
     */
    virtual std::string name() = 0;

    /**
     * @brief Returns the hexadecimal address of the device.
     *
     * This address only includes the hexadecimal address of the
     * device itself not the path leading to the device.
     * @return The hexadecimal address of the device.
     */
    virtual std::string address() = 0;

    /**
     * @brief returns the GATT Characteristic matched by the name, if any
     *
     * @param uuid The UUID of the Characteristic that should be returned.
     * @return an optional that contains a pointer to the Characteristic if found.
     */
    virtual std::optional<std::shared_ptr<IGattCharacteristic>> findCharacteristic(std::string uuid) = 0;

    // TODO: Add UUID
};

#endif
