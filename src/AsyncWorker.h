#ifndef ARCTIC_JS_ARCTIC_ASYNCWORKER_H_
#define ARCTIC_JS_ARCTIC_ASYNCWORKER_H_

namespace arctic {
    template <typename T>
    class PromiseWorker : public Napi::AsyncWorker {
    public:
        PromiseWorker(Napi::Env& env, std::function<T()> work) :
            Napi::AsyncWorker(env), internal_work_(work),
            deferred(Napi::Promise::Deferred::New(env)) {
        }

        virtual ~PromiseWorker() {};

        void Execute() override {
            return_value_ = internal_work_();
        }

        void OnOK() override {
            napi_value value = ConvertToNapiValue(return_value_);
            deferred.Resolve(value);
        }

        void OnError(Napi::Error const& error) override {
            deferred.Reject(error.Value());
        }

        Napi::Promise GetPromise() { return deferred.Promise(); };

    private:

        template <typename T>
        napi_value ConvertToNapiValue(T value) {
            return Env().Undefined();
        }

        template <>
        napi_value ConvertToNapiValue(Object* value) {
            Napi::Object obj = NObject::NewInstance(Env(), value);
            return obj;
        }

        template <>
        napi_value ConvertToNapiValue(Variant value) {
            return Variant2NapiValue(Env(), value);
        }

        T return_value_;
        std::function<T()> internal_work_;
        Napi::Promise::Deferred deferred;
    };
}

#endif // ARCTIC_JS_ARCTIC_ASYNCWORKER_H_