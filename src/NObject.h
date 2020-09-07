#ifndef ARCTIC_JS_ARCTIC_NOBJECT_H_
#define ARCTIC_JS_ARCTIC_NOBJECT_H_

#include <napi.h>
#include "Arctic.h"

namespace arctic {
    class NObject : public Napi::ObjectWrap<NObject> {
    public:
        class EventHandler : public ObjectEventListener {
        public:
            EventHandler(std::string event_type, Napi::Function& callback);
            virtual ~EventHandler() {};

            Napi::Function GetFunction();

            void DispatchEvent(Object* instance, std::string event_type, std::vector<NamedVariant> params) override;

        private:
            std::unique_ptr<Napi::FunctionReference> native_;
        };

        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        NObject(const Napi::CallbackInfo& info);

        Handle GetHandle() { return native_->GetHandle(); }

        static Napi::Object NewInstance(Napi::Env env, Object* native);

        Napi::Value GetProperty(const Napi::CallbackInfo& info);
        Napi::Value SetProperty(const Napi::CallbackInfo& info);

        Napi::Value Invoke(const Napi::CallbackInfo& info);

        Napi::Value AddEventListener(const Napi::CallbackInfo& info);
        Napi::Value RemoveEventListener(const Napi::CallbackInfo& info);
        Napi::Value FireEvent(const Napi::CallbackInfo& info);

    private:

        std::unique_ptr<Object> native_;

        std::vector<std::unique_ptr<EventHandler>> event_listeners_;
    };
}

#endif // ARCTIC_JS_ARCTIC_NOBJECT_H_