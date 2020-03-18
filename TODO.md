# TODO
* Debug/release builds
* Make component classes non-copyable/moveable
    * See here:https://stackoverflow.com/questions/23709690/most-concise-way-to-disable-copying-class-in-c11
* Implement rule-of-5 for object classes (Address, Script, etc)
* Ensure balances of all kinds of addresses can be read
* Get rid of mongoose, use Poco HTTP server instead
* Implement fix for boost::property_tree stringifying everything
    * See https://stackoverflow.com/questions/2855741/why-does-boost-property-tree-write-json-save-everything-as-string-is-it-possibl
* Implement asserts for script factories, have them switched off for release builds
* ElectrumXClient: refactor into client and TCP logic, use DI to test
* Digitally sign all RPC messages
* Change BOOST_TEST_MODULE names to include full path
* Stop using boost property tree for JSON and use this library instead: https://github.com/nlohmann/json
* Write unit tests for << operator for structs
* More tests for scripts: test empty scripts, ptree init+casting copy/move ctors, copy-assignment, etc
* Cleanup JSON functions
* Change all occurances of `TxId` to `Txid`
* Tests for `util/json`
* Split `tests/CMakeLists.txt` over test directories
* Refactor ConcalveNode and subsystems so they use DI and builder pattern