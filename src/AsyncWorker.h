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
            Error err;
            napi_value value = ConvertToNapiValue(return_value_, err);
            if (err.IsError()) {
                Napi::String err_msg = Napi::String::New(Env(), err.msg);
                deferred.Reject(err_msg);
            }
            else {
                deferred.Resolve(value);
            }
        }

        void OnError(Napi::Error const& error) override {
            deferred.Reject(error.Value());
        }

        Napi::Promise GetPromise() { return deferred.Promise(); };

    private:

        template <typename T>
        napi_value ConvertToNapiValue(T value, Error& err) {
            return Env().Undefined();
        }

        template <>
        napi_value ConvertToNapiValue(Object* value, Error& err) {
            Napi::Object obj = NObject::NewInstance(Env(), value);
            return obj;
        }

        template <>
        napi_value ConvertToNapiValue(Variant value, Error& err) {
            return Variant2NapiValue(Env(), value);
        }

        template <>
        napi_value ConvertToNapiValue(MaybeError<Variant> value, Error& err) {
            if (value.IsError()) {
                err = value.err;
                return Env().Undefined();
            }
            return Variant2NapiValue(Env(), value.value);
        }

        T return_value_;
        std::function<T()> internal_work_;
        Napi::Promise::Deferred deferred;
    };
}

#endif // ARCTIC_JS_ARCTIC_ASYNCWORKER_H_