/*
 * Copyright (c) 2020 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <asm/armv8/mmu.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>
#include <asm/arch/grf_rk3568.h>
#include <asm/arch/rk_atags.h>

DECLARE_GLOBAL_DATA_PTR;

#define PMUGRF_BASE		0xfdc20000
#define GRF_BASE		0xfdc60000
#define GRF_GPIO1B_DS_2		0x218
#define GRF_GPIO1B_DS_3		0x21c
#define GRF_GPIO1C_DS_0		0x220
#define GRF_GPIO1C_DS_1		0x224
#define GRF_GPIO1C_DS_2		0x228
#define GRF_GPIO1C_DS_3		0x22c
#define GRF_GPIO1D_DS_0		0x230
#define GRF_GPIO1D_DS_1		0x234
#define GRF_SOC_CON4		0x510
#define EDP_PHY_GRF_BASE	0xfdcb0000
#define EDP_PHY_GRF_CON0	(EDP_PHY_GRF_BASE + 0x00)
#define EDP_PHY_GRF_CON10	(EDP_PHY_GRF_BASE + 0x28)
#define PMU_BASE_ADDR		0xfdd90000
#define PMU_NOC_AUTO_CON0	(0x70)
#define PMU_NOC_AUTO_CON1	(0x74)
#define CRU_BASE		0xfdd20000
#define CRU_SOFTRST_CON26	0x468
#define SGRF_BASE		0xFDD18000
#define SGRF_SOC_CON4		0x10

#define CPU_GRF_BASE		0xfdc30000
#define GRF_CORE_PVTPLL_CON0	(0x10)

enum {
	/* PMU_GRF_GPIO0C_IOMUX_L */
	GPIO0C1_SHIFT		= 4,
	GPIO0C1_MASK		= GENMASK(6, 4),
	GPIO0C1_GPIO		= 0,
	GPIO0C1_PWM2_M0,
	GPIO0C1_NPU_AVS,
	GPIO0C1_UART0_TX,
	GPIO0C1_MCU_JTAGTDI,

	GPIO0C0_SHIFT		= 0,
	GPIO0C0_MASK		= GENMASK(2, 0),
	GPIO0C0_GPIO		= 0,
	GPIO0C0_PWM1_M0,
	GPIO0C0_GPU_AVS,
	GPIO0C0_UART0_RX,

	/* PMU_GRF_GPIO0D_IOMUX_L */
	GPIO0D1_SHIFT		= 4,
	GPIO0D1_MASK		= GENMASK(6, 4),
	GPIO0D1_GPIO		= 0,
	GPIO0D1_UART2_TXM0,

	GPIO0D0_SHIFT		= 0,
	GPIO0D0_MASK		= GENMASK(2, 0),
	GPIO0D0_GPIO		= 0,
	GPIO0D0_UART2_RXM0,

	/* PMU_GRF_SOC_CON0 */
	UART0_IO_SEL_SHIFT	= 8,
	UART0_IO_SEL_MASK	= GENMASK(9, 8),
	UART0_IO_SEL_M0		= 0,
	UART0_IO_SEL_M1,
	UART0_IO_SEL_M2,
};

enum {
	/* GRF_GPIO1A_IOMUX_L */
	GPIO1A1_SHIFT		= 4,
	GPIO1A1_MASK		= GENMASK(6, 4),
	GPIO1A1_GPIO		= 0,
	GPIO1A1_I2C3_SCLM0,
	GPIO1A1_UART3_TXM0,
	GPIO1A1_CAN1_TXM0,
	GPIO1A1_AUDIOPWM_ROUT,
	GPIO1A1_ACODEC_ADCCLK,
	GPIO1A1_AUDIOPWM_LOUT,

	GPIO1A0_SHIFT		= 0,
	GPIO1A0_MASK		= GENMASK(2, 0),
	GPIO1A0_GPIO		= 0,
	GPIO1A0_I2C3_SDAM0,
	GPIO1A0_UART3_RXM0,
	GPIO1A0_CAN1_RXM0,
	GPIO1A0_AUDIOPWM_LOUT,
	GPIO1A0_ACODEC_ADCDATA,
	GPIO1A0_AUDIOPWM_LOUTP,

	/* GRF_GPIO1A_IOMUX_H */
	GPIO1A6_SHIFT		= 8,
	GPIO1A6_MASK		= GENMASK(10, 8),
	GPIO1A6_GPIO		= 0,
	GPIO1A6_I2S1_LRCKRXM0,
	GPIO1A6_UART4_TXM0,
	GPIO1A6_PDM_CLK0M0,
	GPIO1A6_AUDIOPWM_ROUTP,

	GPIO1A4_SHIFT		= 0,
	GPIO1A4_MASK		= GENMASK(2, 0),
	GPIO1A4_GPIO		= 0,
	GPIO1A4_I2S1_SCLKRXM0,
	GPIO1A4_UART4_RXM0,
	GPIO1A4_PDM_CLK1M0,
	GPIO1A4_SPDIF_TXM0,

	/* GRF_GPIO1D_IOMUX_H */
	GPIO1D6_SHIFT		= 8,
	GPIO1D6_MASK		= GENMASK(10, 8),
	GPIO1D6_GPIO		= 0,
	GPIO1D6_SDMMC0_D1,
	GPIO1D6_UART2_RXM1,
	GPIO1D6_UART6_RXM1,
	GPIO1D6_PWM9_M1,

	GPIO1D5_SHIFT		= 4,
	GPIO1D5_MASK		= GENMASK(6, 4),
	GPIO1D5_GPIO		= 0,
	GPIO1D5_SDMMC0_D0,
	GPIO1D5_UART2_TXM1,
	GPIO1D5_UART6_TXM1,
	GPIO1D5_PWM8_M1,

	/* GRF_GPIO2A_IOMUX_L */
	GPIO2A3_SHIFT		= 12,
	GPIO2A3_MASK		= GENMASK(14, 12),
	GPIO2A3_GPIO		= 0,
	GPIO2A3_SDMMC1_D0,
	GPIO2A3_GMAC0_RXD2,
	GPIO2A3_UART6_RXM0,

	GPIO2A2_SHIFT		= 8,
	GPIO2A2_MASK		= GENMASK(10, 8),
	GPIO2A2_GPIO		= 0,
	GPIO2A2_SDMMC0_CLK,
	GPIO2A2_TEST_CLKOUT,
	GPIO2A2_UART5_TXM0,
	GPIO2A2_CAN0_RXM1,

	GPIO2A1_SHIFT		= 4,
	GPIO2A1_MASK		= GENMASK(6, 4),
	GPIO2A1_GPIO		= 0,
	GPIO2A1_SDMMC0_CMD,
	GPIO2A1_PWM10_M1,
	GPIO2A1_UART5_RXM0,
	GPIO2A1_CAN0_TXM1,

	/* GRF_GPIO2A_IOMUX_H */
	GPIO2A7_SHIFT		= 12,
	GPIO2A7_MASK		= GENMASK(14, 12),
	GPIO2A7_GPIO		= 0,
	GPIO2A7_SDMMC1_CMD,
	GPIO2A7_GMAC0_TXD3,
	GPIO2A7_UART9_RXM0,

	GPIO2A6_SHIFT		= 8,
	GPIO2A6_MASK		= GENMASK(10, 8),
	GPIO2A6_GPIO		= 0,
	GPIO2A6_SDMMC1_D3,
	GPIO2A6_GMAC0_TXD2,
	GPIO2A6_UART7_TXM0,

	GPIO2A5_SHIFT		= 4,
	GPIO2A5_MASK		= GENMASK(6, 4),
	GPIO2A5_GPIO		= 0,
	GPIO2A5_SDMMC1_D2,
	GPIO2A5_GMAC0_RXCLK,
	GPIO2A5_UART7_RXM0,

	GPIO2A4_SHIFT		= 0,
	GPIO2A4_MASK		= GENMASK(2, 0),
	GPIO2A4_GPIO		= 0,
	GPIO2A4_SDMMC1_D1,
	GPIO2A4_GMAC0_RXD3,
	GPIO2A4_UART6_TXM0,

	/* GRF_GPIO2B_IOMUX_L */
	GPIO2B3_SHIFT		= 12,
	GPIO2B3_MASK		= GENMASK(14, 12),
	GPIO2B3_GPIO		= 0,
	GPIO2B3_GMAC0_TXD0,
	GPIO2B3_UART1_RXM0,

	GPIO2B0_SHIFT		= 0,
	GPIO2B0_MASK		= GENMASK(2, 0),
	GPIO2B0_GPIO		= 0,
	GPIO2B0_SDMMC1_CLK,
	GPIO2B0_GMAC0_TXCLK,
	GPIO2B0_UART9_TXM0,

	/* GRF_GPIO2B_IOMUX_H */
	GPIO2B4_SHIFT		= 0,
	GPIO2B4_MASK		= GENMASK(2, 0),
	GPIO2B4_GPIO		= 0,
	GPIO2B4_GMAC0_TXD1,
	GPIO2B4_UART1_TXM0,

	/* GRF_GPIO2C_IOMUX_H */
	GPIO2C6_SHIFT		= 8,
	GPIO2C6_MASK		= GENMASK(10, 8),
	GPIO2C6_GPIO		= 0,
	GPIO2C6_CLK32K_OUT1,
	GPIO2C6_UART8_RXM0,
	GPIO2C6_SPI1_CS1M0,

	GPIO2C5_SHIFT		= 4,
	GPIO2C5_MASK		= GENMASK(6, 4),
	GPIO2C5_GPIO		= 0,
	GPIO2C5_I2S2_SDIM0,
	GPIO2C5_GMAC0_RXER,
	GPIO2C5_UART8_TXM0,
	GPIO2C5_SPI2_CS1M0,

	/* GRF_GPIO2D_IOMUX_H */
	GPIO2D7_SHIFT		= 12,
	GPIO2D7_MASK		= GENMASK(14, 12),
	GPIO2D7_GPIO		= 0,
	GPIO2D7_LCDC_D7,
	GPIO2D7_BT656_D7M0,
	GPIO2D7_SPI2_MISOM1,
	GPIO2D7_UART8_TXM1,
	GPIO2D7_I2S1_SDO0M2,

	/* GRF_GPIO3A_IOMUX_L */
	GPIO3A0_SHIFT		= 0,
	GPIO3A0_MASK		= GENMASK(2, 0),
	GPIO3A0_GPIO		= 0,
	GPIO3A0_LCDC_CLK,
	GPIO3A0_BT656_CLKM0,
	GPIO3A0_SPI2_CLKM1,
	GPIO3A0_UART8_RXM1,
	GPIO3A0_I2S1_SDO1M2,

	/* GRF_GPIO3B_IOMUX_L */
	GPIO3B2_SHIFT		= 8,
	GPIO3B2_MASK		= GENMASK(10, 8),
	GPIO3B2_GPIO		= 0,
	GPIO3B2_LCDC_D17,
	GPIO3B2_BT1120_D8,
	GPIO3B2_GMAC1_RXD1M0,
	GPIO3B2_UART4_TXM1,
	GPIO3B2_PWM9_M0,

	GPIO3B1_SHIFT		= 4,
	GPIO3B1_MASK		= GENMASK(6, 4),
	GPIO3B1_GPIO		= 0,
	GPIO3B1_LCDC_D16,
	GPIO3B1_BT1120_D7,
	GPIO3B1_GMAC1_RXD0M0,
	GPIO3B1_UART4_RXM1,
	GPIO3B1_PWM8_M0,

	/* GRF_GPIO3B_IOMUX_H */
	GPIO3B7_SHIFT		= 12,
	GPIO3B7_MASK		= GENMASK(14, 12),
	GPIO3B7_GPIO		= 0,
	GPIO3B7_LCDC_D22,
	GPIO3B7_PWM12_M0,
	GPIO3B7_GMAC1_TXENM0,
	GPIO3B7_UART3_TXM1,
	GPIO3B7_PDM_SDI2M2,

	/* GRF_GPIO3C_IOMUX_L */
	GPIO3C3_SHIFT		= 12,
	GPIO3C3_MASK		= GENMASK(14, 12),
	GPIO3C3_GPIO		= 0,
	GPIO3C3_LCDC_DEN,
	GPIO3C3_BT1120_D15,
	GPIO3C3_SPI1_CLKM1,
	GPIO3C3_UART5_RXM1,
	GPIO3C3_I2S1_SCLKRXM,

	GPIO3C2_SHIFT		= 8,
	GPIO3C2_MASK		= GENMASK(10, 8),
	GPIO3C2_GPIO		= 0,
	GPIO3C2_LCDC_VSYNC,
	GPIO3C2_BT1120_D14,
	GPIO3C2_SPI1_MISOM1,
	GPIO3C2_UART5_TXM1,
	GPIO3C2_I2S1_SDO3M2,

	GPIO3C0_SHIFT		= 0,
	GPIO3C0_MASK		= GENMASK(2, 0),
	GPIO3C0_GPIO		= 0,
	GPIO3C0_LCDC_D23,
	GPIO3C0_PWM13_M0,
	GPIO3C0_GMAC1_MCLKINOUTM0,
	GPIO3C0_UART3_RXM1,
	GPIO3C0_PDM_SDI3M2,

	/* GRF_GPIO3C_IOMUX_H */
	GPIO3C5_SHIFT		= 4,
	GPIO3C5_MASK		= GENMASK(6, 4),
	GPIO3C5_GPIO		= 0,
	GPIO3C5_PWM15_IRM0,
	GPIO3C5_SPDIF_TXM1,
	GPIO3C5_GMAC1_MDIOM0,
	GPIO3C5_UART7_RXM1,
	GPIO3C5_I2S1_LRCKRXM2,

	GPIO3C4_SHIFT		= 0,
	GPIO3C4_MASK		= GENMASK(2, 0),
	GPIO3C4_GPIO		= 0,
	GPIO3C4_PWM14_M0,
	GPIO3C4_VOP_PWMM1,
	GPIO3C4_GMAC1_MDCM0,
	GPIO3C4_UART7_TXM1,
	GPIO3C4_PDM_CLK1M2,

	/* GRF_GPIO3D_IOMUX_H */
	GPIO3D7_SHIFT		= 12,
	GPIO3D7_MASK		= GENMASK(14, 12),
	GPIO3D7_GPIO		= 0,
	GPIO3D7_CIF_D9,
	GPIO3D7_EBC_SDDO9,
	GPIO3D7_GMAC1_TXD3M1,
	GPIO3D7_UART1_RXM1,
	GPIO3D7_PDM_SDI0M1,

	GPIO3D6_SHIFT		= 8,
	GPIO3D6_MASK		= GENMASK(10, 8),
	GPIO3D6_GPIO		= 0,
	GPIO3D6_CIF_D8,
	GPIO3D6_EBC_SDDO8,
	GPIO3D6_GMAC1_TXD2M1,
	GPIO3D6_UART1_TXM1,
	GPIO3D6_PDM_CLK0M1,

	/* GRF_GPIO4A_IOMUX_L */
	GPIO4A3_SHIFT		= 12,
	GPIO4A3_MASK		= GENMASK(14, 12),
	GPIO4A3_GPIO		= 0,
	GPIO4A3_CIF_D13,
	GPIO4A3_EBC_SDDO13,
	GPIO4A3_GMAC1_RXCLKM1,
	GPIO4A3_UART7_RXM2,
	GPIO4A3_PDM_SDI3M1,

	GPIO4A2_SHIFT		= 8,
	GPIO4A2_MASK		= GENMASK(10, 8),
	GPIO4A2_GPIO		= 0,
	GPIO4A2_CIF_D12,
	GPIO4A2_EBC_SDDO12,
	GPIO4A2_GMAC1_RXD3M1,
	GPIO4A2_UART7_TXM2,
	GPIO4A2_PDM_SDI2M1,

	/* GRF_GPIO4A_IOMUX_H */
	GPIO4A5_SHIFT		= 4,
	GPIO4A5_MASK		= GENMASK(6, 4),
	GPIO4A5_GPIO		= 0,
	GPIO4A5_CIF_D15,
	GPIO4A5_EBC_SDDO15,
	GPIO4A5_GMAC1_TXD1M1,
	GPIO4A5_UART9_RXM2,
	GPIO4A5_I2S2_LRCKRXM1,

	GPIO4A4_SHIFT		= 0,
	GPIO4A4_MASK		= GENMASK(2, 0),
	GPIO4A4_GPIO		= 0,
	GPIO4A4_CIF_D14,
	GPIO4A4_EBC_SDDO14,
	GPIO4A4_GMAC1_TXD0M1,
	GPIO4A4_UART9_TXM2,
	GPIO4A4_I2S2_LRCKTXM1,

	/* GRF_GPIO4C_IOMUX_H */
	GPIO4C6_SHIFT		= 8,
	GPIO4C6_MASK		= GENMASK(10, 8),
	GPIO4C6_GPIO		= 0,
	GPIO4C6_PWM13_M1,
	GPIO4C6_SPI3_CS0M1,
	GPIO4C6_SATA0_ACTLED,
	GPIO4C6_UART9_RXM1,
	GPIO4C6_I2S3_SDIM1,

	GPIO4C5_SHIFT		= 4,
	GPIO4C5_MASK		= GENMASK(6, 4),
	GPIO4C5_GPIO		= 0,
	GPIO4C5_PWM12_M1,
	GPIO4C5_SPI3_MISOM1,
	GPIO4C5_SATA1_ACTLED,
	GPIO4C5_UART9_TXM1,
	GPIO4C5_I2S3_SDOM1,

	/* GRF_IOFUNC_SEL3 */
	UART4_IO_SEL_SHIFT	= 14,
	UART4_IO_SEL_MASK	= GENMASK(14, 14),
	UART4_IO_SEL_M0		= 0,
	UART4_IO_SEL_M1,

	UART3_IO_SEL_SHIFT	= 12,
	UART3_IO_SEL_MASK	= GENMASK(12, 12),
	UART3_IO_SEL_M0		= 0,
	UART3_IO_SEL_M1,

	UART2_IO_SEL_SHIFT	= 10,
	UART2_IO_SEL_MASK	= GENMASK(11, 10),
	UART2_IO_SEL_M0		= 0,
	UART2_IO_SEL_M1,

	UART1_IO_SEL_SHIFT	= 8,
	UART1_IO_SEL_MASK	= GENMASK(8, 8),
	UART1_IO_SEL_M0		= 0,
	UART1_IO_SEL_M1,

	/* GRF_IOFUNC_SEL4 */
	UART9_IO_SEL_SHIFT	= 8,
	UART9_IO_SEL_MASK	= GENMASK(9, 8),
	UART9_IO_SEL_M0		= 0,
	UART9_IO_SEL_M1,
	UART9_IO_SEL_M2,

	UART8_IO_SEL_SHIFT	= 6,
	UART8_IO_SEL_MASK	= GENMASK(6, 6),
	UART8_IO_SEL_M0		= 0,
	UART8_IO_SEL_M1,

	UART7_IO_SEL_SHIFT	= 4,
	UART7_IO_SEL_MASK	= GENMASK(5, 4),
	UART7_IO_SEL_M0		= 0,
	UART7_IO_SEL_M1,
	UART7_IO_SEL_M2,

	UART6_IO_SEL_SHIFT	= 2,
	UART6_IO_SEL_MASK	= GENMASK(2, 2),
	UART6_IO_SEL_M0		= 0,
	UART6_IO_SEL_M1,

	UART5_IO_SEL_SHIFT	= 0,
	UART5_IO_SEL_MASK	= GENMASK(0, 0),
	UART5_IO_SEL_M0		= 0,
	UART5_IO_SEL_M1,
};

static struct mm_region rk3568_mem_map[] = {
	{
		.virt = 0x0UL,
		.phys = 0x0UL,
		.size = 0xf0000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0xf0000000UL,
		.phys = 0xf0000000UL,
		.size = 0x10000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = rk3568_mem_map;

void board_debug_uart_init(void)
{
#if defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfdd5000)
	static struct rk3568_pmugrf * const pmugrf = (void *)PMUGRF_BASE;
	/* UART0 M0 */
	rk_clrsetreg(&pmugrf->pmu_soc_con0, UART0_IO_SEL_MASK,
		     UART0_IO_SEL_M0 << UART0_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&pmugrf->pmu_gpio0c_iomux_l,
		     GPIO0C1_MASK | GPIO0C0_MASK,
		     GPIO0C1_UART0_TX << GPIO0C1_SHIFT |
		     GPIO0C0_UART0_RX << GPIO0C0_SHIFT);

#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfe650000)
	static struct rk3568_grf * const grf = (void *)GRF_BASE;

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)
	/* UART1 M0 */
	rk_clrsetreg(&grf->iofunc_sel3, UART1_IO_SEL_MASK,
		     UART1_IO_SEL_M0 << UART1_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio2b_iomux_l,
		     GPIO2B3_MASK, GPIO2B3_UART1_RXM0 << GPIO2B3_SHIFT);
	rk_clrsetreg(&grf->gpio2b_iomux_h,
		     GPIO2B4_MASK, GPIO2B4_UART1_TXM0 << GPIO2B4_SHIFT);
#else
	/* UART1 M1 */
	rk_clrsetreg(&grf->iofunc_sel3, UART1_IO_SEL_MASK,
		     UART1_IO_SEL_M1 << UART1_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio3d_iomux_h,
		     GPIO3D7_MASK | GPIO3D6_MASK,
		     GPIO3D7_UART1_RXM1 << GPIO3D7_SHIFT |
		     GPIO3D6_UART1_TXM1 << GPIO3D6_SHIFT);
#endif
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfe660000)
	static struct rk3568_grf * const grf = (void *)GRF_BASE;

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)
	static struct rk3568_pmugrf * const pmugrf = (void *)PMUGRF_BASE;
	/* UART2 M0 */
	rk_clrsetreg(&grf->iofunc_sel3, UART2_IO_SEL_MASK,
		     UART2_IO_SEL_M0 << UART2_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&pmugrf->pmu_gpio0d_iomux_l,
		     GPIO0D1_MASK | GPIO0D0_MASK,
		     GPIO0D1_UART2_TXM0 << GPIO0D1_SHIFT |
		     GPIO0D0_UART2_RXM0 << GPIO0D0_SHIFT);
#else
	/* UART2 M1 */
	rk_clrsetreg(&grf->iofunc_sel3, UART2_IO_SEL_MASK,
		     UART2_IO_SEL_M1 << UART2_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio1d_iomux_h,
		     GPIO1D6_MASK | GPIO1D5_MASK,
		     GPIO1D6_UART2_RXM1 << GPIO1D6_SHIFT |
		     GPIO1D5_UART2_TXM1 << GPIO1D5_SHIFT);
#endif
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfe670000)
	static struct rk3568_grf * const grf = (void *)GRF_BASE;

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)
	/* UART3 M0 */
	rk_clrsetreg(&grf->iofunc_sel3, UART3_IO_SEL_MASK,
		     UART3_IO_SEL_M0 << UART3_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->pmu_gpio1a_iomux_l,
		     GPIO1A1_MASK | GPIO1A0_MASK,
		     GPIO1A1_UART3_TXM0 << GPIO1A1_SHIFT |
		     GPIO1A0_UART3_RXM0 << GPIO1A0_SHIFT);
#else
	/* UART3 M1 */
	rk_clrsetreg(&grf->iofunc_sel3, UART3_IO_SEL_MASK,
		     UART3_IO_SEL_M1 << UART3_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio3b_iomux_h,
		     GPIO3B7_MASK, GPIO3B7_UART3_TXM1 << GPIO3B7_SHIFT);
	rk_clrsetreg(&grf->gpio3c_iomux_l,
		     GPIO3C0_MASK, GPIO3C0_UART3_RXM1 << GPIO3C0_SHIFT);
#endif
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfe680000)
	static struct rk3568_grf * const grf = (void *)GRF_BASE;

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)
	/* UART4 M0 */
	rk_clrsetreg(&grf->iofunc_sel3, UART4_IO_SEL_MASK,
		     UART4_IO_SEL_M0 << UART4_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->pmu_gpio1a_iomux_h,
		     GPIO1A6_MASK | GPIO1A4_MASK,
		     GPIO1A6_UART4_TXM0 << GPIO1A6_SHIFT |
		     GPIO1A4_UART4_RXM0 << GPIO1A4_SHIFT);
#else
	/* UART4 M1 */
	rk_clrsetreg(&grf->iofunc_sel3, UART4_IO_SEL_MASK,
		     UART4_IO_SEL_M1 << UART4_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio3b_iomux_l,
		     GPIO3B2_MASK | GPIO3B1_MASK,
		     GPIO3B2_UART4_TXM1 << GPIO3B2_SHIFT |
		     GPIO3B1_UART4_RXM1 << GPIO3B1_SHIFT);
#endif
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfe690000)
	static struct rk3568_grf * const grf = (void *)GRF_BASE;

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)
	/* UART5 M0 */
	rk_clrsetreg(&grf->iofunc_sel4, UART5_IO_SEL_MASK,
		     UART5_IO_SEL_M0 << UART5_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->pmu_gpio2a_iomux_l,
		     GPIO2A2_MASK | GPIO2A1_MASK,
		     GPIO2A2_UART5_TXM0 << GPIO2A2_SHIFT |
		     GPIO2A1_UART5_RXM0 << GPIO2A1_SHIFT);
#else
	/* UART5 M1 */
	rk_clrsetreg(&grf->iofunc_sel4, UART5_IO_SEL_MASK,
		     UART5_IO_SEL_M1 << UART5_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio3c_iomux_l,
		     GPIO3C3_MASK | GPIO3C2_MASK,
		     GPIO3C3_UART5_RXM1 << GPIO3C3_SHIFT |
		     GPIO3C2_UART5_TXM1 << GPIO3C2_SHIFT);
#endif
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfe6a0000)
	static struct rk3568_grf * const grf = (void *)GRF_BASE;

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)
	/* UART6 M0 */
	rk_clrsetreg(&grf->iofunc_sel4, UART6_IO_SEL_MASK,
		     UART6_IO_SEL_M0 << UART6_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio2a_iomux_l,
		     GPIO2A3_MASK, GPIO2A3_UART6_RXM0 << GPIO2A3_SHIFT);
	rk_clrsetreg(&grf->gpio2a_iomux_h,
		     GPIO2A4_MASK, GPIO2A4_UART6_TXM0 << GPIO2A4_SHIFT);
#else
	/* UART6 M1 */
	rk_clrsetreg(&grf->iofunc_sel4, UART6_IO_SEL_MASK,
		     UART6_IO_SEL_M1 << UART6_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio1d_iomux_h,
		     GPIO1D6_MASK | GPIO1D5_MASK,
		     GPIO1D6_UART6_RXM1 << GPIO1D6_SHIFT |
		     GPIO1D5_UART6_TXM1 << GPIO1D5_SHIFT);
#endif
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfe6b0000)
	static struct rk3568_grf * const grf = (void *)GRF_BASE;

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)
	/* UART7 M0 */
	rk_clrsetreg(&grf->iofunc_sel4, UART7_IO_SEL_MASK,
		     UART7_IO_SEL_M0 << UART7_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio2a_iomux_h,
		     GPIO2A6_MASK | GPIO2A5_MASK,
		     GPIO2A6_UART7_TXM0 << GPIO2A6_SHIFT |
		     GPIO2A5_UART7_RXM0 << GPIO2A5_SHIFT);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)
	/* UART7 M1 */
	rk_clrsetreg(&grf->iofunc_sel4, UART7_IO_SEL_MASK,
		     UART7_IO_SEL_M1 << UART7_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio3c_iomux_h,
		     GPIO3C5_MASK | GPIO3C4_MASK,
		     GPIO3C5_UART7_RXM1 << GPIO3C5_SHIFT |
		     GPIO3C4_UART7_TXM1 << GPIO3C4_SHIFT);
#else
	/* UART7 M2 */
	rk_clrsetreg(&grf->iofunc_sel4, UART7_IO_SEL_MASK,
		     UART7_IO_SEL_M2 << UART7_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio4a_iomux_l,
		     GPIO4A3_MASK | GPIO4A2_MASK,
		     GPIO4A3_UART7_RXM2 << GPIO4A3_SHIFT |
		     GPIO4A2_UART7_TXM2 << GPIO4A2_SHIFT);
#endif
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfe6c0000)
	static struct rk3568_grf * const grf = (void *)GRF_BASE;

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)
	/* UART8 M0 */
	rk_clrsetreg(&grf->iofunc_sel4, UART8_IO_SEL_MASK,
		     UART8_IO_SEL_M0 << UART8_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio2c_iomux_h,
		     GPIO2C6_MASK | GPIO2C5_MASK,
		     GPIO2C6_UART8_RXM0 << GPIO2C6_SHIFT |
		     GPIO2C5_UART8_TXM0 << GPIO2C5_SHIFT);
#else
	/* UART8 M1 */
	rk_clrsetreg(&grf->iofunc_sel4, UART8_IO_SEL_MASK,
		     UART8_IO_SEL_M1 << UART8_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio2d_iomux_h,
		     GPIO2D7_MASK | GPIO3A0_MASK,
		     GPIO2D7_UART8_TXM1 << GPIO2D7_SHIFT |
		     GPIO3A0_UART8_RXM1 << GPIO3A0_SHIFT);
#endif
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfe6d0000)
	static struct rk3568_grf * const grf = (void *)GRF_BASE;

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)
	/* UART9 M0 */
	rk_clrsetreg(&grf->iofunc_sel4, UART9_IO_SEL_MASK,
		     UART9_IO_SEL_M0 << UART9_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio2a_iomux_h,
		     GPIO2A7_MASK, GPIO2A7_UART9_RXM0 << GPIO2A7_SHIFT);
	rk_clrsetreg(&grf->gpio2b_iomux_l,
		     GPIO2B0_MASK, GPIO2B0_UART9_TXM0 << GPIO2B0_SHIFT);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)
	/* UART9 M1 */
	rk_clrsetreg(&grf->iofunc_sel4, UART9_IO_SEL_MASK,
		     UART9_IO_SEL_M1 << UART9_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio4c_iomux_h,
		     GPIO4C6_MASK | GPIO4C5_MASK,
		     GPIO4C6_UART9_RXM1 << GPIO4C6_SHIFT |
		     GPIO4C5_UART9_TXM1 << GPIO4C5_SHIFT);
#else
	/* UART9 M2 */
	rk_clrsetreg(&grf->iofunc_sel4, UART9_IO_SEL_MASK,
		     UART9_IO_SEL_M2 << UART9_IO_SEL_SHIFT);

	/* Switch iomux */
	rk_clrsetreg(&grf->gpio4a_iomux_h,
		     GPIO4A5_MASK | GPIO4A4_MASK,
		     GPIO4A5_UART9_RXM2 << GPIO4A5_SHIFT |
		     GPIO4A4_UART9_TXM2 << GPIO4A4_SHIFT);
#endif
#endif
}

int arch_cpu_init(void)
{
#ifdef CONFIG_SPL_BUILD
	/*
	 * When perform idle operation, corresponding clock can
	 * be opened or gated automatically.
	 */
	writel(0xffffffff, PMU_BASE_ADDR + PMU_NOC_AUTO_CON0);
	writel(0x000f000f, PMU_BASE_ADDR + PMU_NOC_AUTO_CON1);

	/* Set the emmc sdmmc0 to secure */
	writel(((0x3 << 11 | 0x1 << 4) << 16), SGRF_BASE + SGRF_SOC_CON4);
	/* set the emmc ds to level 2 */
	writel(0x3f3f0707, GRF_BASE + GRF_GPIO1B_DS_2);
	writel(0x3f3f0707, GRF_BASE + GRF_GPIO1B_DS_3);
	writel(0x3f3f0707, GRF_BASE + GRF_GPIO1C_DS_0);
	writel(0x3f3f0707, GRF_BASE + GRF_GPIO1C_DS_1);
	writel(0x3f3f0707, GRF_BASE + GRF_GPIO1C_DS_2);
	writel(0x3f3f0707, GRF_BASE + GRF_GPIO1C_DS_3);

#ifndef CONFIG_TPL_BUILD
	/* set the fspi d0 cs0 to level 1 */
	if (get_bootdev_by_brom_bootsource() == BOOT_TYPE_SPI_NOR ||
	    get_bootdev_by_brom_bootsource() == BOOT_TYPE_SPI_NAND) {
		writel(0x3f000300, GRF_BASE + GRF_GPIO1D_DS_0);
		writel(0x3f000300, GRF_BASE + GRF_GPIO1D_DS_1);
	}
#endif

	/* Disable eDP phy by default */
	writel(0x00070007, EDP_PHY_GRF_CON10);
	writel(0x0ff10ff1, EDP_PHY_GRF_CON0);

	/* Set core pvtpll ring length */
	writel(0x00ff002b, CPU_GRF_BASE + GRF_CORE_PVTPLL_CON0);
#endif

	return 0;
}

#ifdef CONFIG_SPL_BUILD
int spl_fit_standalone_release(uintptr_t entry_point)
{
	/* Reset the scr1 */
	writel(0x04000400, CRU_BASE + CRU_SOFTRST_CON26);
	udelay(100);
	/* set the scr1 addr */
	writel((0xffff0000) | (entry_point >> 16), GRF_BASE + GRF_SOC_CON4);
	udelay(10);
	/* release the scr1 */
	writel(0x04000000, CRU_BASE + CRU_SOFTRST_CON26);

	return 0;
}
#endif
