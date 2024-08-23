CC=g++
# CFLAGS := -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla
CFLAGS = -D_DEBUG
# 1) libRun -> variable; testRun -> variable
# 2) build directory -> variable, -o flags for each compilation
# 3) @> @^ @< $<
# 4) PHONY - что делает


SOURCE_DIR       := source
LIB_RUN_NAME     := libRun
TESTS_RUN_NAME   := testsRun
BUILD_DIR        := building
RUN_TESTS_DEFINE := -DRUN_ON_TESTS
DEBUG            := 1
ASSERT_DEFINE    :=

ifeq ($(DEBUG), 0)
	ASSERT_DEFINE = -DNDEBUG
endif

.PHONY: $(LIB_RUN_NAME) test run testrun $(TESTS_RUN_NAME) $(BUILD_DIR) clean

# -------------------------   LIB RUN   -----------------------------

SRC := $(wildcard ./$(SOURCE_DIR)/*.cpp)
OBJ := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(notdir ${SRC}))

$(LIB_RUN_NAME): $(OBJ)
	$(CC) $^ -o $(BUILD_DIR)/$(LIB_RUN_NAME) $(CFLAGS)
#
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE)

test: $(TESTS_RUN_NAME)
	$(BUILD_DIR)/$(TESTS_RUN_NAME)

testrun: $(LIB_RUN_NAME)
	$(BUILD_DIR)/$(TESTS_RUN_NAME)

run: $(LIB_RUN_NAME)
	$(BUILD_DIR)/$(LIB_RUN_NAME)





# -------------------------   TESTS RUN     -----------------------------

OBJ_TESTS := $(patsubst %.cpp, $(BUILD_DIR)/TESTS_%.o, $(notdir ${SRC}))

$(TESTS_RUN_NAME): $(OBJ_TESTS)
	$(CC) $^ -o $(BUILD_DIR)/$(TESTS_RUN_NAME) $(CFLAGS) $(RUN_TESTS_DEFINE)

$(BUILD_DIR)/TESTS_%.o: $(SOURCE_DIR)/%.cpp $(BUILD_DIR)
	$(CC) -c $< $(CFLAGS) -o $@ $(ASSERT_DEFINE) $(RUN_TESTS_DEFINE)







# -------------------------   HELPER TARGETS   ---------------------------

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
clean:
	rm -f $(BUILD_DIR)/*.o libRun

# g++ -o main.exe main.cpp quadraticEquationLib/quadraticEquation.cpp testsGeneratorLib/testsGenerator.cpp colourfullPrintLib/colourfullPrint.cpp -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla
