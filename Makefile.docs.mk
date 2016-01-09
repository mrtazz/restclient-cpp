# Make tasks to create and deploy documentation
.PHONY: docs clean-docs deploy-docs doxygen jekyll

GAUGES_CODE="539c6c8de32bb442640005f3"

doxygen:
	doxygen doxygen.config

jekyll:
	install -d docs
	echo "gaugesid: $(GAUGES_CODE)" > docs/_config.yml
	echo "projecturl: $(PROJECT_URL)" >> docs/_config.yml
	echo "basesite: http://www.unwiredcouch.com" >> docs/_config.yml
	echo "markdown: redcarpet" >> docs/_config.yml
	echo "---" > docs/index.md
	echo "layout: project" >> docs/index.md
	echo "title: $(NAME)" >> docs/index.md
	echo "---" >> docs/index.md
	cat README.md >> docs/index.md

docs: doxygen jekyll

clean-docs:
	rm -rf ./docs

deploy-docs: docs
	@cd docs && git init && git remote add upstream "https://${GH_TOKEN}@github.com/mrtazz/$(NAME).git" && \
	git submodule add https://github.com/mrtazz/jekyll-layouts.git ./_layouts && \
	git submodule update --init && \
	git fetch upstream && git reset upstream/gh-pages && \
	git config user.name 'Daniel Schauenberg' && git config user.email d@unwiredcouch.com && \
	touch . && git add -A . && \
	git commit -m "rebuild pages at $(VERSION)" && \
	git push -q upstream HEAD:gh-pages

