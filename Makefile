CC=g++
CFLAGS=-D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla
#CFLAGS=-D_DEBUG
# 1) libRun -> variable; testRun -> variable
# 2) build directory -> variable, -o flags for each compilation
# 3) @> @^ @< $<
# 4) PHONY - что делает


LIB_RUN_NAME     := libRun
TESTS_RUN_NAME   := testsRun
BUILD_DIR        := building
RUN_TESTS_DEFINE := -DRUN_ON_TESTS


# -------------------------   LIB RUN   -----------------------------


.PHONY: $(LIB_RUN_NAME)
$(LIB_RUN_NAME): $(BUILD_DIR)/main.o $(BUILD_DIR)/quadraticEquation.o $(BUILD_DIR)/colourfullPrint.o $(BUILD_DIR)/terminalArgs.o
	$(CC) $^ -o $(BUILD_DIR)/$(LIB_RUN_NAME) $(CFLAGS)

$(BUILD_DIR)/main.o: main.cpp $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

# $(LIB_RUN_NAME):
# for name in main quadraticEquation; do \
# 	$(BUILD_DIR)/$(name).o: quadraticEquationLib/$(name).cpp
# 		$(CC) -c $^ $(CFLAGS) -o $@

$(BUILD_DIR)/quadraticEquation.o: quadraticEquationLib/quadraticEquation.cpp $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR)/colourfullPrint.o: colourfullPrintLib/colourfullPrint.cpp $(BUILD_DIR)
	$(CC) -c $^ $(CFLAGS) -o $@

$(BUILD_DIR)/terminalArgs.o: terminalArgsLib/terminalArgs.cpp $(BUILD_DIR)
	$(CC) -c $^ $(CFLAGS) -o $@


.PHONY: test
test: $(TESTS_RUN_NAME)
	$(BUILD_DIR)/$(TESTS_RUN_NAME)

.PHONY: testrun
testrun: $(LIB_RUN_NAME)
	$(BUILD_DIR)/$(TESTS_RUN_NAME)

.PHONY: run
run: $(LIB_RUN_NAME)
	$(BUILD_DIR)/$(LIB_RUN_NAME)

# -------------------------   TESTS RUN     -----------------------------

.PHONY: $(TESTS_RUN_NAME)
$(TESTS_RUN_NAME): $(BUILD_DIR)/test_main.o $(BUILD_DIR)/quadraticEquation.o $(BUILD_DIR)/colourfullPrint.o $(BUILD_DIR)/testsGenerator.o
	$(CC) $^ -o $(BUILD_DIR)/$(TESTS_RUN_NAME) $(CFLAGS) $(RUN_TESTS_DEFINE)

$(BUILD_DIR)/testsGenerator.o: testsGeneratorLib/testsGenerator.cpp $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $@

$(BUILD_DIR)/test_main.o: main.cpp $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $@ $(RUN_TESTS_DEFINE)


# -------------------------   HELPER TARGETS   ---------------------------

.PHONY: $(BUILD_DIR)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# CAREFUL
.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o libRun

# g++ -o main.exe main.cpp quadraticEquationLib/quadraticEquation.cpp testsGeneratorLib/testsGenerator.cpp colourfullPrintLib/colourfullPrint.cpp -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla
