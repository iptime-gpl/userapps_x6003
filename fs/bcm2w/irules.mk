
APPS_DIR:= ../..

include $(USERAPPS_ROOT)/config
-include $(USERAPPS_ROOT)/reg_config
include $(USERAPPS_ROOT)/rootfs/clone_info.mk

STRIP:=arm-brcm-linux-uclibcgnueabi-strip

ifeq ($(USE_CUSTOM_VERSION),y)
include $(USERAPPS_ROOT)/rootfs/clones/$(TARGET)/version.mk
else
include $(USERAPPS_ROOT)/rootfs/version.mk
endif
include $(USERAPPS_ROOT)/lang_profile
include $(USERAPPS_ROOT)/rootfs/kernel_info.mk

ROOTFS_IMG=rootfs.lzma
PLUGIN_DIR:=./plugin
CHIPSET_APP_INSTALL_DIR:=bcmapp
CLIB_DIR:=fs/bcm2w/clib
IPTABLES_BIN_PATH:=$(USERAPPS_ROOT)/$(IPTABLES_DIR)/src/install/sbin
IPTABLES_BINS:=iptables xtables-multi
IPTABLES_LIB_PATH:=$(USERAPPS_ROOT)/$(IPTABLES_DIR)/src/install/lib/
IPTABLES_LIBS:=

CFE_SRC_IMG:=./prebuilt/boot/$(BOOT_FILE) 
CFE_NV_IMG:=./clones/$(TARGET)/$(TARGET)_xboot.bin
NVRAM_FILE:=./clones/$(TARGET)/$(NVRAM_FILENAME)
TRX_NAME:=linux.trx



$(TARGET): target.fs image

# for reserving upgrade memory

ROOT_DIR:= root

post_targetfs:
	@echo -e "\t--->Post processing..."
ifeq ($(USE_EMBEDDED_HELP),y)
	@rm -rf $(ROOT_DIR)/home/httpd/help/sysconf_swupgrade_help.*
	@rm -rf $(ROOT_DIR)/home/httpd/help/netconf_internet_help.html.time54g
endif
	@mkdir -p $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm
	@cp -ra $(KMODULE_DIR)/* $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/
	@rm -rf $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/net/usb/ipheth.ko 

ifeq ($(USE_WIRELESS_CGI),y)
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/net/wl/wl.ko
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/net/dpsta/dpsta.ko
endif
	@rm -rf $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/lib/libcrc32c.ko
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/net/et/et.ko
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/net/emf/emf.ko
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/net/igs/igs.ko
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/net/ctf/ctf.ko
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/connector/cn.ko
ifeq ($(USE_ROUTER_NAS),y)
	@rm -rf $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/usb/mon/usbmon.ko
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/usb/storage/usb-storage.ko
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/usb/host/ehci-hcd.ko
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/usb/host/ohci-hcd.ko
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/usb/host/xhci-hcd.ko
	@$(STRIP) -d $(ROOT_DIR)/lib/modules/2.6.36.4brcmarm/kernel/drivers/scsi/scsi_wait_scan.ko
endif

ifeq ($(USE_BCM5354),y)
	@cp -ra $(APPS_DIR)/bcmapp/install/usr/lib/* $(ROOT_DIR)/usr/lib
	@cp -ra $(APPS_DIR)/bcmapp/install/sbin/* $(ROOT_DIR)/sbin
	@cp -ra $(APPS_DIR)/bcmapp/install/bin/* $(ROOT_DIR)/bin
	@cp -ra $(APPS_DIR)/rc/install/sbin/* $(ROOT_DIR)/sbin
endif

ifeq ($(USE_ROUTER_NAS),y)
ifeq ($(USE_ROUTER_NAS_DEBUG),y)
	@ln -s /tmp/hotplug $(ROOT_DIR)/sbin/hotplug
endif
	@ln -s /mnt/HDD1/libnas2.so $(ROOT_DIR)/lib/libnas2.so
	@ln -s /sbin/lighttpd $(ROOT_DIR)/sbin/lighttpd_ipdisk


	@cp -ra $(APPS_DIR)/nasapp/lib/libintl.so.8.1.2 $(ROOT_DIR)/lib
	@ln -s /lib/libintl.so.8.1.2 $(ROOT_DIR)/lib/libintl.so
	@ln -s /lib/libintl.so.8.1.2 $(ROOT_DIR)/lib/libintl.so.8


	@cp -ra $(APPS_DIR)/nasapp/lib/libiconv.so.2.2.0 $(ROOT_DIR)/lib
	@ln -s /lib/libiconv.so.2.2.0 $(ROOT_DIR)/lib/libiconv.so
	@ln -s /lib/libiconv.so.2.2.0 $(ROOT_DIR)/lib/libiconv.so.2

	@$(STRIP) $(ROOT_DIR)/sbin/xtables-multi

	@cp -ra $(APPS_DIR)/nasapp/lib/libpcre.so.0.0.1 $(ROOT_DIR)/lib
	@ln -s /lib/libpcre.so.0.0.1 $(ROOT_DIR)/lib/libpcre.so
	@ln -s /lib/libpcre.so.0.0.1 $(ROOT_DIR)/lib/libpcre.so.0

	@ln -s /mnt/HDD1/libbigballofmud.so $(ROOT_DIR)/lib/libbigballofmud.so.0

	@$(STRIP) $(ROOT_DIR)/sbin/smbd
	@$(STRIP) $(ROOT_DIR)/sbin/nmbd
	@$(STRIP) $(ROOT_DIR)/sbin/smbpasswd

ifeq ($(USE_EXFAT),y)
	@cp -ra $(APPS_DIR)/nasapp/source/fuse-2.9.2/lib/.libs/libfuse.so.2.9.2 $(ROOT_DIR)/lib
	@$(STRIP) $(ROOT_DIR)/lib/libfuse.so.2.9.2
	@ln -s /lib/libfuse.so.2.9.2 $(ROOT_DIR)/lib/libfuse.so
	@ln -s /lib/libfuse.so.2.9.2 $(ROOT_DIR)/lib/libfuse.so.2
endif

endif

	@cp -ra $(APPS_DIR)/$(IPTABLES_DIR)/src/install/lib/libipq.so.0.0.0 $(ROOT_DIR)/lib
	@ln -s /lib/libipq.so.0.0.0 $(ROOT_DIR)/lib/libipq.so
	@ln -s /lib/libipq.so.0.0.0 $(ROOT_DIR)/lib/libipq.so.0

	@cp -ra $(APPS_DIR)/$(IPTABLES_DIR)/src/install/lib/libip4tc.so.0.0.0 $(ROOT_DIR)/lib
	@ln -s /lib/libip4tc.so.0.0.0 $(ROOT_DIR)/lib/libip4tc.so
	@ln -s /lib/libip4tc.so.0.0.0 $(ROOT_DIR)/lib/libip4tc.so.0

	@cp -ra $(APPS_DIR)/$(IPTABLES_DIR)/src/install/lib/libxtables.so.7.0.0 $(ROOT_DIR)/lib
	@ln -s /lib/libxtables.so.7.0.0 $(ROOT_DIR)/lib/libxtables.so
	@ln -s /lib/libxtables.so.7.0.0 $(ROOT_DIR)/lib/libxtables.so.7

	@/sbin/ldconfig -r $(ROOT_DIR)/default
	@./mksquashfs $(ROOT_DIR) target.lzma -noappend -all-root

include $(USERAPPS_ROOT)/mkscripts/target.mk

image:
	@./tools/trx -o $(TRX_NAME) $(KERNEL_NAME) target.lzma;
	@./tools/nvserial -i $(CFE_SRC_IMG) -o $(CFE_NV_IMG) $(NVRAM_FILE)
	@./makefirm -t trx -a $(PRODUCT_ID) -k $(TRX_NAME) -v $(MAJOR_VER)_$(MINOR_VER) -l $(LANGUAGE_POSTFIX) -b $(CFE_NV_IMG) -s 40000 -f $(MAX_FIRMWARE_SIZE)
	@./makefirm -t trx -a $(PRODUCT_ID) -k $(TRX_NAME) -v 0_00 -l $(LANGUAGE_POSTFIX) -b $(CFE_NV_IMG) -s 40000 -f $(MAX_FIRMWARE_SIZE)
	./firmware_size_check.sh $(PRODUCT_ID)_$(LANGUAGE_POSTFIX)_$(MAJOR_VER)_$(MINOR_VER).bin $(MAX_FIRMWARE_SIZE)
ifeq ($(USE_KT_SPEED_TEST),y)
ifeq ($(USE_MULTI_LANG),y)
	@mv $(PRODUCT_ID)_$(LANGUAGE_POSTFIX)_$(MAJOR_VER)_$(MINOR_VER).bin binary/$(PRODUCT_ID)_ktspeed_$(LANGUAGE_POSTFIX)_$(MAJOR_VER)_$(MINOR_VER).bin
else
	@mv $(PRODUCT_ID)_$(LANGUAGE_POSTFIX)_$(MAJOR_VER)_$(MINOR_VER).bin binary/$(PRODUCT_ID)_ktspeed_ml_$(MAJOR_VER)_$(MINOR_VER).bin
endif
else
ifeq ($(USE_MULTI_LANG),y)
	@mv $(PRODUCT_ID)_$(LANGUAGE_POSTFIX)_0_00.bin binary/$(PRODUCT_ID)_ml_$(MAJOR_VER)_$(MINOR_VER).bin.boot
	@mv $(PRODUCT_ID)_$(LANGUAGE_POSTFIX)_$(MAJOR_VER)_$(MINOR_VER).bin binary/$(PRODUCT_ID)_ml_$(MAJOR_VER)_$(MINOR_VER).bin
else
	@mv $(PRODUCT_ID)_$(LANGUAGE_POSTFIX)_0_00.bin binary/$(PRODUCT_ID)_$(LANGUAGE_POSTFIX)_$(MAJOR_VER)_$(MINOR_VER).bin.boot
	@mv $(PRODUCT_ID)_$(LANGUAGE_POSTFIX)_$(MAJOR_VER)_$(MINOR_VER).bin* binary
endif
endif
#	./makefirm -t trx -a $(PRODUCT_ID) -k mfglinux.trx -v 0_1 -l $(LANGUAGE_POSTFIX) -b cfez.bin.nv -s 20000 -f 0x3f0000
	@rm -rf target.cramfs
	@rm -rf target.lzma
#	rm -rf root

#	mv $(PRODUCT_ID)_$(LANGUAGE_POSTFIX)_$(MAJOR_VER)_$(MINOR_VER).bin binary
#	ftp 192.168.0.18
