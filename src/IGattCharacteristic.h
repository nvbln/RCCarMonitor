#ifndef IGATTCHARACTERISTIC_H 
#define IGATTCHARACTERISTIC_H 

/**
 * @class IGattCharacteristic
 * @brief Representation of the GATT Characteristic in Bluetooth BLE.
 *
 * This interface is a representation of the GATT Characteristic
 * present in Bluetooth BLE. It enables generic interactions
 * with this Characteristic.
 */
class IGattCharacteristic {
public:
    virtual ~IGattCharacteristic() = default;

    /**
     * @brief Returns the UUID of the Characteristic
     *
     * @return The UUID of the Characteristic
     */
    virtual std::string uuid() = 0;

    /**
     * @brief Reads the current value from the Characteristic.
     *
     * @return The value as a byte array.
     */
    virtual std::vector<uint8_t> read() = 0;

    /**
     * @brief Writes the given value to the Characteristic.
     *
     * Writes the given value to the Characteristic on the
     * peripheral. The value is encoded as a byte array.
     */
    virtual void write(const std::vector<uint8_t>& value) = 0;
};

#endif
