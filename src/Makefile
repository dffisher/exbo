################################################################################
################################################################################
####                                                                        ####
####  MIT License                                                           ####
####                                                                        ####
####  Copyright 2016 Daniel F. Fisher                                       ####
####                                                                        ####
####  Permission is hereby granted, free of charge, to any person           ####
####  obtaining a copy of this software and associated documentation files  ####
####  (the "Software"), to deal in the Software without restriction,        ####
####  including without limitation the rights to use, copy, modify, merge,  ####
####  publish, distribute, sublicense, and/or sell copies of the Software,  ####
####  and to permit persons to whom the Software is furnished to do so,     ####
####  subject to the following conditions:                                  ####
####                                                                        ####
####  The above copyright notice and this permission notice shall be        ####
####  included in all copies or substantial portions of the Software.       ####
####                                                                        ####
####  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       ####
####  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    ####
####  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                 ####
####  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS   ####
####  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN    ####
####  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN     ####
####  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE      ####
####  SOFTWARE.                                                             ####
####                                                                        ####
################################################################################
################################################################################

################################################################################
# Source is at <repos_parent>/repos/exbo/src
# Build is at <repos_parent>/build/exbo
################################################################################

.SUFFIXES:

SRC = .
BUILD = ../../../build/exbo
DEP = $(BUILD)/dep
OBJ = $(BUILD)/obj
LIB = $(BUILD)/lib
BIN = $(BUILD)/bin
HdrTest = $(BUILD)/HdrTest
UnitTest = $(BUILD)/UnitTest

CPPFLAGS = -I$(SRC)/include -I/usr/include
CFLAGS = -std=c99 \
         -pedantic \
         -Wall \
         -Wshadow \
         -Wpointer-arith \
         -Wcast-qual \
         -Wcast-align \
         -Wwrite-strings \
         -Wconversion \
         -Waggregate-return \
         -Wstrict-prototypes \
         -Wmissing-prototypes \
         -Wmissing-declarations \
         -Wredundant-decls \
         -Wnested-externs \
         -Wno-long-long \
         -Winline

ALT_CFLAGS = -ansi -Wno-long-long -pedantic 

SRC_libexbo = \
    $(SRC)/exbo.c \


# SRC_test_exbo = \
# 

SRC_HdrTest = \
    $(SRC)/HdrTest/exbo_exbo_h.c \


SRC_UnitTest = \
    $(SRC)/UnitTest/exbo.c \


SRCS = \
    $(SRC_libexbo) \


OBJ_libexbo = $(SRC_libexbo:$(SRC)/%.c=$(OBJ)/%.o)
# OBJ_test_exbo = $(SRC_test_exbo:$(SRC)/%.c=$(OBJ)/%.o)
OBJ_HdrTest = $(SRC_HdrTest:$(SRC)/HdrTest/%.c=$(HdrTest)/obj/%.o)
BIN_UnitTest = $(SRC_UnitTest:$(SRC)/UnitTest/%.c=$(UnitTest)/bin/%)

ALL_TARGETS = \
    $(LIB)/libexbo.a \
#     $(BIN)/test_exbo \


all: $(ALL_TARGETS) \


HdrTest: $(OBJ_HdrTest)


UnitTest: $(BIN_UnitTest)


$(BIN)/test_exbo: $(OBJ_test_exbo) $(LIB)/libexbo.a
	@mkdir -pv $(@D)
	LIBRARY_PATH=$(LIB):${LIBRARY_PATH} \
	    $(CC) $(CPPFLAGS) $(CFLAGS) -lm -lexbo $(OBJ_test_exbo) -o $@

$(UnitTest)/bin/%: $(UnitTest)/obj/%.o $(LIB)/libexbo.a
	@mkdir -pv $(@D)
	LIBRARY_PATH=$(LIB):${LIBRARY_PATH} \
	    $(CC) $(CPPFLAGS) $(CFLAGS) -lm -lexbo $< -o $@

$(HdrTest)/dep/%.P: $(SRC)/HdrTest/%.c
	@mkdir -pv $(@D)
	@$(CC) -M $(CPPFLAGS) $(CFLAGS) -o $(HdrTest)/dep/$(*F).d $<
	@sed -e 's#^$(*F).o: #$(HdrTest)/obj/$(*F).o: #' \
	    -e 's# $(*F).c # $(SRC)/HdrTest/$(*F).c #' \
	    < $(HdrTest)/dep/$(*F).d > $(HdrTest)/dep/$(*F).P
	@sed -e 's#^$(*F).o: #$(HdrTest)/dep/$(*F).P: #' \
	    -e 's# $(*F).c # $(SRC)/HdrTest/$(*F).c #' \
	    < $(HdrTest)/dep/$(*F).d >> $(HdrTest)/dep/$(*F).P
	@rm -f $(HdrTest)/dep/$(*F).d
	@echo "Created $@"

$(UnitTest)/dep/%.P: $(SRC)/UnitTest/%.c
	@mkdir -pv $(@D)
	@$(CC) -M $(CPPFLAGS) $(CFLAGS) -o $(UnitTest)/dep/$(*F).d $<
	@sed -e 's#^$(*F).o: #$(UnitTest)/obj/$(*F).o: #' \
	    -e 's# $(*F).c # $(SRC)/UnitTest/$(*F).c #' \
	    < $(UnitTest)/dep/$(*F).d > $(UnitTest)/dep/$(*F).P
	@sed -e 's#^$(*F).o: #$(UnitTest)/dep/$(*F).P: #' \
	    -e 's# $(*F).c # $(SRC)/UnitTest/$(*F).c #' \
	    < $(UnitTest)/dep/$(*F).d >> $(UnitTest)/dep/$(*F).P
	@rm -f $(UnitTest)/dep/$(*F).d
	@echo "Created $@"

$(DEP)/%.P: $(SRC)/%.c
	@mkdir -pv $(@D)
	@$(CC) -M $(CPPFLAGS) $(CFLAGS) -o $(DEP)/$(*F).d $<
	@sed -e 's#^$(*F).o: #$(OBJ)/$(*F).o: #' \
	    -e 's# $(*F).c # $(SRC)/$(*F).c #' \
	    < $(DEP)/$(*F).d > $(DEP)/$(*F).P
	@sed -e 's#^$(*F).o: #$(DEP)/$(*F).P: #' \
	    -e 's# $(*F).c # $(SRC)/$(*F).c #' \
	    < $(DEP)/$(*F).d >> $(DEP)/$(*F).P
	@rm -f $(DEP)/$(*F).d
	@echo "Created $@"

$(HdrTest)/obj/%.o: $(SRC)/HdrTest/%.c
	@mkdir -pv $(@D)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(UnitTest)/obj/%.o: $(SRC)/UnitTest/%.c
	@mkdir -pv $(@D)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -pv $(@D)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(LIB)/libexbo.a: $(OBJ_libexbo)
	@mkdir -pv $(@D)
	ar rcs $@ $(OBJ_libexbo)

clean:
	rm -rf $(BUILD)

-include $(SRCS:$(SRC)/%.c=$(DEP)/%.P)
-include $(SRC_HdrTest:$(SRC)/HdrTest/%.c=$(HdrTest)/dep/%.P)
-include $(SRC_UnitTest:$(SRC)/UnitTest/%.c=$(UnitTest)/dep/%.P)

###################################
# The End
###################################
