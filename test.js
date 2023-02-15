const { encrypt } = require("./index");

function test() {
  // Very simple tests to validate that C bindings are working
  const hash = encrypt("test", "salt");
  console.assert(hash === "$1$salt$No6gqynaE4urT3jScs91F/")
  console.assert(encrypt("test").length > 10);
  console.log(encrypt("test"));
  console.log(encrypt("test"));
}

test();