test:
	@NODE_ENV=test node-gyp clean configure build && node_modules/.bin/mocha ./test/*.mocha.js
benchmar:
	@NODE_ENV=test node-gyp clean configure build
benchmar:
.PHONY: test
