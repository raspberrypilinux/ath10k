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
#ifndef _TXRX_H
#define _TXRX_H

#include "htt.h"

void ath10k_data_tx_completed(struct htt_struct *htt,
			      struct htt_data_tx_done *ev);
void ath10k_mgmt_tx_completed(struct htt_struct *, struct htt_mgmt_tx_done *);
void ath10k_process_rx(struct ath10k *ar, struct htt_rx_info *info);

struct ath10k_peer *ath10k_peer_find(struct ath10k *ar, int vdev_id,
				     const u8 *addr);
int ath10k_wait_for_peer_created(struct ath10k *ar, int vdev_id, const u8 *addr);
int ath10k_wait_for_peer_deleted(struct ath10k *ar, int vdev_id, const u8 *addr);

void ath10k_peer_map_event(struct htt_struct *htt,
			   struct htt_peer_map_event *ev);
void ath10k_peer_unmap_event(struct htt_struct *htt,
			     struct htt_peer_unmap_event *ev);

#endif
