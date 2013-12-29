install: install_memwatch
	@npm install
install_memwatch:
	@NODE_ENV=development bash ./cli/install_memwatch.sh
rebuild: install
	@NODE_ENV=development node-gyp rebuild
bootstrap: install
	@NODE_ENV=development sh ./cli/bootstrap.sh
mem_test: install rebuild bootstrap
	@NODE_ENV=test valgrind --leak-check=full ./test/memory_leak.js
test: install rebuild bootstrap
	@NODE_ENV=test DEBUG=geoip:* node_modules/.bin/mocha ./test/*.mocha.js 
.PHONY: install rebuild bootstrap test
