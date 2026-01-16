#ifndef IBLUETOOTHDEVICE_H
#define IBLUETOOTHDEVICE_H

class IBluetoothDevice {
public:
    virtual ~IBluetoothDevice() = default;

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() = 0;
    virtual std::string name() = 0;
    virtual std::string address() = 0;
};

#endif
