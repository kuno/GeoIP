test:
	@NODE_ENV=test node_modules/.bin/mocha ./test/*.mocha.js
.PHONY: test
