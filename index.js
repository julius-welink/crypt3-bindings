'use strict'
const bindings = require('node-gyp-build')(__dirname)

module.exports = {
  encrypt(passwd, salt) {
    return bindings.hashPassword(passwd, salt, 1)
  }
}