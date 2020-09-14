#pragma once

namespace arctic {

#define ARCTIC_PROTOCOL_MSG_TYPE_CLIENT_INIT 1
#define ARCTIC_PROTOCOL_MSG_TYPE_HOST_ACK 2
#define ARCTIC_PROTOCOL_MSG_TYPE_NORMAL 3
#define ARCTIC_PROTOCOL_MSG_TYPE_INTERNAL_OBJECT 4
#define ARCTIC_PROTOCOL_MSG_TYPE_INTERNAL_AGENT 5

#define ARCTIC_PROTOCOL_FIXED_HEADER_LENGTH 4

#define ARCTIC_OBJECT_TYPE_CORE 0

#define ARCTIC_ROUTING_BROADCAST_ID 255

    typedef struct Buffer {
        char* data;
        size_t length;

        void Zero() {
            data = nullptr;
            length = 0;
        }
    } Buffer;

    typedef struct Handle {
        uint8_t routing_id;
        uint8_t type;
        uint64_t raw_handle;

        static Handle New() {
            return Handle{ 0, 0, 0 };
        }

        static Handle New(uint8_t routing_id, uint8_t type, uint64_t raw_handle) {
            return Handle{ routing_id, type, raw_handle };
        }

        static Handle New(uint16_t source_id, uint64_t raw_handle) {
            uint8_t routing_id = (uint8_t)(source_id >> 8);
            uint8_t type = (uint8_t)source_id;
            return Handle{ routing_id, type, raw_handle };
        }

        static uint16_t GetSourceId(uint8_t routing_id, uint8_t type) {
            uint16_t source_id = (uint16_t)(routing_id << 8);
            source_id += (uint16_t)type;
            return source_id;
        }

        uint16_t source_id() {
            uint16_t source_id = (uint16_t)(routing_id << 8);
            source_id += (uint16_t)type;
            return source_id;
        }

        bool operator==(const Handle& h) const
        {
            return routing_id == h.routing_id && type == h.type && raw_handle == h.raw_handle;
        }

        bool operator<(const Handle& h) const
        {
            if (routing_id < h.routing_id) {
                return true;
            }
            else if (routing_id == h.routing_id) {
                if (type < h.type) {
                    return true;
                }
                else if (type == type) {
                    return raw_handle < h.raw_handle;
                }
                return false;
            }
            return false;
        }

        bool IsNull()
        {
            return routing_id == 0 && type == 0 && raw_handle == 0;
        }
    } Handle;

    typedef enum VariantType {
        NullType, Boolean, Integer, Double, String, HandleType
    } VariantType;

    typedef struct Null {} Null;

    typedef std::variant<Null, bool, int, double, std::string, Handle> Variant;

    inline bool IsNull(Variant var) { return var.index() == VariantType::NullType; }
    inline bool IsBoolean(Variant var) { return var.index() == VariantType::Boolean; }
    inline bool IsInteger(Variant var) { return var.index() == VariantType::Integer; }
    inline bool IsDouble(Variant var) { return var.index() == VariantType::Double; }
    inline bool IsString(Variant var) { return var.index() == VariantType::String; }
    inline bool IsHandle(Variant var) { return var.index() == VariantType::HandleType; }

    inline Variant NewVariant() { return Variant(); }

    inline VariantType GetVariantType(const Variant& var) { return (VariantType)var.index(); }

    typedef struct Error {
        int32_t code;
        std::string msg;

        Error() : code(0) {}

        Error(std::string m) : code(-1), msg(m) {}
        Error(int32_t c, std::string m) : code(c), msg(m) {}

        bool IsError() {
            return code != 0;
        }
    } Error;

    inline bool IsError(Error& err) { return err.code != 0; }

    template<typename T>
    struct MaybeError {
        Error err;
        T value;

        MaybeError() {}

        MaybeError(Error e) : err(e) {}
        MaybeError(T v) : value(v) {}

        bool IsError() {
            return err.code != 0;
        }
    };

    typedef struct NamedVariant {
        std::string name;
        Variant value;
    } NamedVariant;

    typedef std::vector<NamedVariant> NamedVariants;

    //typedef struct NVPacket {
    //    uint8_t type;
    //    uint8_t name_length;
    //    uint16_t value_length;
    //    // string type include \0
    //    std::unique_ptr<char[]> name;
    //    std::unique_ptr<char[]> value;
    //} NVPacket;

    typedef struct Packet {
        // 1 = syn (Send child process routing id to the main process) 
        // 2 = ack (Send main process routing id to the child process) 
        // 3 = normal message
        // 4 = internal object message
        uint8_t type;
        // 255 = broadcast address
        uint8_t routing_id;
        uint16_t remaining_length;

        std::unique_ptr<char[]> payload;
    } Packet;

}