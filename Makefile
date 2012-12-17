test:
	@NODE_ENV=test node-gyp rebuild && node_modules/.bin/mocha ./test/*.mocha.js
rebuild:
	@NODE_ENV=test node-gyp rebuild
bootstrap:
	@NODE_ENV=test sh ./bootstrap.sh
memleak:
	@NODE_ENV=test valgrind --leak-check=full ./test/memory_leak.js
benchmar:
.PHONY: test