#ifndef ARCTIC_JS_ARCTIC_NODEJSOBJECTFACTORYDELEGATE_H_
#define ARCTIC_JS_ARCTIC_NODEJSOBJECTFACTORYDELEGATE_H_

#include <napi.h>
#include "Arctic.h"

namespace arctic {
    class NodeJsObjectFactoryDelegate : public ObjectFactoryDelegate {
    public:
        NodeJsObjectFactoryDelegate();
        virtual ~NodeJsObjectFactoryDelegate();

        // Inherited via ObjectFactoryDelegate
        virtual uint8_t GetType() override;
        virtual Object* Create(std::string id) override;
        virtual Variant GetProperty(Object* instance, std::string name) override;
        virtual void SetProperty(Object* instance, std::string name, Variant value) override;
        virtual Variant Invoke(Object* instance, std::string method) override;
        virtual Variant Invoke(Object* instance, std::string method, std::vector<NamedVariant> params) override;

        uint64_t RegisterObject(Napi::Object obj);
        Napi::Object FindObject(uint64_t handle);

    private:
        // Weak reference
        std::map<uint64_t, std::unique_ptr<Napi::ObjectReference>> objects_;

        std::atomic<uint64_t> next_handle_;
    };
}

#endif // ARCTIC_JS_ARCTIC_NODEJSOBJECTFACTORYDELEGATE_H_