/***************************************************************************//**
 *   @file   ad796x.c
 *   @brief  Implementation of AD796X Driver.
 *   @author Axel Haslam (ahaslam@baylibre.com)
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

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <stdlib.h>
#include <errno.h>
#include "ad796x.h"
#include "no_os_alloc.h"

/******************************************************************************/
/************************ Functions Definitions *******************************/
/******************************************************************************/
/******************************************************************************/


/**
 * @brief Initialize the device.
 * @param device - The device structure.
 * @param init_param - The structure that contains the device initial
 * 		       parameters.
 * @return 0 in case of success, negative error code otherwise.
 */
int ad796x_init(struct ad796x_dev **device,
		struct ad796x_init_param init_param)
{
	struct ad796x_dev *dev;
	int ret;

	dev = (struct ad796x_dev *)no_os_calloc(1, sizeof(*dev));
	if (!dev)
		return -ENOMEM;

	/* GPIO Initialization */
	ret = no_os_gpio_get(&dev->gpio_adc_en3_fmc, init_param.gpio_adc_en3_fmc_param);
	if (ret)
		goto error_en3_fmc;

	ret = no_os_gpio_get(&dev->gpio_adc_en2_fmc, init_param.gpio_adc_en2_fmc_param);
	if (ret)
		goto error_en2_fmc;

	ret = no_os_gpio_get(&dev->gpio_adc_en1_fmc, init_param.gpio_adc_en1_fmc_param);
	if (ret)
		goto error_en1_fmc;

	ret = no_os_gpio_get(&dev->gpio_adc_en0_fmc, init_param.gpio_adc_en0_fmc_param);
	if (ret)
		goto error_en0_fmc;

	ret = no_os_gpio_direction_output(dev->gpio_adc_en3_fmc, NO_OS_GPIO_LOW);
	if (ret)
		goto error;

	ret = no_os_gpio_direction_output(dev->gpio_adc_en2_fmc, NO_OS_GPIO_LOW);
	if (ret)
		goto error;

	ret = no_os_gpio_direction_output(dev->gpio_adc_en1_fmc, NO_OS_GPIO_LOW);
	if (ret)
		goto error;

	ret = no_os_gpio_direction_output(dev->gpio_adc_en0_fmc, NO_OS_GPIO_LOW);
	if (ret)
		goto error;

	ret = no_os_gpio_set_value(dev->gpio_adc_en0_fmc, NO_OS_GPIO_HIGH);
	if (ret)
		goto error;

	ret = no_os_gpio_set_value(dev->gpio_adc_en1_fmc, NO_OS_GPIO_LOW);
	if (ret)
		goto error;

	ret = no_os_gpio_set_value(dev->gpio_adc_en2_fmc, NO_OS_GPIO_HIGH);
	if (ret)
		goto error;

	ret = no_os_gpio_set_value(dev->gpio_adc_en3_fmc, NO_OS_GPIO_LOW);
	if (ret)
		goto error;

	*device = dev;

	return 0;

error:
	no_os_gpio_remove(dev->gpio_adc_en0_fmc);
error_en0_fmc:
	no_os_gpio_remove(dev->gpio_adc_en1_fmc);
error_en1_fmc:
	no_os_gpio_remove(dev->gpio_adc_en2_fmc);
error_en2_fmc:
	no_os_gpio_remove(dev->gpio_adc_en3_fmc);
error_en3_fmc:

	no_os_free(dev);

	return ret;
}

/**
 * @brief Remove the device and release resources.
 * @param dev - The device structure.
 * @return 0 in case of success, negative error code otherwise.
 */
int ad796x_remove(struct ad796x_dev *dev)
{
	int ret;

	ret = no_os_gpio_remove(dev->gpio_adc_en0_fmc);
	if (ret)
		return ret;

	ret = no_os_gpio_remove(dev->gpio_adc_en1_fmc);
	if (ret)
		return ret;

	ret = no_os_gpio_remove(dev->gpio_adc_en2_fmc);
	if (ret)
		return ret;

	ret = no_os_gpio_remove(dev->gpio_adc_en3_fmc);
	if (ret)
		return ret;

	no_os_free(dev);

	return ret;
}

