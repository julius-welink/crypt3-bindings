'use strict'
const bindings = require('node-gyp-build')(__dirname)
const crypto = require('crypto');

module.exports = {
  encrypt(passwd, salt) {
    if (!salt) {
      salt = crypto.randomBytes(8).toString('hex');
    }
    return bindings.hashPassword(passwd, salt, 1)
  }
}