var addon = require('bindings')('arctic');

addon.configure_module_path(__dirname);

module.exports = addon;

// test

(async () => {
    try {
        var agent = addon.create_host_agent(1, [
            {"routing_id": 2, "cmdline": "path/to/x"},
        ]);
        
        var myinstance = {};
        myinstance.foo = "bar";
        myinstance.func = function (addon_value) {

            console.log("func has been called.(" + addon_value + ")");
        }

        agent.export("myinstance", myinstance);

        var instance = await agent.find(1, "myinstance");
        console.log("myinstance.foo = " + await instance.getProperty("foo"));
        var xxx = await instance.setProperty("aaa", 123321);
        console.log("myinstance.aaa = ", await instance.getProperty("aaa"));
        instance.invoke("func", "hohohho");
        var one = function() {

            console.log("one event has been fired.");
        }
        var two = function() {

            console.log("one event has been fired.");
        }
        instance.addEventListener("one", one);
        instance.removeEventListener("one", two);
        instance.fireEvent("one");

        var x = agent.getRoutingId();
    } catch (e) {
        console.log(e);
    }
})();

// setInterval(function () {}, 0);