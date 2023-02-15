# Introduction

Implements crypt3 C bindings for Node.js. Provides a simple interface to the crypt3 function with MD5 hashing. This
is useful for generating password hashes for Linux systems.

# Installation

Only linux platform is supported.

```bash
npm install crypt3-bindings
```

# Usage

```js
const { encrypt } = require('crypt3-bindings');

console.log(crypt3.encrypt("test")); // random salt will be generated
console.log(crypt3.encrypt("test", "salt")); // provide your own salt
```