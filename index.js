'use strict'
const bindings = require('node-gyp-build')(__dirname)
const crypto = require('crypto');

const SALT = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz./';

function generateSalt(length = 8) {
  let salt = "";
  const randomness = crypto.randomBytes(length);
  for (const byte of randomness) {
    salt += SALT[byte % SALT.length];
  }
  return salt;
}

module.exports = {
  encrypt(passwd, salt) {
    if (!salt) {
      salt = generateSalt();
    }
    const result = bindings.hashPassword(passwd, salt, 1);

    if (!result.startsWith("$1$")) {
      throw new Error("could not generate hash (unsupported os?)");
    }

    return result;
  }
}