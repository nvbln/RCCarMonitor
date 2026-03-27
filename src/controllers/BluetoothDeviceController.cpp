#include "BluetoothDeviceController.h"

#include "interfaces/IBluetoothDevice.h"
#include "interfaces/IBluetoothManager.h"

#include <memory>

BluetoothDeviceController::BluetoothDeviceController(IBluetoothManager *bluetoothManager,
                                                     IItemPickerView *itemPicker)
    : mBluetoothManager(bluetoothManager), mItemPicker(itemPicker) {
  mDevices = mBluetoothManager->getDevices();
  mItemPicker->subscribe([this](int ind) { event.notify(mDevices[ind]); });
  update();
}

void BluetoothDeviceController::update() {
  mDevices = mBluetoothManager->getDevices();
  std::vector<std::string> deviceNames;
  std::transform(mDevices.begin(), mDevices.end(), std::back_inserter(deviceNames),
                 [](const IBluetoothDevice *device) { return device->name(); });
  mItemPicker->update(deviceNames);
}
