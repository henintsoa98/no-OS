/***************************************************************************//**
 *   @file   stm32_dma.c
 *   @brief  Implementation of stm32 DMA driver.
********************************************************************************
 * Copyright 2024(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "stm32_dma.h"
#include "no_os_error.h"

/**
 * @brief Initialize DMA Peripheral
 * @param desc - The DMA descriptor.
 * @param param - The DMA Initialization Parameters
 * @return 0 in case of success, negative error code otherwise
 */
int32_t stm32_dma_init(struct no_os_dma_desc** desc,
		       const struct no_os_dma_init_param* param)
{

	int ret;
	struct no_os_dma_desc* dma_desc;
	struct stm32_dma_desc* ddesc;
	struct stm32_dma_init_param* dinit;
	DMA_HandleTypeDef* hdma = NULL;

	if (!desc || !param)
		return -EINVAL;

	dma_desc = (struct no_os_dma_desc*)no_os_calloc(1, sizeof(*dma_desc));
	if (!dma_desc)
		return -ENOMEM;

	ddesc = (struct stm32_dma_desc*)no_os_calloc(1, sizeof(*ddesc));
	if (!ddesc)
		return -ENOMEM;

	dma_desc->extra = ddesc;
	dinit = param->extra;

	ddesc->hdma.Instance = dinit->dma_stream;
	ddesc->hdma.Init.Channel = dinit->dma_channel;
	ddesc->hdma.Init.Priority = dinit->priority;
	ddesc->hdma.Init.MemInc = dinit->mem_increment ? DMA_MINC_ENABLE :
				  DMA_MINC_DISABLE;
	ddesc->hdma.Init.PeriphInc = dinit->per_increment ? DMA_PINC_ENABLE :
				     DMA_PINC_DISABLE;
	ddesc->hdma.Init.MemDataAlignment = dinit->mem_data_alignment;
	ddesc->hdma.Init.PeriphDataAlignment = dinit->periph_data_alignment;
	ddesc->hdma.Init.Direction = dinit->direction;
	ddesc->hdma.Init.Mode = dinit->mode;

	ret = HAL_DMA_Init(&ddesc->hdma);
	if (ret != HAL_OK) {
		return -EINVAL;
	}

	*desc = dma_desc;

	return 0;
}

/**
 * @brief stm32 platform specific DMA platform ops structure
 */
const struct no_os_dma_platform_ops stm32_dma_ops = {
	.dma_init = stm32_dma_init,
};