# R2FS Makefile
# 
# 使用方法：
#   make configure  - 配置项目
#   make build      - 构建项目
#   make run        - 运行项目
#   make clean      - 清理构建文件

# 默认配置
RTEMS_ROOT ?= $(HOME)/quick-start/rtems/6
BSP ?= arm/realview_pbx_a9_qemu

# 项目配置
PROJECT_NAME = main
TARGET = $(PROJECT_NAME).exe

# 源文件
SOURCES = src/main.c

# 默认目标
all: build

# 配置项目
configure:
	@echo "Configuring R2FS project..."
	./waf configure --rtems=$(RTEMS_ROOT) --rtems-bsp=$(BSP)

# 构建项目
build:
	@echo "Building R2FS project..."
	./waf

# 运行项目
run: build
	@echo "Running R2FS project..."
	@export QEMU_AUDIO_DRV="none" && \
	qemu-system-arm -no-reboot -nographic -M realview-pbx-a9 -m 256M \
		-kernel ./build/arm-rtems6-realview_pbx_a9_qemu/$(TARGET)

# 清理构建文件
clean:
	@echo "Cleaning build files..."
	./waf clean
	rm -rf build/

# 完全清理
distclean: clean
	./waf distclean

# 显示帮助信息
help:
	@echo "R2FS - RTEMS F2FS-inspired Filesystem"
	@echo ""
	@echo "Available targets:"
	@echo "  configure  - Configure the project"
	@echo "  build      - Build the project"
	@echo "  run        - Run the project in QEMU"
	@echo "  clean      - Clean build files"
	@echo "  distclean  - Complete cleanup"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Configuration:"
	@echo "  RTEMS_ROOT - Path to RTEMS installation (default: $(RTEMS_ROOT))"
	@echo "  BSP        - Target BSP (default: $(BSP))"
	@echo ""
	@echo "Example usage:"
	@echo "  make configure"
	@echo "  make build"
	@echo "  make run"

# 检查源文件
check-sources:
	@echo "Checking source files..."
	@for file in $(SOURCES); do \
		if [ -f "$$file" ]; then \
			echo "  ✓ $$file"; \
		else \
			echo "  ✗ $$file (missing)"; \
		fi; \
	done

# 显示项目信息
info:
	@echo "R2FS Project Information:"
	@echo "  Project name: $(PROJECT_NAME)"
	@echo "  Target: $(TARGET)"
	@echo "  RTEMS root: $(RTEMS_ROOT)"
	@echo "  BSP: $(BSP)"
	@echo "  Source files:"
	@for file in $(SOURCES); do echo "    $$file"; done

.PHONY: all configure build run clean distclean help check-sources info
