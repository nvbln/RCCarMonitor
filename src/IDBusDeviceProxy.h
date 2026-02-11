#pragma once

#include <string>
#include <sdbus-c++/sdbus-c++.h>

/**
 * @class IDBusDeviceProxy
 * @brief interface for the wrapper between DBusBluetoothDevice and the SDBus library.
 *
 * The proxy implementation handles the communication with the SDBus library.
 * This interface facilitates the communication between the DBus implementation in
 * the application and the SDBus library specifics.
 */
class IDBusDeviceProxy {
public:

    /**
     * @brief Connect to the device.
     */
    virtual void connect() = 0;

    /**
     * @brief Disconnect from the device.
     */
    virtual void disconnect() = 0;

    /**
     * @brief whether the device is connected over Bluetooth.
     *
     * @return whether the device is connected.
     */
    virtual bool isConnected() = 0;

    /**
     * @brief retrieves the name of the device
     *
     * @return the name of the device
     */
    virtual std::string name() = 0;

    /**
     * @brief retrieves the device address
     *
     * @return the MAC address of the device as a string
     */
    virtual std::string address() = 0;
};
