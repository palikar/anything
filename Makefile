BUILD_DEBUG_DIR?=build

plain_build:
	mkdir -p $(BUILD_DEBUG_DIR)	
	cd $(BUILD_DEBUG_DIR) && cmake .. -DCMAKE_BUILD_TYPE=Debug
	cd $(BUILD_DEBUG_DIR) && make -j4

count_line:
	@cloc CMakeLists.txt ./include/ ./examples/

format_project:
	@echo "Fromating the source directory..."
	@find ./include/ -iname *.hpp -o -iname *.cpp | xargs clang-format -i -style=file

repo_stats:
	@gitstats ./ ./stats

clean:
	@rm -rf build_*	
	@rm -rf site
	@rm -ff stats

