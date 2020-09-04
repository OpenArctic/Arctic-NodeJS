#pragma once

#include <functional>
#include <future>
#include <map>
#include <variant>
#include <vector>

#include "DataType.h"
#include "Object.h"

namespace arctic {
    typedef struct HostAgentCreateParams {
        // current process routing id
        uint8_t routing_id;

        // child processes info 
        // K: routing id V: startup command line
        std::map<uint8_t, std::string> child_processes_info;
    } HostAgentCreateParams;

    typedef struct ClientAgentCreateParams {
        // current process routing id
        uint8_t routing_id;
    } ClientAgentCreateParams;

    class AgentDelegate {
    public:
        AgentDelegate() {};
        virtual ~AgentDelegate() {};

        virtual void Received(Packet* p) = 0;
        virtual void Received(std::vector<NamedVariant>& nvs) = 0;
    };

    class Agent {
    public:
        Agent(AgentDelegate* delegate) : delegate_(delegate) {};
        virtual ~Agent() {};

        virtual int Start() = 0;
        virtual void Stop() = 0;

        virtual void Export(std::string id, Object* instance) = 0;
        virtual std::future<Object*> Find(uint8_t routing_id, std::string id) = 0;
        virtual void Find(uint8_t routing_id, std::string id, std::function<void(Object*)>& callback) = 0;

        virtual void Send(Packet* p) = 0;
        virtual void Send(uint8_t routing_id, std::vector<NamedVariant> nvs) = 0;

        virtual ObjectFactory* GetObjectFactory() = 0;

        uint8_t GetRoutingId() { return routing_id_; };

        AgentDelegate* GetDelegate() { return delegate_; };

    protected:
        void SetRoutingId(uint8_t routing_id) { routing_id_ = routing_id; };

    private:
        AgentDelegate* delegate_;

        uint8_t routing_id_;
    };

    typedef Agent* (*CreateHostAgentFunc)(HostAgentCreateParams& cp, AgentDelegate* delegate);
    typedef Agent* (*CreateClientAgentFunc)(ClientAgentCreateParams& cp, AgentDelegate* delegate);
}

