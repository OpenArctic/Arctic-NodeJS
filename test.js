var addon = require('bindings')('arctic');

addon.configure_module_path(__dirname);

// test

setInterval(function () { }, 0);

(async () => {
    try {
        var agent = addon.create_host_agent(1, []);

        var object = {};
        object.foo = "bar";
        object.func = function (value) {

            console.log("func has been called.(" + value + ")");
        }
        agent.export("instance", object);
        var res = agent.start();
        if (res != 0) {
            return;
        }

        var localInstance = await agent.find(1, "instance");
        console.log("localInstance.foo = " + await localInstance.getProperty("foo"));

        await localInstance.setProperty("aaa", 123321);
        console.log("localInstance.aaa = ", await localInstance.getProperty("aaa"));

        localInstance.invoke("func", "fake parameters");

        var one = function () {

            console.log("one event has been fired.");
        }
        var two = function () {

            console.log("two event has been fired.");
        }
        localInstance.addEventListener("one", one);
        localInstance.addEventListener("two", two);
        localInstance.removeEventListener("one", one);
        localInstance.fireEvent("one");
        localInstance.fireEvent("two");

    } catch (e) {
        console.log(e);
    }
})();
