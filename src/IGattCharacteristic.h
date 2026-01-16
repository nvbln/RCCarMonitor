#ifndef IGATTCHARACTERISTIC_H 
#define IGATTCHARACTERISTIC_H 

class IGattCharacteristic {
public:
    virtual ~IGattCharacteristic() = default;

    virtual std::string uuid() = 0;
    virtual std::vector<uint8_t> read() = 0;
    virtual void write(const std::vector<uint8_t>& value) = 0;
};

#endif
