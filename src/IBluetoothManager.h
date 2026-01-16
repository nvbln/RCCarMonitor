#include <vector>

#include "IBluetoothDevice.h"

class IBluetoothManager {
public:
    virtual ~IBluetoothManager() = default;

    virtual std::vector<std::shared_ptr<IBluetoothDevice>> getDevices() const = 0;
};
