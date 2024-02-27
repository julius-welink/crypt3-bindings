const { encrypt, verify } = require("./index");
const assert = require("assert");

function test1() {
  // Very simple tests to validate that C bindings are working
  const hash = encrypt("test", "salt");
  assert(hash === "$1$salt$No6gqynaE4urT3jScs91F/")
  assert(encrypt("test").length > 10);
}

function test2() {
  // Encrypt and then verify
  const hash = encrypt("test");
  assert(verify("test", hash) === true);
  assert(verify("incorrect_passwd", hash) === false);
}

try {
  test1();
  test2();
} catch (e) {
  console.error(e);
  process.exit(1);
}