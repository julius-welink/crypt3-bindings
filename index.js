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

function encrypt(passwd, salt) {
  if (!salt) {
    salt = generateSalt();
  }
  const result = bindings.hashPassword(passwd, salt, 1);

  if (!result.startsWith("$1$")) {
    throw new Error("could not generate hash (unsupported os?)");
  }

  return result;
}

function verify(passwd, hash) {
  // get salt from hash
  const salt = hash.split('$')[2];

  if (!salt) {
    throw new Error("could not extract salt from hash");
  }

  const newHash = encrypt(passwd, salt);

  return newHash === hash;
}

module.exports = { encrypt, verify }