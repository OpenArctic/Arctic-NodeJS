#include "Global.h"
#include "NodeJsObjectFactoryDelegate.h"

namespace arctic {
    NodeJsObjectFactoryDelegate::NodeJsObjectFactoryDelegate() : next_handle_(0) {

    }

    NodeJsObjectFactoryDelegate::~NodeJsObjectFactoryDelegate() {

    }

    uint8_t NodeJsObjectFactoryDelegate::GetType() {
        return 2;
    }

    Object* NodeJsObjectFactoryDelegate::Create(std::string id) {
        return nullptr;
    }

    Variant NodeJsObjectFactoryDelegate::GetProperty(Object* instance, std::string name) {
        Handle handle = instance->GetHandle();
        auto it = objects_.find(handle.raw_handle);
        if (it != objects_.end()) {
            Napi::ObjectReference* ref = it->second.get();
            Napi::Value value = ref->Get(name);
            return NapiValue2Variant(value);
        }
        return Variant(Null{});
    }

    void NodeJsObjectFactoryDelegate::SetProperty(Object* instance, std::string name, Variant value) {

    }

    Variant NodeJsObjectFactoryDelegate::Invoke(Object* instance, std::string method) {
        return Variant(Null{});
    }

    Variant NodeJsObjectFactoryDelegate::Invoke(Object* instance, std::string method, std::vector<NamedVariant> params) {
        return Variant(Null{});
    }

    uint64_t NodeJsObjectFactoryDelegate::RegisterObject(Napi::Object obj) {
        Napi::ObjectReference* ref = new Napi::ObjectReference();
        *ref = Napi::Weak(obj);
        uint64_t handle = ++next_handle_;
        objects_[handle] = std::unique_ptr<Napi::ObjectReference>(ref);
        return handle;
    }

    Napi::Object NodeJsObjectFactoryDelegate::FindObject(uint64_t handle) {
        auto it = objects_.find(handle);
        if (it != objects_.end()) {
            return it->second->Value();
        }
        return Napi::Object();
    }
}