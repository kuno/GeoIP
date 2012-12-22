test:
	@NODE_ENV=test sh ./bootstrap.sh && node-gyp rebuild && node_modules/.bin/mocha ./test/*.mocha.js
rebuild:
	@NODE_ENV=test node-gyp rebuild
bootstrap:
	@NODE_ENV=test sh ./bootstrap.sh
mem_test:
	@NODE_ENV=test sh ./bootstrap.sh && node-gyp rebuild && valgrind --leak-check=full ./test/memory_leak.js
.PHONY: test