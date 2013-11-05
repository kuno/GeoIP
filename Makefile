install:
	@npm install 
test: install
	@NODE_ENV=test sh ./bootstrap.sh && node-gyp rebuild && node_modules/.bin/mocha ./test/*.mocha.js
rebuild: install
	@NODE_ENV=test node-gyp rebuild
bootstrap: install
	@NODE_ENV=test sh ./bootstrap.sh
mem_test: install
	@NODE_ENV=test sh ./bootstrap.sh && node-gyp rebuild && valgrind --leak-check=full ./test/memory_leak.js
.PHONY: test
