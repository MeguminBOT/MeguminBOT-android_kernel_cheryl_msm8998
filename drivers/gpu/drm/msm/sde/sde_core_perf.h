/* Copyright (c) 2016, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __SDE_CORE_PERF_H__
#define __SDE_CORE_PERF_H__

#include <linux/types.h>
#include <linux/dcache.h>
#include <linux/mutex.h>
#include <drm/drm_crtc.h>

#include "sde_hw_catalog.h"
#include "sde_power_handle.h"

/**
 * struct sde_core_perf_params - definition of performance parameters
 * @max_per_pipe_ib: maximum instantaneous bandwidth request
 * @bw_ctl: arbitrated bandwidth request
 * @core_clk_rate: core clock rate request
 */
struct sde_core_perf_params {
	u64 max_per_pipe_ib;
	u64 bw_ctl;
	u32 core_clk_rate;
};

/**
 * struct sde_core_perf_tune - definition of performance tuning control
 * @min_core_clk: minimum core clock
 * @min_bus_vote: minimum bus vote
 */
struct sde_core_perf_tune {
	unsigned long min_core_clk;
	u64 min_bus_vote;
};

/**
 * struct sde_core_perf - definition of core performance context
 * @dev: Pointer to drm device
 * @debugfs_root: top level debug folder
 * @perf_lock: serialization lock for this context
 * @catalog: Pointer to catalog configuration
 * @phandle: Pointer to power handler
 * @pclient: Pointer to power client
 * @clk_name: core clock name
 * @core_clk: Pointer to core clock structure
 * @core_clk_rate: current core clock rate
 * @max_core_clk_rate: maximum allowable core clock rate
 * @perf_tune: debug control for performance tuning
 * @enable_bw_release: debug control for bandwidth release
 */
struct sde_core_perf {
	struct drm_device *dev;
	struct dentry *debugfs_root;
	struct mutex perf_lock;
	struct sde_mdss_cfg *catalog;
	struct sde_power_handle *phandle;
	struct sde_power_client *pclient;
	char *clk_name;
	struct clk *core_clk;
	u32 core_clk_rate;
	u64 max_core_clk_rate;
	struct sde_core_perf_tune perf_tune;
	u32 enable_bw_release;
};

/**
 * sde_core_perf_crtc_check - validate performance of the given crtc state
 * @crtc: Pointer to crtc
 * @state: Pointer to new crtc state
 * return: zero if success, or error code otherwise
 */
int sde_core_perf_crtc_check(struct drm_crtc *crtc,
		struct drm_crtc_state *state);

/**
 * sde_core_perf_crtc_update - update performance of the given crtc
 * @crtc: Pointer to crtc
 * @params_changed: true if crtc parameters are modified
 * @stop_req: true if this is a stop request
 */
void sde_core_perf_crtc_update(struct drm_crtc *crtc,
		int params_changed, bool stop_req);

/**
 * sde_core_perf_crtc_release_bw - release bandwidth of the given crtc
 * @crtc: Pointer to crtc
 */
void sde_core_perf_crtc_release_bw(struct drm_crtc *crtc);

/**
 * sde_core_perf_destroy - destroy the given core performance context
 * @perf: Pointer to core performance context
 */
void sde_core_perf_destroy(struct sde_core_perf *perf);

/**
 * sde_core_perf_init - initialize the given core performance context
 * @perf: Pointer to core performance context
 * @dev: Pointer to drm device
 * @catalog: Pointer to catalog
 * @phandle: Pointer to power handle
 * @pclient: Pointer to power client
 * @clk_name: core clock name
 * @debugfs_parent: Pointer to parent debugfs
 */
int sde_core_perf_init(struct sde_core_perf *perf,
		struct drm_device *dev,
		struct sde_mdss_cfg *catalog,
		struct sde_power_handle *phandle,
		struct sde_power_client *pclient,
		char *clk_name,
		struct dentry *debugfs_parent);

#endif /* __SDE_CORE_PERF_H__ */
