obj-$(CONFIG_ATH10K) += ath10k_core.o
ath10k_core-y += mac.o \
		 debug.o \
		 core.o \
		 htc.o \
		 htt.o \
		 htt_rx.o \
		 htt_tx.o \
		 txrx.o \
		 wmi.o \
		 bmi.o

ath10k_core-$(CONFIG_ATH10K_TRACING) += trace.o

obj-$(CONFIG_ATH10K_PCI) += ath10k_pci.o
ath10k_pci-y += pci.o \
		ce.o \
		regtable.o \
		ar9888_regtable.o

# for tracing framework to find trace.h
CFLAGS_trace.o := -I$(src)
