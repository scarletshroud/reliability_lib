#!/bin/bash

# Configuration
QEMU=qemu-system-riscv32
MACHINE=virt        # или "virt" — зависит от цели
KERNEL=build/main.elf
GRAPHIC="-nographic"     # отключить графику, вывод в терминал
DEBUG_PORT=1234

# Check Elf-file
if [ ! -f "$KERNEL" ]; then
  echo "Image $KERNEL doesn't exists. Build project with CMake."
  exit 1
fi

echo "Запуск QEMU с $KERNEL ..."
$QEMU -machine $MACHINE -kernel $KERNEL $GRAPHIC -s -S -monitor none -serial mon:stdio
