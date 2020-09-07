var addon = require('bindings')('arctic');

addon.configure_module_path(__dirname);

module.exports = addon;