#!/bin/bash
make savedefconfig
cp defconfig arch/arm/configs/omap3_mws4_defconfig
hg commit -m"update defconfig"
hg push
