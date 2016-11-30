# Make tasks to create and deploy documentation
.PHONY: docs clean-docs deploy-docs doxygen update-docs-from-readme jekyll-layouts

GAUGES_CODE="539c6c8de32bb442640005f3"

jekyll-layouts:
	git submodule update --init

doxygen:
	doxygen doxygen.config

update-docs-from-readme: jekyll-layouts
	install -d docs
	echo "gaugesid: $(GAUGES_CODE)" > docs/_config.yml
	echo "projecturl: $(PROJECT_URL)" >> docs/_config.yml
	echo "basesite: http://www.unwiredcouch.com" >> docs/_config.yml
	echo "markdown: kramdown" >> docs/_config.yml
	echo "---" > docs/index.md
	echo "layout: project" >> docs/index.md
	echo "title: $(NAME)" >> docs/index.md
	echo "---" >> docs/index.md
	cat README.md >> docs/index.md

docs: doxygen update-docs-from-readme

clean-docs:
	rm -rf ./docs
