/* Copyright (c) 2015 SpeedModTeam
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/leds-pmic8058.h>
#include <linux/pwm.h>
#include <linux/pmic8058-pwm.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <mach/pmic.h>
#include <mach/camera.h>
#include <mach/gpio.h>

#if defined CONFIG_MSM_CAMERA_FLASH_SC628A
static struct i2c_client *sc628a_client;

static const struct i2c_device_id sc628a_i2c_id[] = {
	{"sc628a", 0},
	{ }
};

static int32_t sc628a_i2c_txdata(unsigned short saddr,
		unsigned char *txdata, int length)
{
	struct i2c_msg msg[] = {
		{
			.addr = saddr,
			.flags = 0,
			.len = length,
			.buf = txdata,
		},
	};
	if (i2c_transfer(sc628a_client->adapter, msg, 1) < 0) {
		CDBG("sc628a_i2c_txdata faild 0x%x\n", saddr);
		return -EIO;
	}

	return 0;
}

static int32_t sc628a_i2c_write_b_flash(uint8_t waddr, uint8_t bdata)
{
	int32_t rc = -EFAULT;
	unsigned char buf[2];
	if (!sc628a_client)
		return  -ENOTSUPP;

	memset(buf, 0, sizeof(buf));
	buf[0] = waddr;
	buf[1] = bdata;

	rc = sc628a_i2c_txdata(sc628a_client->addr>>1, buf, 2);
	if (rc < 0) {
		CDBG("i2c_write_b failed, addr = 0x%x, val = 0x%x!\n",
				waddr, bdata);
	}
	usleep_range(4000, 5000);

	return rc;
}

static int sc628a_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	int rc = 0;
	CDBG("sc628a_probe called!\n");

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		pr_err("i2c_check_functionality failed\n");
		goto probe_failure;
	}

	sc628a_client = client;

	CDBG("sc628a_probe successed! rc = %d\n", rc);
	return 0;

probe_failure:
	pr_err("sc628a_probe failed! rc = %d\n", rc);
	return rc;
}

static struct i2c_driver sc628a_i2c_driver = {
	.id_table = sc628a_i2c_id,
	.probe  = sc628a_i2c_probe,
	.remove = __exit_p(sc628a_i2c_remove),
	.driver = {
		.name = "sc628a",
	},
};
#endif
