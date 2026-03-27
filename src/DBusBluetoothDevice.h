#pragma once

#include <sdbus-c++/sdbus-c++.h>

#include <optional>

#include "DBusGattCharacteristic.h"
#include "Event.h"
#include "interfaces/IBluetoothDevice.h"
#include "interfaces/IGattCharacteristic.h"
#include "proxies/IDBusDeviceProxy.h"
#include "proxies/IDBusPropertiesProxy.h"

/**
 * @class DBusBluetoothDevice
 * @brief DBus implementation of the Bluetooth device.
 *
 * Brings together the general interface for Bluetooth devices
 * and the Bluetooth device as defined by the DBus XML object.
 * Interacts with the Bluetooth device through the DBus API.
 */
class DBusBluetoothDevice final : public IBluetoothDevice {
public:
  /**
   * @brief creates an instance representing the DBus Bluetooth device.
   *
   * @param proxy the proxy through which the device communicates with SDBus.
   * @param propertiesProxy the proxy that notifies the object of any property changes.
   */
  DBusBluetoothDevice(std::shared_ptr<IDBusDeviceProxy> proxy,
                      std::shared_ptr<IDBusPropertiesProxy> propertiesProxy)
      : mProxy(proxy), mPropertiesProxy(propertiesProxy), mName(proxy->name()),
        mAddress(proxy->address()) {
    propertiesProxy->subscribeToOnPropertiesChanged(
        [this](const sdbus::InterfaceName &interface,
               const std::map<sdbus::MemberName, sdbus::Variant> &changedProperties,
               const std::vector<sdbus::MemberName> &invalidatedProperties) {
          this->onPropertiesChanged(interface, changedProperties, invalidatedProperties);
        });
  }

  /**
   * @brief adds a DBus GATT Characteristic to the device.
   *
   * @param characteristic The characteristic to add to the device.
   */
  void addCharacteristic(std::shared_ptr<DBusGattCharacteristic> characteristic);

  /**
   * @brief removes the DBus GATT Characteristic from the device.
   *
   * @param characteristic The characteristic to remove from the device.
   */
  void removeCharacteristic(std::shared_ptr<DBusGattCharacteristic> characteristic);

  /**
   * @brief retrieves the first DBusCharacteristic with the given property, if any
   *
   * @param value The value that the sought after Characteristic has.
   * @param property The property of the value, uuid by default.
   */
  std::optional<std::shared_ptr<DBusGattCharacteristic>>
  findDBusCharacteristic(std::string value, std::string property = "uuid");

  /**
   * @see IBluetoothDevice::connect()
   */
  void connect() override { mProxy->connect(); }

  /**
   * @see IBluetoothDevice::disconnect()
   */
  void disconnect() override { mProxy->disconnect(); }

  /**
   * @see IBluetoothDevice::isConnected()
   */
  bool isConnected() override { return mProxy->isConnected(); }

  /**
   * @see IBluetoothDevice::name()
   */
  std::string name() const override { return mName; }

  /**
   * @see IBluetoothDevice::address()
   */
  std::string address() const override { return mAddress; }

  /**
   * @see IBluetoothDevice::findCharacteristic()
   */
  std::optional<std::shared_ptr<IGattCharacteristic>> findCharacteristic(std::string uuid) override;

  void subscribeToAddCharacteristic(Callback callback) override { addEvent.subscribe(callback); }

  void subscribeToRemoveCharacteristic(Callback callback) override {
    removeEvent.subscribe(callback);
  }

private:
  std::shared_ptr<IDBusDeviceProxy> mProxy;
  std::shared_ptr<IDBusPropertiesProxy> mPropertiesProxy;
  std::vector<std::shared_ptr<DBusGattCharacteristic>> mCharacteristics;

  std::string mName;
  std::string mAddress;

  Event<Callback, std::shared_ptr<IGattCharacteristic>> addEvent;
  Event<Callback, std::shared_ptr<IGattCharacteristic>> removeEvent;

  /**
   * @brief Called when a property of the device changes.
   *
   * This callback takes care of changing locally saved properties in case they get updated.
   *
   * @param interface The interface that gets updated (i.e. Device1 or Battery1).
   * @param changedProperties The new values of the properties that were changed.
   * @param invalidatedProperties The properaties that have been invalidated.
   */
  void onPropertiesChanged(const sdbus::InterfaceName &interface,
                           const std::map<sdbus::MemberName, sdbus::Variant> &changedProperties,
                           const std::vector<sdbus::MemberName> &invalidatedProperties);
};
