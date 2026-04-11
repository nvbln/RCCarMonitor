#pragma once

#include "Event.h"

#include "interfaces/IBluetoothDevice.h"
#include "interfaces/IBluetoothManager.h"

#include "views/IItemPickerView.h"

#include <functional>

/**
 * @class BluetoothDeviceController
 * @brief Handles bluetooth devices in UI.
 */
class BluetoothDeviceController final {
public:
  /**
   * @brief Callback to receive the selected Bluetooth device.
   * @return The Bluetooth device that the user picked.
   */
  using Callback = std::function<void(IBluetoothDevice*)>;

  /**
   * @brief Creates a controller for picking a Bluetooth device.
   *
   * Pointers are assumed to be available during the entire lifetime of the object.
   *
   * @param bluetoothManager the interface to retrieve all available Bluetooth devices.
   * @param itemPicker the UI element that allows the user to pick a Bluetooth device.
   */
  BluetoothDeviceController(IBluetoothManager* bluetoothManager, IItemPickerView* itemPicker);

  /**
   * @brief Updates the available devices, also updates the view.
   */
  void update();

  /**
   * @brief Notifies the subscriber of the device that the user has picked.
   *
   * @param callback the callback that should be executed once a device is selected.
   */
  void subscribe(Callback callback) { event.subscribe(callback); }

private:
  IBluetoothManager* mBluetoothManager;
  IItemPickerView* mItemPicker;
  std::vector<IBluetoothDevice*> mDevices;

  Event<Callback, IBluetoothDevice*> event;
};
