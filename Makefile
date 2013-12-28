test: install rebuild bootstrap
	@NODE_ENV=test DEBUG=geoip:* node_modules/.bin/mocha ./test/*.mocha.js
install:
	@npm install
rebuild: install
	@NODE_ENV=test node-gyp rebuild
bootstrap: install
	@NODE_ENV=test sh ./bootstrap.sh
mem_test: install rebuild bootstrap
	@NODE_ENV=test valgrind --leak-check=full ./test/memory_leak.js

.PHONY: test install rebuild bootstrap
