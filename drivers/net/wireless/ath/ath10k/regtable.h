/*
 * Copyright (c) 2005-2011 Atheros Communications Inc.
 * Copyright (c) 2011-2013 Qualcomm Atheros, Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __REGTABLE_H__
#define __REGTABLE_H__

#include <linux/types.h>

/* TARGET REGISTERS TABLE */
struct targetdef {
	u32 D_RTC_SOC_BASE_ADDRESS;
	u32 D_RTC_WMAC_BASE_ADDRESS;
	u32 D_SYSTEM_SLEEP_OFFSET;
	u32 D_WLAN_SYSTEM_SLEEP_OFFSET;
	u32 D_WLAN_SYSTEM_SLEEP_DISABLE_LSB;
	u32 D_WLAN_SYSTEM_SLEEP_DISABLE_MASK;
	u32 D_CLOCK_CONTROL_OFFSET;
	u32 D_CLOCK_CONTROL_SI0_CLK_MASK;
	u32 D_RESET_CONTROL_OFFSET;
	u32 D_RESET_CONTROL_MBOX_RST_MASK;
	u32 D_RESET_CONTROL_SI0_RST_MASK;
	u32 D_WLAN_RESET_CONTROL_OFFSET;
	u32 D_WLAN_RESET_CONTROL_COLD_RST_MASK;
	u32 D_WLAN_RESET_CONTROL_WARM_RST_MASK;
	u32 D_GPIO_BASE_ADDRESS;
	u32 D_GPIO_PIN0_OFFSET;
	u32 D_GPIO_PIN1_OFFSET;
	u32 D_GPIO_PIN0_CONFIG_MASK;
	u32 D_GPIO_PIN1_CONFIG_MASK;
	u32 D_SI_CONFIG_BIDIR_OD_DATA_LSB;
	u32 D_SI_CONFIG_BIDIR_OD_DATA_MASK;
	u32 D_SI_CONFIG_I2C_LSB;
	u32 D_SI_CONFIG_I2C_MASK;
	u32 D_SI_CONFIG_POS_SAMPLE_LSB;
	u32 D_SI_CONFIG_POS_SAMPLE_MASK;
	u32 D_SI_CONFIG_INACTIVE_CLK_LSB;
	u32 D_SI_CONFIG_INACTIVE_CLK_MASK;
	u32 D_SI_CONFIG_INACTIVE_DATA_LSB;
	u32 D_SI_CONFIG_INACTIVE_DATA_MASK;
	u32 D_SI_CONFIG_DIVIDER_LSB;
	u32 D_SI_CONFIG_DIVIDER_MASK;
	u32 D_SI_BASE_ADDRESS;
	u32 D_SI_CONFIG_OFFSET;
	u32 D_SI_TX_DATA0_OFFSET;
	u32 D_SI_TX_DATA1_OFFSET;
	u32 D_SI_RX_DATA0_OFFSET;
	u32 D_SI_RX_DATA1_OFFSET;
	u32 D_SI_CS_OFFSET;
	u32 D_SI_CS_DONE_ERR_MASK;
	u32 D_SI_CS_DONE_INT_MASK;
	u32 D_SI_CS_START_LSB;
	u32 D_SI_CS_START_MASK;
	u32 D_SI_CS_RX_CNT_LSB;
	u32 D_SI_CS_RX_CNT_MASK;
	u32 D_SI_CS_TX_CNT_LSB;
	u32 D_SI_CS_TX_CNT_MASK;
	u32 D_BOARD_DATA_SZ;
	u32 D_BOARD_EXT_DATA_SZ;
	u32 D_MBOX_BASE_ADDRESS;
	u32 D_LOCAL_SCRATCH_OFFSET;
	u32 D_CPU_CLOCK_OFFSET;
	u32 D_LPO_CAL_OFFSET;
	u32 D_GPIO_PIN10_OFFSET;
	u32 D_GPIO_PIN11_OFFSET;
	u32 D_GPIO_PIN12_OFFSET;
	u32 D_GPIO_PIN13_OFFSET;
	u32 D_CLOCK_GPIO_OFFSET;
	u32 D_CPU_CLOCK_STANDARD_LSB;
	u32 D_CPU_CLOCK_STANDARD_MASK;
	u32 D_LPO_CAL_ENABLE_LSB;
	u32 D_LPO_CAL_ENABLE_MASK;
	u32 D_CLOCK_GPIO_BT_CLK_OUT_EN_LSB;
	u32 D_CLOCK_GPIO_BT_CLK_OUT_EN_MASK;
	u32 D_ANALOG_INTF_BASE_ADDRESS;
	u32 D_WLAN_MAC_BASE_ADDRESS;
	u32 D_CE0_BASE_ADDRESS;
	u32 D_CE1_BASE_ADDRESS;
	u32 D_FW_INDICATOR_ADDRESS;
	u32 D_DRAM_BASE_ADDRESS;
	u32 D_SOC_CORE_BASE_ADDRESS;
	u32 D_CORE_CTRL_ADDRESS;
	u32 D_CE_COUNT;
	u32 D_MSI_NUM_REQUEST;
	u32 D_MSI_ASSIGN_FW;
	u32 D_MSI_ASSIGN_CE_INITIAL;
	u32 D_MSI_ASSIGN_CE_MAX;
	u32 D_PCIE_INTR_ENABLE_ADDRESS;
	u32 D_PCIE_INTR_CLR_ADDRESS;
	u32 D_PCIE_INTR_FIRMWARE_MASK;
	u32 D_PCIE_INTR_CE_MASK_ALL;
	u32 D_CORE_CTRL_CPU_INTR_MASK;
	u32 D_CE_WRAPPER_BASE_ADDRESS;
};


/* target */
/* helper macros to fetch registers via ar->targdef */
#define CE0_BASE_ADDRESS_T(ar) ((ar)->targetdef->D_CE0_BASE_ADDRESS)
#define CE1_BASE_ADDRESS_T(ar) ((ar)->targetdef->D_CE1_BASE_ADDRESS)
#define FW_INDICATOR_ADDRESS_T(ar) ((ar)->targetdef->D_FW_INDICATOR_ADDRESS)
#define DRAM_BASE_ADDRESS_T(ar) ((ar)->targetdef->D_DRAM_BASE_ADDRESS)
#define SOC_CORE_BASE_ADDRESS_T(ar) ((ar)->targetdef->D_SOC_CORE_BASE_ADDRESS)
#define CORE_CTRL_ADDRESS_T(ar) ((ar)->targetdef->D_CORE_CTRL_ADDRESS)
#define CE_COUNT_T(ar) ((ar)->targetdef->D_CE_COUNT)
#define PCIE_INTR_ENABLE_ADDRESS_T(ar) \
				((ar)->targetdef->D_PCIE_INTR_ENABLE_ADDRESS)
#define PCIE_INTR_CLR_ADDRESS_T(ar) ((ar)->targetdef->D_PCIE_INTR_CLR_ADDRESS)
#define PCIE_INTR_FIRMWARE_MASK_T(ar) \
				((ar)->targetdef->D_PCIE_INTR_FIRMWARE_MASK)
#define PCIE_INTR_CE_MASK_ALL_T(ar) ((ar)->targetdef->D_PCIE_INTR_CE_MASK_ALL)
#define CORE_CTRL_CPU_INTR_MASK_T(ar) \
				((ar)->targetdef->D_CORE_CTRL_CPU_INTR_MASK)
#define CE_WRAPPER_BASE_ADDRESS_T(ar) \
				((ar)->targetdef->D_CE_WRAPPER_BASE_ADDRESS)
#define MSI_NUM_REQUEST_T(ar) ((ar)->targetdef->D_MSI_NUM_REQUEST)
#define MSI_ASSIGN_FW_T(ar) ((ar)->targetdef->D_MSI_ASSIGN_FW)
#define MSI_ASSIGN_CE_INITIAL_T(ar) ((ar)->targetdef->D_MSI_ASSIGN_CE_INITIAL)
#define MSI_ASSIGN_CE_MAX_T(ar) ((ar)->targetdef->D_MSI_ASSIGN_CE_MAX)

/* HOST REGISTERS TABLE */
struct hostdef {
	u32 D_INT_STATUS_ENABLE_ERROR_LSB;
	u32 D_INT_STATUS_ENABLE_ERROR_MASK;
	u32 D_INT_STATUS_ENABLE_CPU_LSB;
	u32 D_INT_STATUS_ENABLE_CPU_MASK;
	u32 D_INT_STATUS_ENABLE_COUNTER_LSB;
	u32 D_INT_STATUS_ENABLE_COUNTER_MASK;
	u32 D_INT_STATUS_ENABLE_MBOX_DATA_LSB;
	u32 D_INT_STATUS_ENABLE_MBOX_DATA_MASK;
	u32 D_ERROR_STATUS_ENABLE_RX_UNDERFLOW_LSB;
	u32 D_ERROR_STATUS_ENABLE_RX_UNDERFLOW_MASK;
	u32 D_ERROR_STATUS_ENABLE_TX_OVERFLOW_LSB;
	u32 D_ERROR_STATUS_ENABLE_TX_OVERFLOW_MASK;
	u32 D_COUNTER_INT_STATUS_ENABLE_BIT_LSB;
	u32 D_COUNTER_INT_STATUS_ENABLE_BIT_MASK;
	u32 D_INT_STATUS_ENABLE_ADDRESS;
	u32 D_CPU_INT_STATUS_ENABLE_BIT_LSB;
	u32 D_CPU_INT_STATUS_ENABLE_BIT_MASK;
	u32 D_HOST_INT_STATUS_ADDRESS;
	u32 D_CPU_INT_STATUS_ADDRESS;
	u32 D_ERROR_INT_STATUS_ADDRESS;
	u32 D_ERROR_INT_STATUS_WAKEUP_MASK;
	u32 D_ERROR_INT_STATUS_WAKEUP_LSB;
	u32 D_ERROR_INT_STATUS_RX_UNDERFLOW_MASK;
	u32 D_ERROR_INT_STATUS_RX_UNDERFLOW_LSB;
	u32 D_ERROR_INT_STATUS_TX_OVERFLOW_MASK;
	u32 D_ERROR_INT_STATUS_TX_OVERFLOW_LSB;
	u32 D_COUNT_DEC_ADDRESS;
	u32 D_HOST_INT_STATUS_CPU_MASK;
	u32 D_HOST_INT_STATUS_CPU_LSB;
	u32 D_HOST_INT_STATUS_ERROR_MASK;
	u32 D_HOST_INT_STATUS_ERROR_LSB;
	u32 D_HOST_INT_STATUS_COUNTER_MASK;
	u32 D_HOST_INT_STATUS_COUNTER_LSB;
	u32 D_RX_LOOKAHEAD_VALID_ADDRESS;
	u32 D_WINDOW_DATA_ADDRESS;
	u32 D_WINDOW_READ_ADDR_ADDRESS;
	u32 D_WINDOW_WRITE_ADDR_ADDRESS;
	u32 D_SOC_GLOBAL_RESET_ADDRESS;
	u32 D_RTC_STATE_ADDRESS;
	u32 D_RTC_STATE_COLD_RESET_MASK;
	u32 D_PCIE_LOCAL_BASE_ADDRESS;
	u32 D_PCIE_SOC_WAKE_RESET;
	u32 D_PCIE_SOC_WAKE_ADDRESS;
	u32 D_PCIE_SOC_WAKE_V_MASK;
	u32 D_RTC_STATE_V_MASK;
	u32 D_RTC_STATE_V_LSB;
	u32 D_FW_IND_EVENT_PENDING;
	u32 D_FW_IND_INITIALIZED;
	u32 D_RTC_STATE_V_ON;
};

/* host */
#define SOC_GLOBAL_RESET_ADDRESS_T(ar) \
				((ar)->hostdef->D_SOC_GLOBAL_RESET_ADDRESS)
#define RTC_STATE_ADDRESS_T(ar)	((ar)->hostdef->D_RTC_STATE_ADDRESS)
#define RTC_STATE_COLD_RESET_MASK_T(ar) \
				((ar)->hostdef->D_RTC_STATE_COLD_RESET_MASK)
#define PCIE_LOCAL_BASE_ADDRESS_T(ar) \
				((ar)->hostdef->D_PCIE_LOCAL_BASE_ADDRESS)
#define PCIE_SOC_WAKE_RESET_T(ar) ((ar)->hostdef->D_PCIE_SOC_WAKE_RESET)
#define PCIE_SOC_WAKE_ADDRESS_T(ar) ((ar)->hostdef->D_PCIE_SOC_WAKE_ADDRESS)
#define PCIE_SOC_WAKE_V_MASK_T(ar) ((ar)->hostdef->D_PCIE_SOC_WAKE_V_MASK)
#define RTC_STATE_V_MASK_T(ar) ((ar)->hostdef->D_RTC_STATE_V_MASK)
#define RTC_STATE_V_LSB_T(ar) ((ar)->hostdef->D_RTC_STATE_V_LSB)
#define FW_IND_EVENT_PENDING_T(ar) ((ar)->hostdef->D_FW_IND_EVENT_PENDING)
#define FW_IND_INITIALIZED_T(ar) ((ar)->hostdef->D_FW_IND_INITIALIZED)
#define RTC_STATE_V_ON_T(ar) ((ar)->hostdef->D_RTC_STATE_V_ON)
#define RTC_STATE_V_GET(ar, x) \
		(((x) & RTC_STATE_V_MASK_T(ar)) >> RTC_STATE_V_LSB_T(ar))

#endif /* __REGTABLE_H__ */
