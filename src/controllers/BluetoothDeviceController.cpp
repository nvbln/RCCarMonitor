#include "BluetoothDeviceController.h"

#include "IBluetoothDevice.h"
#include "IBluetoothManager.h"

#include <memory>

BluetoothDeviceController::BluetoothDeviceController(
    std::shared_ptr<IBluetoothManager> bluetoothManager,
    std::shared_ptr<IItemPickerView> itemPicker)
    : mBluetoothManager(bluetoothManager), mItemPicker(itemPicker) {
  mDevices = mBluetoothManager->getDevices();
  mItemPicker->subscribe([this](int ind) { event.notify(mDevices[ind]); });
  update();
}

void BluetoothDeviceController::update() {
  mDevices = mBluetoothManager->getDevices();
  std::vector<std::string> deviceNames;
  std::transform(mDevices.begin(), mDevices.end(), std::back_inserter(deviceNames),
                 [](const std::shared_ptr<IBluetoothDevice> device) { return device->name(); });
  mItemPicker->update(deviceNames);
}
