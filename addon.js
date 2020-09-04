var addon = require('bindings')('arctic');

addon.configure_module_path(__dirname);

// test

(async () => {
    try {
        var agent = addon.create_host_agent(1, [
            {"routing_id": 2, "cmdline": "path/to/x"},
        ]);
        
        var myinstance = {};
        myinstance.foo = "bar";

        agent.export("myinstance", myinstance);

        var instance = await agent.find(1, "myinstance");
        console.log("myinstance.foo = " + instance.getProperty("foo"));

        var x = agent.getRoutingId();
    } catch (e) {
        console.log(e);
    }
})();

setInterval(function () {}, 0);