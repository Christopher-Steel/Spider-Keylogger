#include "APacket.hpp"

APacket::APacket(PacketType type)
: _type(type)
{
    
}

APacket::~APacket()
{
    
}

std::vector<char> APacket::to_bytes() const
{
    std::vector<char> ret;
    std::vector<char> tmp = to_bytes_body();

    ret.push_back(_type);
    ret.insert(ret.end(), tmp.begin(), tmp.end());
    return (ret);
}

std::string APacket::to_readable(IReadable &parser) const
{
    std::string ret;
    
    parser.clear();
    parser.put("Type", static_cast<int>(_type));
    to_readable_body(parser);
    parser.write(ret);
    return (ret);
}

void APacket::from_bytes(const std::vector<char> &bytes)
{
    if (bytes.empty() && bytes[0] != _type)
        throw std::invalid_argument("Error while parse packet");
    from_bytes_body(bytes);
}

void APacket::from_readable(const std::string &data, IReadable &parser)
{
    int type = -1;
    
    parser.clear();
    parser.read(data);
    parser.get("Type", type);
    if (type != _type)
        throw std::invalid_argument("Error while parsing packet");
    from_readable_body(parser);
}
