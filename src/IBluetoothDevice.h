#ifndef IBLUETOOTHDEVICE_H
#define IBLUETOOTHDEVICE_H

#include <optional>
#include "IGattCharacteristic.h"

class IBluetoothDevice {
public:
    virtual ~IBluetoothDevice() = default;

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() = 0;
    virtual std::string name() = 0;
    virtual std::string address() = 0;
    virtual std::optional<std::shared_ptr<IGattCharacteristic>> findCharacteristic(std::string uuid) = 0;

    // TODO: Add UUID
};

#endif
