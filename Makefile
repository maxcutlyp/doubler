PLUGIN_NAME = hello_world
LV2_DIR = ${HOME}/.lv2/
CFLAGS = `pkg-config --cflags lv2` -fPIC -DPIC -Wall -Wextra
LDFLAGS = `pkg-config --libs lv2` -shared

all: $(PLUGIN_NAME).lv2/$(PLUGIN_NAME).so $(PLUGIN_NAME).lv2/manifest.ttl $(PLUGIN_NAME).lv2/$(PLUGIN_NAME).ttl

$(PLUGIN_NAME).lv2/$(PLUGIN_NAME).so: $(PLUGIN_NAME).c
	mkdir -p $(PLUGIN_NAME).lv2
	cc $(CFLAGS) $(PLUGIN_NAME).c $(LDFLAGS) -o $(PLUGIN_NAME).lv2/$(PLUGIN_NAME).so

$(PLUGIN_NAME).lv2/manifest.ttl: manifest.ttl
	cp manifest.ttl $(PLUGIN_NAME).lv2/manifest.ttl

$(PLUGIN_NAME).lv2/$(PLUGIN_NAME).ttl: $(PLUGIN_NAME).ttl
	cp $(PLUGIN_NAME).ttl $(PLUGIN_NAME).lv2/$(PLUGIN_NAME).ttl

install: $(PLUGIN_NAME).lv2/$(PLUGIN_NAME).so
	cp -r $(PLUGIN_NAME).lv2 $(LV2_DIR)

clean:
	rm -rf $(PLUGIN_NAME).lv2
