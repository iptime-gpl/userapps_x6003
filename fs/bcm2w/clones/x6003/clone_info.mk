PRODUCT_ID := x6003
CLASS_ID:=iptime
BCM5357C0_SUPPORT:=y
#NVRAM_FILENAME:=4708nrh_tnw_p110_143-CFE-only.txt
NVRAM_FILENAME:=nvram.txt
BOOT_FILE:=cfez.bin.470x.6.37.14.62
MAX_FIRMWARE_SIZE:=0x3a0000
KERNEL_POSTFIX:=2wan
LANG_PACKS:=kr,en
KERNEL_PATH:=/home/$(shell whoami)/broadcom_sdk/bcm_6.37.14.62
KCONFIG_FILE_NAME:=.config_2wan
LINUX_PATH:=components/opensource/linux/linux-2.6.36.2wan
ROUTER_PATH:=src/router
