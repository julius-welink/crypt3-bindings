const { encrypt } = require("./index");

function test() {
  const hash = encrypt("test", "salt");
  console.assert(hash === "$1$salt$No6gqynaE4urT3jScs91F/")
}

test();