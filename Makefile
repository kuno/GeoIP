install: nodejs
	@npm install
iojs:
	@NODE_ENV=development bash ./cli/iojs.sh
nodejs:
	@NODE_ENV=development bash ./cli/nodejs.sh
rebuild: iojs install
	@NODE_ENV=development ./node_modules/.bin/node-gyp rebuild --verbose
bootstrap: install
	@NODE_ENV=development sh ./cli/bootstrap.sh
mem_test: install rebuild bootstrap
	@NODE_ENV=test valgrind --leak-check=full ./test/memory_leak.js
test: install rebuild bootstrap
	@NODE_ENV=test DEBUG=geoip:* node_modules/.bin/mocha ./test/*.mocha.js
.PHONY: install iojs rebuild bootstrap test
