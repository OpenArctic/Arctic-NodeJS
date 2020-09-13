#ifndef ARCTIC_JS_ARCTIC_NAGENT_H_
#define ARCTIC_JS_ARCTIC_NAGENT_H_

#include <napi.h>
#include <uv.h>
#include "Arctic.h"
#include "NodeJsObjectFactoryDelegate.h"

namespace arctic {
    class NAgent : public Napi::ObjectWrap<NAgent> {
    public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        NAgent(const Napi::CallbackInfo& info);

        static Napi::Object CreateHostAgent(const Napi::CallbackInfo& info, std::string& module_path);
        static Napi::Object CreateClientAgent(const Napi::CallbackInfo& info, std::string& module_path);

    private:
        static void IdleTask(uv_idle_t* idle);

        void InstallIdleTask();

        Napi::Value Start(const Napi::CallbackInfo& info);
        Napi::Value Stop(const Napi::CallbackInfo& info);

        Napi::Value Export(const Napi::CallbackInfo& info);
        Object* FindInternal(uint8_t routing_id, std::string id);
        Napi::Value Find(const Napi::CallbackInfo& info);

        Napi::Value GetRoutingId(const Napi::CallbackInfo& info);

        void InstallNodeJsObjectFactoryDelegate();

        void Finalize(Napi::Env env) override;

        uv_idle_t* main_loop_handle_;

        NodeJsObjectFactoryDelegate* object_factory_delegate_;
        Agent* agent_;
    };
}

#endif // ARCTIC_JS_ARCTIC_NAGENT_H_