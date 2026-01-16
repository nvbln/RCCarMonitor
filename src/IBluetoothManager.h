#include <vector>
#include <optional>

#include "IBluetoothDevice.h"

class IBluetoothManager {
public:
    virtual ~IBluetoothManager() = default;

    virtual std::vector<std::shared_ptr<IBluetoothDevice>> getDevices() const = 0;
    virtual std::optional<std::shared_ptr<IBluetoothDevice>> findDevice(std::string deviceName) const = 0;
};
