# ================================================================
#  APM32F402 RTOS Demo - GCC Makefile
# ================================================================
#  Toolchain: ARM GNU Toolchain (arm-none-eabi-gcc)
#  MCU:       APM32F402xx (Cortex-M4, 128KB Flash, 32KB RAM)
#  OS:        CMSIS-FreeRTOS (CMSIS RTOS2 API)
#
#  This project is FULLY SELF-CONTAINED -- no SDK dependencies.
#  Copy this folder anywhere and run "make".
# ================================================================

# --- Project name -------------------------------------------------
TARGET    := APM32F402_RTOS

# --- Toolchain ----------------------------------------------------
PREFIX    := arm-none-eabi
CC        := $(PREFIX)-gcc
CXX       := $(PREFIX)-g++
AS        := $(PREFIX)-gcc -x assembler-with-cpp
OBJCOPY   := $(PREFIX)-objcopy
OBJDUMP   := $(PREFIX)-objdump
SIZE      := $(PREFIX)-size
GDB       := $(PREFIX)-gdb

# --- Directories (all relative to Makefile location) --------------
APP_DIR   := Application
BSP_DIR   := BSP
LIB_DIR   := Libraries
RTOS_DIR  := Middlewares/CMSIS-FreeRTOS

# --- Build & output dirs -------------------------------------------
BUILD_DIR := build
OUT_DIR   := output

# --- MCU flags ----------------------------------------------------
CPU       := -mcpu=cortex-m4
FPU       := -mfpu=fpv4-sp-d16 -mfloat-abi=hard
MCU_FLAGS := $(CPU) -mthumb $(FPU)

# --- Compiler flags -----------------------------------------------
ASFLAGS   := $(MCU_FLAGS) -Wall -fdata-sections -ffunction-sections
CFLAGS    := $(MCU_FLAGS) \
             -Wall -Wextra -g3 -Os \
             -fdata-sections -ffunction-sections \
             -DUSE_STDPERIPH_DRIVER \
             -DBOARD_APM32F402_403_TINY \
             -DAPM32F40X

LDFLAGS   := $(MCU_FLAGS) \
             -T$(LIB_DIR)/Device/Source/gcc/apm32f402_403xb_flash.ld \
             -Wl,--gc-sections \
             -Wl,-Map=$(OUT_DIR)/$(TARGET).map \
             --specs=nano.specs \
             --specs=nosys.specs

# --- Include paths ------------------------------------------------
INCLUDES  := -I$(APP_DIR)/Include \
             -I$(BSP_DIR)/Include \
             -I$(LIB_DIR)/Device/Include \
             -I$(LIB_DIR)/StdPeriphDriver/inc \
             -I$(LIB_DIR)/CMSIS/Core/Include \
             -I$(LIB_DIR)/CMSIS/Include \
             -I$(RTOS_DIR)/Source/include \
             -I$(RTOS_DIR)/CMSIS/RTOS2/FreeRTOS/Include \
             -I$(RTOS_DIR)/Source/portable/GCC/ARM_CM4F

# --- Source files -------------------------------------------------
# Application
C_SRCS    := $(wildcard $(APP_DIR)/Source/*.c)

# BSP
C_SRCS    += $(wildcard $(BSP_DIR)/Source/*.c)

# Device (system)
C_SRCS    += $(LIB_DIR)/Device/Source/system_apm32f402_403.c

# Standard peripheral driver
C_SRCS    += $(wildcard $(LIB_DIR)/StdPeriphDriver/src/*.c)

# FreeRTOS kernel
C_SRCS    += $(RTOS_DIR)/Source/croutine.c
C_SRCS    += $(RTOS_DIR)/Source/event_groups.c
C_SRCS    += $(RTOS_DIR)/Source/list.c
C_SRCS    += $(RTOS_DIR)/Source/queue.c
C_SRCS    += $(RTOS_DIR)/Source/stream_buffer.c
C_SRCS    += $(RTOS_DIR)/Source/tasks.c
C_SRCS    += $(RTOS_DIR)/Source/timers.c

# FreeRTOS port (GCC/Cortex-M4F)
C_SRCS    += $(RTOS_DIR)/Source/portable/GCC/ARM_CM4F/port.c
C_SRCS    += $(RTOS_DIR)/Source/portable/Common/mpu_wrappers.c
C_SRCS    += $(RTOS_DIR)/Source/portable/MemMang/heap_4.c

# CMSIS RTOS2 wrapper
C_SRCS    += $(RTOS_DIR)/CMSIS/RTOS2/FreeRTOS/Source/cmsis_os2.c
C_SRCS    += $(RTOS_DIR)/CMSIS/RTOS2/FreeRTOS/Source/freertos_evr.c
C_SRCS    += $(RTOS_DIR)/CMSIS/RTOS2/FreeRTOS/Source/handlers.c
C_SRCS    += $(RTOS_DIR)/CMSIS/RTOS2/FreeRTOS/Source/os_systick.c

# GCC startup (assembler)
ASM_SRCS  := $(LIB_DIR)/Device/Source/gcc/startup_apm32f402_403.S

# --- Object files -------------------------------------------------
C_OBJS    := $(C_SRCS:%.c=$(BUILD_DIR)/%.o)
ASM_OBJS  := $(ASM_SRCS:%.S=$(BUILD_DIR)/%.o)
ALL_OBJS  := $(C_OBJS) $(ASM_OBJS)

# --- Build rules --------------------------------------------------

.PHONY: all clean flash debug size

all: dirs $(OUT_DIR)/$(TARGET).elf $(OUT_DIR)/$(TARGET).bin $(OUT_DIR)/$(TARGET).hex size

dirs:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(sort $(dir $(ALL_OBJS)))

# --- Compile C files ----------------------------------------------
$(BUILD_DIR)/%.o: %.c
	@echo "CC    $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# --- Assemble startup file ----------------------------------------
$(BUILD_DIR)/%.o: %.S
	@echo "AS    $<"
	@$(AS) $(ASFLAGS) $(INCLUDES) -c $< -o $@

# --- Link ---------------------------------------------------------
$(OUT_DIR)/$(TARGET).elf: $(ALL_OBJS)
	@echo "LINK  $@"
	@$(CC) $(ALL_OBJS) $(LDFLAGS) -o $@

$(OUT_DIR)/$(TARGET).bin: $(OUT_DIR)/$(TARGET).elf
	@echo "BIN   $@"
	@$(OBJCOPY) -O binary $< $@

$(OUT_DIR)/$(TARGET).hex: $(OUT_DIR)/$(TARGET).elf
	@echo "HEX   $@"
	@$(OBJCOPY) -O ihex $< $@

# --- Size report --------------------------------------------------
size: $(OUT_DIR)/$(TARGET).elf
	@echo ""
	@echo "--- Size ---"
	@$(SIZE) $<

# --- Clean --------------------------------------------------------
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR) $(OUT_DIR)
	@echo "Done."

# --- Flash with J-Link --------------------------------------------
flash: all
	@echo "Flashing with J-Link..."
	@echo "Make sure J-Link is connected and JLinkExe is in PATH."
	JLinkExe -device APM32F402xx -if SWD -speed 4000 -autoconnect 1 \
	         -CommanderScript flash.jlink

# --- Debug with GDB + J-Link GDB Server ---------------------------
debug: all
	@echo "Starting GDB session..."
	@echo "Start J-Link GDB Server first in another terminal:"
	@echo "  JLinkGDBServer -device APM32F402xx -if SWD -speed 4000"
	@$(GDB) -ex "target remote localhost:2331" \
	        -ex "monitor reset" \
	        -ex "monitor halt" \
	        -ex "load" \
	        -ex "monitor reset" \
	        $(OUT_DIR)/$(TARGET).elf
