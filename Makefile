test:
	@NODE_ENV=test node-gyp rebuild && node_modules/.bin/mocha ./test/*.mocha.js
rebuild:
	@NODE_ENV=test node-gyp rebuild
bootstrap:
	@NODE_ENV=test sh ./bootstrap.sh
benchmar:
.PHONY: test
