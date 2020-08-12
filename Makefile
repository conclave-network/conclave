# CONCLAVE - Making Bitcoin Scale And Be Useful
# Copyright (C) 2019-2020 Noel P. O'Donnell <noel.odonnell.2020@mumail.ie>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

# No release build for the moment
BUILD_DIR=cmake-build-debug

all: conclave
.PHONY: clean

install: conclave
	cp $(BUILD_DIR)/bin/* /usr/local/bin
	cp etc/* /usr/local/etc

dev: conclave
	valgrind --tool=memcheck --leak-check=yes ./$(BUILD_DIR)/bin/conclaved

test:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -D INCLUDE_TESTS:bool=YES ..
	cd $(BUILD_DIR) && make
	cd $(BUILD_DIR)/tests && ctest --verbose -R $(test)

conclave:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -D INCLUDE_SRC:bool=YES ..
	cd $(BUILD_DIR) && make

clean:
	rm -fr $(BUILD_DIR)
