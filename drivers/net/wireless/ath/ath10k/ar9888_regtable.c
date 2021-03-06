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

#include "ar9888_regtable.h"
#include "ar9888def.h"

/* AR9888_V1 hostdef, targetdef structures */
static struct targetdef ar9888_target_def = {
	.D_RTC_SOC_BASE_ADDRESS =		RTC_SOC_BASE_ADDRESS,
	.D_RTC_WMAC_BASE_ADDRESS =		RTC_WMAC_BASE_ADDRESS,
	.D_SYSTEM_SLEEP_OFFSET =		WLAN_SYSTEM_SLEEP_OFFSET,
	.D_WLAN_SYSTEM_SLEEP_OFFSET =		WLAN_SYSTEM_SLEEP_OFFSET,
	.D_WLAN_SYSTEM_SLEEP_DISABLE_LSB =	WLAN_SYSTEM_SLEEP_DISABLE_LSB,
	.D_WLAN_SYSTEM_SLEEP_DISABLE_MASK =	WLAN_SYSTEM_SLEEP_DISABLE_MASK,
	.D_CLOCK_CONTROL_OFFSET =		CLOCK_CONTROL_OFFSET,
	.D_CLOCK_CONTROL_SI0_CLK_MASK =		CLOCK_CONTROL_SI0_CLK_MASK,
	.D_RESET_CONTROL_OFFSET =		SOC_RESET_CONTROL_OFFSET,
	.D_RESET_CONTROL_MBOX_RST_MASK =	RESET_CONTROL_MBOX_RST_MASK,
	.D_RESET_CONTROL_SI0_RST_MASK =		RESET_CONTROL_SI0_RST_MASK,
	.D_WLAN_RESET_CONTROL_OFFSET =		WLAN_RESET_CONTROL_OFFSET,
	.D_WLAN_RESET_CONTROL_COLD_RST_MASK =	WLAN_RESET_CONTROL_COLD_RST_MASK,
	.D_WLAN_RESET_CONTROL_WARM_RST_MASK =	WLAN_RESET_CONTROL_WARM_RST_MASK,
	.D_GPIO_BASE_ADDRESS =			GPIO_BASE_ADDRESS,
	.D_GPIO_PIN0_OFFSET =			GPIO_PIN0_OFFSET,
	.D_GPIO_PIN1_OFFSET =			GPIO_PIN1_OFFSET,
	.D_GPIO_PIN0_CONFIG_MASK =		GPIO_PIN0_CONFIG_MASK,
	.D_GPIO_PIN1_CONFIG_MASK =		GPIO_PIN1_CONFIG_MASK,
	.D_SI_CONFIG_BIDIR_OD_DATA_LSB =	SI_CONFIG_BIDIR_OD_DATA_LSB,
	.D_SI_CONFIG_BIDIR_OD_DATA_MASK =	SI_CONFIG_BIDIR_OD_DATA_MASK,
	.D_SI_CONFIG_I2C_LSB =			SI_CONFIG_I2C_LSB,
	.D_SI_CONFIG_I2C_MASK =			SI_CONFIG_I2C_MASK,
	.D_SI_CONFIG_POS_SAMPLE_LSB =		SI_CONFIG_POS_SAMPLE_LSB,
	.D_SI_CONFIG_POS_SAMPLE_MASK =		SI_CONFIG_POS_SAMPLE_MASK,
	.D_SI_CONFIG_INACTIVE_CLK_LSB =		SI_CONFIG_INACTIVE_CLK_LSB,
	.D_SI_CONFIG_INACTIVE_CLK_MASK =	SI_CONFIG_INACTIVE_CLK_MASK,
	.D_SI_CONFIG_INACTIVE_DATA_LSB =	SI_CONFIG_INACTIVE_DATA_LSB,
	.D_SI_CONFIG_INACTIVE_DATA_MASK =	SI_CONFIG_INACTIVE_DATA_MASK,
	.D_SI_CONFIG_DIVIDER_LSB =		SI_CONFIG_DIVIDER_LSB,
	.D_SI_CONFIG_DIVIDER_MASK =		SI_CONFIG_DIVIDER_MASK,
	.D_SI_BASE_ADDRESS =			SI_BASE_ADDRESS,
	.D_SI_CONFIG_OFFSET =			SI_CONFIG_OFFSET,
	.D_SI_TX_DATA0_OFFSET =			SI_TX_DATA0_OFFSET,
	.D_SI_TX_DATA1_OFFSET =			SI_TX_DATA1_OFFSET,
	.D_SI_RX_DATA0_OFFSET =			SI_RX_DATA0_OFFSET,
	.D_SI_RX_DATA1_OFFSET =			SI_RX_DATA1_OFFSET,
	.D_SI_CS_OFFSET =			SI_CS_OFFSET,
	.D_SI_CS_DONE_ERR_MASK =		SI_CS_DONE_ERR_MASK,
	.D_SI_CS_DONE_INT_MASK =		SI_CS_DONE_INT_MASK,
	.D_SI_CS_START_LSB =			SI_CS_START_LSB,
	.D_SI_CS_START_MASK =			SI_CS_START_MASK,
	.D_SI_CS_RX_CNT_LSB =			SI_CS_RX_CNT_LSB,
	.D_SI_CS_RX_CNT_MASK =			SI_CS_RX_CNT_MASK,
	.D_SI_CS_TX_CNT_LSB =			SI_CS_TX_CNT_LSB,
	.D_SI_CS_TX_CNT_MASK =			SI_CS_TX_CNT_MASK,
	.D_BOARD_DATA_SZ =			MY_TARGET_BOARD_DATA_SZ,
	.D_BOARD_EXT_DATA_SZ =			MY_TARGET_BOARD_EXT_DATA_SZ,
	.D_MBOX_BASE_ADDRESS =			MBOX_BASE_ADDRESS,
	.D_LOCAL_SCRATCH_OFFSET =		LOCAL_SCRATCH_OFFSET,
	.D_CPU_CLOCK_OFFSET =			CPU_CLOCK_OFFSET,
	.D_LPO_CAL_OFFSET =			LPO_CAL_OFFSET,
	.D_GPIO_PIN10_OFFSET =			GPIO_PIN10_OFFSET,
	.D_GPIO_PIN11_OFFSET =			GPIO_PIN11_OFFSET,
	.D_GPIO_PIN12_OFFSET =			GPIO_PIN12_OFFSET,
	.D_GPIO_PIN13_OFFSET =			GPIO_PIN13_OFFSET,
	.D_CLOCK_GPIO_OFFSET =			CLOCK_GPIO_OFFSET,
	.D_CPU_CLOCK_STANDARD_LSB =		CPU_CLOCK_STANDARD_LSB,
	.D_CPU_CLOCK_STANDARD_MASK =		CPU_CLOCK_STANDARD_MASK,
	.D_LPO_CAL_ENABLE_LSB =			LPO_CAL_ENABLE_LSB,
	.D_LPO_CAL_ENABLE_MASK =		LPO_CAL_ENABLE_MASK,
	.D_CLOCK_GPIO_BT_CLK_OUT_EN_LSB =	CLOCK_GPIO_BT_CLK_OUT_EN_LSB,
	.D_CLOCK_GPIO_BT_CLK_OUT_EN_MASK =	CLOCK_GPIO_BT_CLK_OUT_EN_MASK,
	.D_ANALOG_INTF_BASE_ADDRESS =		ANALOG_INTF_BASE_ADDRESS,
	.D_WLAN_MAC_BASE_ADDRESS =		WLAN_MAC_BASE_ADDRESS,
	.D_CE0_BASE_ADDRESS =			CE0_BASE_ADDRESS,
	.D_CE1_BASE_ADDRESS =			CE1_BASE_ADDRESS,
	.D_FW_INDICATOR_ADDRESS =		FW_INDICATOR_ADDRESS,
	.D_DRAM_BASE_ADDRESS =			DRAM_BASE_ADDRESS,
	.D_SOC_CORE_BASE_ADDRESS =		SOC_CORE_BASE_ADDRESS,
	.D_CORE_CTRL_ADDRESS =			CORE_CTRL_ADDRESS,
	.D_CE_COUNT =				CE_COUNT,
	.D_MSI_NUM_REQUEST =			MSI_NUM_REQUEST,
	.D_MSI_ASSIGN_FW =			MSI_ASSIGN_FW,
	.D_MSI_ASSIGN_CE_INITIAL =		MSI_ASSIGN_CE_INITIAL,
	.D_MSI_ASSIGN_CE_MAX =			MSI_ASSIGN_CE_MAX,
	.D_PCIE_INTR_ENABLE_ADDRESS =		PCIE_INTR_ENABLE_ADDRESS,
	.D_PCIE_INTR_CLR_ADDRESS =		PCIE_INTR_CLR_ADDRESS,
	.D_PCIE_INTR_FIRMWARE_MASK =		PCIE_INTR_FIRMWARE_MASK,
	.D_PCIE_INTR_CE_MASK_ALL =		PCIE_INTR_CE_MASK_ALL,
	.D_CORE_CTRL_CPU_INTR_MASK =		CORE_CTRL_CPU_INTR_MASK,
	.D_CE_WRAPPER_BASE_ADDRESS =		CE_WRAPPER_BASE_ADDRESS,
};

struct targetdef *ar9888_get_target_tbl(void)
{
	return &ar9888_target_def;
}

static struct hostdef ar9888_host_def = {
	.D_INT_STATUS_ENABLE_ERROR_LSB =		INT_STATUS_ENABLE_ERROR_LSB,
	.D_INT_STATUS_ENABLE_ERROR_MASK =		INT_STATUS_ENABLE_ERROR_MASK,
	.D_INT_STATUS_ENABLE_CPU_LSB =			INT_STATUS_ENABLE_CPU_LSB,
	.D_INT_STATUS_ENABLE_CPU_MASK =			INT_STATUS_ENABLE_CPU_MASK,
	.D_INT_STATUS_ENABLE_COUNTER_LSB =		INT_STATUS_ENABLE_COUNTER_LSB,
	.D_INT_STATUS_ENABLE_COUNTER_MASK =		INT_STATUS_ENABLE_COUNTER_MASK,
	.D_INT_STATUS_ENABLE_MBOX_DATA_LSB =		INT_STATUS_ENABLE_MBOX_DATA_LSB,
	.D_INT_STATUS_ENABLE_MBOX_DATA_MASK =		INT_STATUS_ENABLE_MBOX_DATA_MASK,
	.D_ERROR_STATUS_ENABLE_RX_UNDERFLOW_LSB =	ERROR_STATUS_ENABLE_RX_UNDERFLOW_LSB,
	.D_ERROR_STATUS_ENABLE_RX_UNDERFLOW_MASK =	ERROR_STATUS_ENABLE_RX_UNDERFLOW_MASK,
	.D_ERROR_STATUS_ENABLE_TX_OVERFLOW_LSB =	ERROR_STATUS_ENABLE_TX_OVERFLOW_LSB,
	.D_ERROR_STATUS_ENABLE_TX_OVERFLOW_MASK =	ERROR_STATUS_ENABLE_TX_OVERFLOW_MASK,
	.D_COUNTER_INT_STATUS_ENABLE_BIT_LSB =		COUNTER_INT_STATUS_ENABLE_BIT_LSB,
	.D_COUNTER_INT_STATUS_ENABLE_BIT_MASK =		COUNTER_INT_STATUS_ENABLE_BIT_MASK,
	.D_INT_STATUS_ENABLE_ADDRESS =			INT_STATUS_ENABLE_ADDRESS,
	.D_CPU_INT_STATUS_ENABLE_BIT_LSB =		CPU_INT_STATUS_ENABLE_BIT_LSB,
	.D_CPU_INT_STATUS_ENABLE_BIT_MASK =		CPU_INT_STATUS_ENABLE_BIT_MASK,
	.D_HOST_INT_STATUS_ADDRESS =			HOST_INT_STATUS_ADDRESS,
	.D_CPU_INT_STATUS_ADDRESS =			CPU_INT_STATUS_ADDRESS,
	.D_ERROR_INT_STATUS_ADDRESS =			ERROR_INT_STATUS_ADDRESS,
	.D_ERROR_INT_STATUS_WAKEUP_MASK =		ERROR_INT_STATUS_WAKEUP_MASK,
	.D_ERROR_INT_STATUS_WAKEUP_LSB =		ERROR_INT_STATUS_WAKEUP_LSB,
	.D_ERROR_INT_STATUS_RX_UNDERFLOW_MASK =		ERROR_INT_STATUS_RX_UNDERFLOW_MASK,
	.D_ERROR_INT_STATUS_RX_UNDERFLOW_LSB =		ERROR_INT_STATUS_RX_UNDERFLOW_LSB,
	.D_ERROR_INT_STATUS_TX_OVERFLOW_MASK =		ERROR_INT_STATUS_TX_OVERFLOW_MASK,
	.D_ERROR_INT_STATUS_TX_OVERFLOW_LSB =		ERROR_INT_STATUS_TX_OVERFLOW_LSB,
	.D_COUNT_DEC_ADDRESS =				COUNT_DEC_ADDRESS,
	.D_HOST_INT_STATUS_CPU_MASK =			HOST_INT_STATUS_CPU_MASK,
	.D_HOST_INT_STATUS_CPU_LSB =			HOST_INT_STATUS_CPU_LSB,
	.D_HOST_INT_STATUS_ERROR_MASK =			HOST_INT_STATUS_ERROR_MASK,
	.D_HOST_INT_STATUS_ERROR_LSB =			HOST_INT_STATUS_ERROR_LSB,
	.D_HOST_INT_STATUS_COUNTER_MASK =		HOST_INT_STATUS_COUNTER_MASK,
	.D_HOST_INT_STATUS_COUNTER_LSB =		HOST_INT_STATUS_COUNTER_LSB,
	.D_RX_LOOKAHEAD_VALID_ADDRESS =			RX_LOOKAHEAD_VALID_ADDRESS,
	.D_WINDOW_DATA_ADDRESS =			WINDOW_DATA_ADDRESS,
	.D_WINDOW_READ_ADDR_ADDRESS =			WINDOW_READ_ADDR_ADDRESS,
	.D_WINDOW_WRITE_ADDR_ADDRESS =			WINDOW_WRITE_ADDR_ADDRESS,
	.D_SOC_GLOBAL_RESET_ADDRESS =			SOC_GLOBAL_RESET_ADDRESS,
	.D_RTC_STATE_ADDRESS =				RTC_STATE_ADDRESS,
	.D_RTC_STATE_COLD_RESET_MASK =			RTC_STATE_COLD_RESET_MASK,
	.D_PCIE_LOCAL_BASE_ADDRESS =			PCIE_LOCAL_BASE_ADDRESS,
	.D_PCIE_SOC_WAKE_RESET =			PCIE_SOC_WAKE_RESET,
	.D_PCIE_SOC_WAKE_ADDRESS =			PCIE_SOC_WAKE_ADDRESS,
	.D_PCIE_SOC_WAKE_V_MASK =			PCIE_SOC_WAKE_V_MASK,
	.D_RTC_STATE_V_MASK =				RTC_STATE_V_MASK,
	.D_RTC_STATE_V_LSB =				RTC_STATE_V_LSB,
	.D_FW_IND_EVENT_PENDING =			FW_IND_EVENT_PENDING,
	.D_FW_IND_INITIALIZED =				FW_IND_INITIALIZED,
	.D_RTC_STATE_V_ON =				RTC_STATE_V_ON,
};

struct hostdef *ar9888_get_host_tbl(void)
{
	return &ar9888_host_def;
}
