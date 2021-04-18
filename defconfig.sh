#!/bin/bash
make savedefconfig
cp defconfig arch/arm/configs/omap3_mws4_defconfig
git add -u
git commit -m"update defconfig"
git push
