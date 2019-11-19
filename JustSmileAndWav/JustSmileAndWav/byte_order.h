#ifndef byte_order_h
#define byte_order_h
namespace su{ 
	enum class byte_order{little_endian,big_endian}; byte_order cur_byte_order() { const unsigned short val{*reinterpret_cast<const unsigned short*>("az")}; return val == 0x617AU ? byte_order::big_endian : byte_order::little_endian; } }
#endif