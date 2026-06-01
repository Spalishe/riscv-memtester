CC := riscv64-linux-gnu-gcc

TARGET := memtester/memtester
OBJ_DIR := ./memtester/obj

SRCS_C := $(shell find memtester -name '*.c' -not -path '*/obj/*')
SRCS_S := $(shell find memtester -name '*.S' -not -path '*/obj/*')

OBJS_C := $(patsubst memtester/%.c,$(OBJ_DIR)/%.o,$(SRCS_C))
OBJS_S := $(patsubst memtester/%.S,$(OBJ_DIR)/%.o,$(SRCS_S))
OBJS   := $(OBJS_C) $(OBJS_S)

CFLAGS  := -Wall -MMD -MP -fno-pic -mno-relax -mcmodel=medany -fno-builtin -mabi=lp64 -march=rv64ima_zicsr_zifencei
ASFLAGS := -Wall -fno-pic -mno-relax -mcmodel=medany -mabi=lp64 -march=rv64ima_zicsr_zifencei
LDFLAGS := -nostdlib -T memtester/link.ld -Wl,--no-relax -no-pie -Wl,--no-warn-rwx-segments -mabi=lp64 -march=rv64ima_zicsr_zifencei


.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "  LD      $@"
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: memtester/%.c
	@mkdir -p $(dir $@)
	@echo "  CC      $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: memtester/%.S
	@mkdir -p $(dir $@)
	@echo "  AS      $<"
	@$(CC) $(ASFLAGS) -c $< -o $@

-include $(OBJS:.o=.d)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(TARGET)

