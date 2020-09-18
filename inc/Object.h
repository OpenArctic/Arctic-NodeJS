#pragma once

#include <cstdint>
#include <future>
#include <string>
#include <variant>
#include <vector>

#include "DataType.h"

namespace arctic {
    class Object;
    class ObjectDelegate;
    class ObjectEventListener;
    class ObjectFactory;
    class ObjectFactoryDelegate;

    class ObjectFactoryDelegate {
    public:
        ObjectFactoryDelegate() : routing_id_(0), object_factory_(nullptr) {}
        virtual ~ObjectFactoryDelegate() {};

        virtual uint8_t GetRoutingId() { return routing_id_; };
        virtual uint8_t GetType() = 0;
        virtual uint16_t GetSourceId() { return Handle::GetSourceId(GetRoutingId(), GetType()); };
        virtual Object* Create(std::string id) = 0;

        virtual Variant GetProperty(Object* instance, std::string name) = 0;
        virtual void SetProperty(Object* instance, std::string name, Variant value) = 0;

        virtual MaybeError<Variant> Invoke(Object* instance, std::string method) = 0;
        virtual MaybeError<Variant> Invoke(Object* instance, std::string method, std::vector<NamedVariant> params) = 0;

        ObjectFactory* object_factory_;

        uint8_t routing_id_;
    };

    class ObjectFactory {
    public:
        ObjectFactory(uint8_t routing_id) : routing_id_(routing_id) {};
        virtual ~ObjectFactory() {};

        virtual uint8_t GetRoutingId() { return routing_id_; };
        virtual void AddDelegate(ObjectFactoryDelegate* delegate) = 0;
        virtual ObjectFactoryDelegate* GetDelegate(uint8_t type) = 0;
        virtual std::future<Object*> GetObjectFromHandle(Handle handle) = 0;
        virtual Object* Attach(uint8_t type, uint64_t raw_handle) = 0;
        virtual std::future<Object*> Create(uint8_t routing_id, uint8_t type, std::string id) = 0;
        virtual bool Destroy(uint8_t type, uint64_t raw_handle) = 0;

    private:
        uint8_t routing_id_;
    };

    class ObjectDelegate {
    public:
        ObjectDelegate() {};
        virtual ~ObjectDelegate() {};

        virtual Variant GetProperty(Object* instance, std::string name) = 0;
        virtual void SetProperty(Object* instance, std::string name, Variant value) = 0;

        virtual MaybeError<Variant> Invoke(Object* instance, std::string method) = 0;
        virtual MaybeError<Variant> Invoke(Object* instance, std::string method, std::vector<NamedVariant> params) = 0;
    };

    class ObjectEventListener {
    public:
        ObjectEventListener(std::string event_type) : event_type_(event_type) {};
        virtual ~ObjectEventListener() {};

        std::string EventType() { return event_type_; };
        virtual void DispatchEvent(Object* instance, std::string event_type, std::vector<NamedVariant> params) = 0;

    private:
        std::string event_type_;
    };

    class Object {
    public:
        Object() {};
        virtual ~Object() {};

        virtual uint8_t GetRoutingId() = 0;
        virtual uint8_t GetType() = 0;
        virtual uint16_t GetSourceId() = 0;
        virtual Handle GetHandle() = 0;

        virtual bool IsLocal() = 0;

        virtual void Delete() = 0;

        virtual bool GetAvaliable() { return is_avaliable_; };
        virtual void SetAvaliable(bool avl) { is_avaliable_ = avl; };

        virtual void AddDelegate(ObjectDelegate* delegate) = 0;
        virtual ObjectDelegate* GetDelegate() = 0;

        virtual std::future<Variant> GetProperty(std::string name) = 0;
        virtual std::future<void> SetProperty(std::string name, Variant value) = 0;

        virtual std::future<MaybeError<Variant>> Invoke(std::string method) = 0;
        virtual std::future<MaybeError<Variant>> Invoke(std::string method, std::vector<NamedVariant> params) = 0;

        virtual void AddEventListener(ObjectEventListener* listener) = 0;
        virtual void RemoveEventListener(ObjectEventListener* listener) = 0;
        virtual void FireEvent(std::string event_type, std::vector<NamedVariant> params) = 0;

    private:
        bool is_avaliable_ = true;
    };
}