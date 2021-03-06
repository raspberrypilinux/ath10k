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

#ifndef _CE_H_
#define _CE_H_

#include "hif.h"


/* Maximum number of Copy Engine's supported */
#define CE_COUNT_MAX 8
#define CE_HTT_H2T_MSG_SRC_NENTRIES 2048

/* Descriptor rings must be aligned to this boundary */
#define CE_DESC_RING_ALIGN	8
#define CE_SENDLIST_ITEMS_MAX	12
#define CE_SEND_FLAG_GATHER	0x00010000

/*
 * Copy Engine support: low-level Target-side Copy Engine API.
 * This is a hardware access layer used by code that understands
 * how to use copy engines.
 */

struct ce_state;

/*
 * Pops 1 completed send buffer from Source ring.
 */
typedef void (*CE_SEND_CB) (struct ce_state *ce_state,
			    void *per_transfer_send_context,
			    u32 buffer,
			    unsigned int nbytes,
			    unsigned int transfer_id);
/*
 * Pops 1 completed send buffer from Destination ring.
 */
typedef void (*CE_RECV_CB) (struct ce_state *ce_state,
			    void *per_transfer_recv_context,
			    u32 buffer,
			    unsigned int nbytes,
			    unsigned int transfer_id,
			    unsigned int flags);

/* Copy Engine operational state */
enum ce_op_state {
	CE_UNUSED,
	CE_PAUSED,
	CE_RUNNING,
};

#define CE_DESC_FLAGS_GATHER         (1 << 0)
#define CE_DESC_FLAGS_BYTE_SWAP      (1 << 1)
#define CE_DESC_FLAGS_META_DATA_MASK 0xFFFC
#define CE_DESC_FLAGS_META_DATA_LSB  3

struct ce_desc {
	__le32 addr;
	__le16 nbytes;
	__le16 flags; /* %CE_DESC_FLAGS_ */
};

/* Copy Engine Ring internal state */
struct ce_ring_state {

	/* Number of entries in this ring; must be power of 2 */
	unsigned int nentries;
	unsigned int nentries_mask;

	/*
	 * For dest ring, this is the next index to be processed
	 * by software after it was/is received into.
	 *
	 * For src ring, this is the last descriptor that was sent
	 * and completion processed by software.
	 *
	 * Regardless of src or dest ring, this is an invariant
	 * (modulo ring size):
	 *     write index >= read index >= sw_index
	 */
	unsigned int sw_index;
	/* cached copy */
	unsigned int write_index;
	/*
	 * For src ring, this is the next index not yet processed by HW.
	 * This is a cached copy of the real HW index (read index), used
	 * for avoiding reading the HW index register more often than
	 * necessary.
	 * This extends the invariant:
	 *     write index >= read index >= hw_index >= sw_index
	 *
	 * For dest ring, this is currently unused.
	 */
	/* cached copy */
	unsigned int hw_index;

	/* Start of DMA-coherent area reserved for descriptors */
	/* Host address space */
	void *base_addr_owner_space_unaligned;
	/* CE address space */
	u32 base_addr_ce_space_unaligned;

	/*
	 * Actual start of descriptors.
	 * Aligned to descriptor-size boundary.
	 * Points into reserved DMA-coherent area, above.
	 */
	/* Host address space */
	void *base_addr_owner_space;

	/* CE address space */
	u32 base_addr_ce_space;
	/*
	 * Start of shadow copy of descriptors, within regular memory.
	 * Aligned to descriptor-size boundary.
	 */
	char *shadow_base_unaligned;
	struct ce_desc *shadow_base;

	void **per_transfer_context;
};

/* Copy Engine internal state */
struct ce_state {
	struct ath10k *ar;
	unsigned int id;

	unsigned int attr_flags;

	u32 ctrl_addr;
	enum ce_op_state state;

	CE_SEND_CB send_cb;
	CE_RECV_CB recv_cb;

	unsigned int src_sz_max;
	struct ce_ring_state *src_ring;
	struct ce_ring_state *dest_ring;
};

struct ce_sendlist_item {
	/* e.g. buffer or desc list */
	dma_addr_t data;
	union {
		/* simple buffer */
		unsigned int nbytes;
		/* Rx descriptor list */
		unsigned int ndesc;
	} u;
	/* externally-specified flags; OR-ed with internal flags */
	u32 flags;
};

struct ce_sendlist {
	unsigned int num_items;
	struct ce_sendlist_item item[CE_SENDLIST_ITEMS_MAX];
};

/* Copy Engine settable attributes */
struct ce_attr;

/*==================Send====================*/

/* ath10k_ce_send flags */
#define CE_SEND_FLAG_BYTE_SWAP 1

/*
 * Queue a source buffer to be sent to an anonymous destination buffer.
 *   ce         - which copy engine to use
 *   buffer          - address of buffer
 *   nbytes          - number of bytes to send
 *   transfer_id     - arbitrary ID; reflected to destination
 *   flags           - CE_SEND_FLAG_* values
 * Returns 0 on success; otherwise an error status.
 *
 * Note: If no flags are specified, use CE's default data swap mode.
 *
 * Implementation note: pushes 1 buffer to Source ring
 */
int ath10k_ce_send(struct ce_state *ce_state,
		   void *per_transfer_send_context,
		   u32 buffer,
		   unsigned int nbytes,
		   /* 14 bits */
		   unsigned int transfer_id,
		   unsigned int flags);

void ath10k_ce_send_cb_register(struct ce_state *ce_state,
				CE_SEND_CB fn_ptr,
				int disable_interrupts);

/* Append a simple buffer (address/length) to a sendlist. */
void ath10k_ce_sendlist_buf_add(struct ce_sendlist *sendlist,
				u32 buffer,
				unsigned int nbytes,
				/* OR-ed with internal flags */
				u32 flags);

/*
 * Queue a "sendlist" of buffers to be sent using gather to a single
 * anonymous destination buffer
 *   ce         - which copy engine to use
 *   sendlist        - list of simple buffers to send using gather
 *   transfer_id     - arbitrary ID; reflected to destination
 * Returns 0 on success; otherwise an error status.
 *
 * Implemenation note: Pushes multiple buffers with Gather to Source ring.
 */
int ath10k_ce_sendlist_send(struct ce_state *ce_state,
			    void *per_transfer_send_context,
			    struct ce_sendlist *sendlist,
			    /* 14 bits */
			    unsigned int transfer_id);

/*==================Recv=======================*/

/*
 * Make a buffer available to receive. The buffer must be at least of a
 * minimal size appropriate for this copy engine (src_sz_max attribute).
 *   ce                    - which copy engine to use
 *   per_transfer_recv_context  - context passed back to caller's recv_cb
 *   buffer                     - address of buffer in CE space
 * Returns 0 on success; otherwise an error status.
 *
 * Implemenation note: Pushes a buffer to Dest ring.
 */
int ath10k_ce_recv_buf_enqueue(struct ce_state *ce_state,
			       void *per_transfer_recv_context,
			       u32 buffer);

void ath10k_ce_recv_cb_register(struct ce_state *ce_state,
				CE_RECV_CB fn_ptr);

/* recv flags */
/* Data is byte-swapped */
#define CE_RECV_FLAG_SWAPPED	1

/*
 * Supply data for the next completed unprocessed receive descriptor.
 * Pops buffer from Dest ring.
 */
int ath10k_ce_completed_recv_next(struct ce_state *ce_state,
				  void **per_transfer_contextp,
				  u32 *bufferp,
				  unsigned int *nbytesp,
				  unsigned int *transfer_idp,
				  unsigned int *flagsp);
/*
 * Supply data for the next completed unprocessed send descriptor.
 * Pops 1 completed send buffer from Source ring.
 */
int ath10k_ce_completed_send_next(struct ce_state *ce_state,
			   void **per_transfer_contextp,
			   u32 *bufferp,
			   unsigned int *nbytesp,
			   unsigned int *transfer_idp);

/*==================CE Engine Initialization=======================*/

/* Initialize an instance of a CE */
struct ce_state *ath10k_ce_init(struct ath10k *ar,
				unsigned int ce_id,
				struct ce_attr *attr);

/*==================CE Engine Shutdown=======================*/
/*
 * Support clean shutdown by allowing the caller to revoke
 * receive buffers.  Target DMA must be stopped before using
 * this API.
 */
int ath10k_ce_revoke_recv_next(struct ce_state *ce_state,
			       void **per_transfer_contextp,
			       u32 *bufferp);

/*
 * Support clean shutdown by allowing the caller to cancel
 * pending sends.  Target DMA must be stopped before using
 * this API.
 */
int ath10k_ce_cancel_send_next(struct ce_state *ce_state,
			       void **per_transfer_contextp,
			       u32 *bufferp,
			       unsigned int *nbytesp,
			       unsigned int *transfer_idp);

void ath10k_ce_deinit(struct ce_state *ce_state);

/*==================CE Interrupt Handlers====================*/
void ath10k_ce_per_engine_service_any(struct ath10k *ar);
void ath10k_ce_per_engine_service(struct ath10k *ar, unsigned int ce_id);
void ath10k_ce_disable_interrupts(struct ath10k *ar);

/* ce_attr.flags values */
/* Use NonSnooping PCIe accesses? */
#define CE_ATTR_NO_SNOOP		1

/* Byte swap data words */
#define CE_ATTR_BYTE_SWAP_DATA		2

/* Swizzle descriptors? */
#define CE_ATTR_SWIZZLE_DESCRIPTORS	4

/* no interrupt on copy completion */
#define CE_ATTR_DIS_INTR		8

/* Attributes of an instance of a Copy Engine */
struct ce_attr {
	/* CE_ATTR_* values */
	unsigned int flags;

	/* currently not in use */
	unsigned int priority;

	/* #entries in source ring - Must be a power of 2 */
	unsigned int src_nentries;

	/*
	 * Max source send size for this CE.
	 * This is also the minimum size of a destination buffer.
	 */
	unsigned int src_sz_max;

	/* #entries in destination ring - Must be a power of 2 */
	unsigned int dest_nentries;

	/* Future use */
	void *reserved;
};

/*
 * When using sendlist_send to transfer multiple buffer fragments, the
 * transfer context of each fragment, except last one, will be filled
 * with CE_SENDLIST_ITEM_CTXT. ce_completed_send will return success for
 * each fragment done with send and the transfer context would be
 * CE_SENDLIST_ITEM_CTXT. Upper layer could use this to identify the
 * status of a send completion.
 */
#define CE_SENDLIST_ITEM_CTXT	((void *)0xcecebeef)

#define SR_BA_ADDRESS		0x0000
#define SR_SIZE_ADDRESS		0x0004
#define DR_BA_ADDRESS		0x0008
#define DR_SIZE_ADDRESS		0x000c
#define CE_CMD_ADDRESS		0x0018

#define CE_CTRL1_DST_RING_BYTE_SWAP_EN_MSB	17
#define CE_CTRL1_DST_RING_BYTE_SWAP_EN_LSB	17
#define CE_CTRL1_DST_RING_BYTE_SWAP_EN_MASK	0x00020000
#define CE_CTRL1_DST_RING_BYTE_SWAP_EN_SET(x) \
	(((0 | (x)) << CE_CTRL1_DST_RING_BYTE_SWAP_EN_LSB) & \
	CE_CTRL1_DST_RING_BYTE_SWAP_EN_MASK)

#define CE_CTRL1_SRC_RING_BYTE_SWAP_EN_MSB	16
#define CE_CTRL1_SRC_RING_BYTE_SWAP_EN_LSB	16
#define CE_CTRL1_SRC_RING_BYTE_SWAP_EN_MASK	0x00010000
#define CE_CTRL1_SRC_RING_BYTE_SWAP_EN_GET(x) \
	(((x) & CE_CTRL1_SRC_RING_BYTE_SWAP_EN_MASK) >> \
	 CE_CTRL1_SRC_RING_BYTE_SWAP_EN_LSB)
#define CE_CTRL1_SRC_RING_BYTE_SWAP_EN_SET(x) \
	(((0 | (x)) << CE_CTRL1_SRC_RING_BYTE_SWAP_EN_LSB) & \
	 CE_CTRL1_SRC_RING_BYTE_SWAP_EN_MASK)

#define CE_CTRL1_DMAX_LENGTH_MSB		15
#define CE_CTRL1_DMAX_LENGTH_LSB		0
#define CE_CTRL1_DMAX_LENGTH_MASK		0x0000ffff
#define CE_CTRL1_DMAX_LENGTH_GET(x) \
	(((x) & CE_CTRL1_DMAX_LENGTH_MASK) >> CE_CTRL1_DMAX_LENGTH_LSB)
#define CE_CTRL1_DMAX_LENGTH_SET(x) \
	(((0 | (x)) << CE_CTRL1_DMAX_LENGTH_LSB) & CE_CTRL1_DMAX_LENGTH_MASK)

#define CE_CTRL1_ADDRESS			0x0010
#define CE_CTRL1_HW_MASK			0x0007ffff
#define CE_CTRL1_SW_MASK			0x0007ffff
#define CE_CTRL1_HW_WRITE_MASK			0x00000000
#define CE_CTRL1_SW_WRITE_MASK			0x0007ffff
#define CE_CTRL1_RSTMASK			0xffffffff
#define CE_CTRL1_RESET				0x00000080

#define CE_CMD_HALT_STATUS_MSB			3
#define CE_CMD_HALT_STATUS_LSB			3
#define CE_CMD_HALT_STATUS_MASK			0x00000008
#define CE_CMD_HALT_STATUS_GET(x) \
	(((x) & CE_CMD_HALT_STATUS_MASK) >> CE_CMD_HALT_STATUS_LSB)
#define CE_CMD_HALT_STATUS_SET(x) \
	(((0 | (x)) << CE_CMD_HALT_STATUS_LSB) & CE_CMD_HALT_STATUS_MASK)
#define CE_CMD_HALT_STATUS_RESET		0
#define CE_CMD_HALT_MSB				0
#define CE_CMD_HALT_MASK			0x00000001

#define HOST_IE_COPY_COMPLETE_MSB		0
#define HOST_IE_COPY_COMPLETE_LSB		0
#define HOST_IE_COPY_COMPLETE_MASK		0x00000001
#define HOST_IE_COPY_COMPLETE_GET(x) \
	(((x) & HOST_IE_COPY_COMPLETE_MASK) >> HOST_IE_COPY_COMPLETE_LSB)
#define HOST_IE_COPY_COMPLETE_SET(x) \
	(((0 | (x)) << HOST_IE_COPY_COMPLETE_LSB) & HOST_IE_COPY_COMPLETE_MASK)
#define HOST_IE_COPY_COMPLETE_RESET		0
#define HOST_IE_ADDRESS				0x002c

#define HOST_IS_DST_RING_LOW_WATERMARK_MASK	0x00000010
#define HOST_IS_DST_RING_HIGH_WATERMARK_MASK	0x00000008
#define HOST_IS_SRC_RING_LOW_WATERMARK_MASK	0x00000004
#define HOST_IS_SRC_RING_HIGH_WATERMARK_MASK	0x00000002
#define HOST_IS_COPY_COMPLETE_MASK		0x00000001
#define HOST_IS_ADDRESS				0x0030

#define MISC_IE_ADDRESS				0x0034

#define MISC_IS_AXI_ERR_MASK			0x00000400

#define MISC_IS_DST_ADDR_ERR_MASK		0x00000200
#define MISC_IS_SRC_LEN_ERR_MASK		0x00000100
#define MISC_IS_DST_MAX_LEN_VIO_MASK		0x00000080
#define MISC_IS_DST_RING_OVERFLOW_MASK		0x00000040
#define MISC_IS_SRC_RING_OVERFLOW_MASK		0x00000020

#define MISC_IS_ADDRESS				0x0038

#define SR_WR_INDEX_ADDRESS			0x003c

#define DST_WR_INDEX_ADDRESS			0x0040

#define CURRENT_SRRI_ADDRESS			0x0044

#define CURRENT_DRRI_ADDRESS			0x0048

#define SRC_WATERMARK_LOW_MSB			31
#define SRC_WATERMARK_LOW_LSB			16
#define SRC_WATERMARK_LOW_MASK			0xffff0000
#define SRC_WATERMARK_LOW_GET(x) \
	(((x) & SRC_WATERMARK_LOW_MASK) >> SRC_WATERMARK_LOW_LSB)
#define SRC_WATERMARK_LOW_SET(x) \
	(((0 | (x)) << SRC_WATERMARK_LOW_LSB) & SRC_WATERMARK_LOW_MASK)
#define SRC_WATERMARK_LOW_RESET			0
#define SRC_WATERMARK_HIGH_MSB			15
#define SRC_WATERMARK_HIGH_LSB			0
#define SRC_WATERMARK_HIGH_MASK			0x0000ffff
#define SRC_WATERMARK_HIGH_GET(x) \
	(((x) & SRC_WATERMARK_HIGH_MASK) >> SRC_WATERMARK_HIGH_LSB)
#define SRC_WATERMARK_HIGH_SET(x) \
	(((0 | (x)) << SRC_WATERMARK_HIGH_LSB) & SRC_WATERMARK_HIGH_MASK)
#define SRC_WATERMARK_HIGH_RESET		0
#define SRC_WATERMARK_ADDRESS			0x004c

#define DST_WATERMARK_LOW_LSB			16
#define DST_WATERMARK_LOW_MASK			0xffff0000
#define DST_WATERMARK_LOW_SET(x) \
	(((0 | (x)) << DST_WATERMARK_LOW_LSB) & DST_WATERMARK_LOW_MASK)
#define DST_WATERMARK_LOW_RESET			0
#define DST_WATERMARK_HIGH_MSB			15
#define DST_WATERMARK_HIGH_LSB			0
#define DST_WATERMARK_HIGH_MASK			0x0000ffff
#define DST_WATERMARK_HIGH_GET(x) \
	(((x) & DST_WATERMARK_HIGH_MASK) >> DST_WATERMARK_HIGH_LSB)
#define DST_WATERMARK_HIGH_SET(x) \
	(((0 | (x)) << DST_WATERMARK_HIGH_LSB) & DST_WATERMARK_HIGH_MASK)
#define DST_WATERMARK_HIGH_RESET		0
#define DST_WATERMARK_ADDRESS			0x0050


#define CE_BASE_ADDRESS(ar, ce_id) \
	(CE0_BASE_ADDRESS_T((ar)) + \
	((CE1_BASE_ADDRESS_T((ar))-CE0_BASE_ADDRESS_T((ar)))*(ce_id)))

#define CE_SRC_RING_WRITE_IDX_SET(ar, targid, ce_ctrl_addr, n) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + SR_WR_INDEX_ADDRESS, (n))

#define CE_SRC_RING_WRITE_IDX_GET(targid, ce_ctrl_addr) \
	TARGET_READ((targid), (ce_ctrl_addr) + SR_WR_INDEX_ADDRESS)

#define CE_SRC_RING_READ_IDX_GET(targid, ce_ctrl_addr) \
	TARGET_READ((targid), (ce_ctrl_addr) + CURRENT_SRRI_ADDRESS)

#define CE_SRC_RING_BASE_ADDR_SET(ar, targid, ce_ctrl_addr, addr) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + SR_BA_ADDRESS, (addr))

#define CE_SRC_RING_SZ_SET(ar, targid, ce_ctrl_addr, n) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + SR_SIZE_ADDRESS, (n))

#define CE_SRC_RING_DMAX_SET(ar, targid, ce_ctrl_addr, n) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + CE_CTRL1_ADDRESS, \
	(TARGET_READ((targid), (ce_ctrl_addr) + CE_CTRL1_ADDRESS) & \
		~CE_CTRL1_DMAX_LENGTH_MASK) | \
		CE_CTRL1_DMAX_LENGTH_SET(n))

#define CE_SRC_RING_BYTE_SWAP_SET(ar, targid, ce_ctrl_addr, n) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + CE_CTRL1_ADDRESS, \
	(TARGET_READ((targid), (ce_ctrl_addr) + CE_CTRL1_ADDRESS) & \
		~CE_CTRL1_SRC_RING_BYTE_SWAP_EN_MASK) | \
		CE_CTRL1_SRC_RING_BYTE_SWAP_EN_SET(n))

#define CE_DEST_RING_BYTE_SWAP_SET(ar, targid, ce_ctrl_addr, n) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + CE_CTRL1_ADDRESS, \
	(TARGET_READ((targid), (ce_ctrl_addr) + CE_CTRL1_ADDRESS) & \
		~CE_CTRL1_DST_RING_BYTE_SWAP_EN_MASK) | \
		CE_CTRL1_DST_RING_BYTE_SWAP_EN_SET(n))

#define CE_DEST_RING_WRITE_IDX_SET(ar, targid, ce_ctrl_addr, n) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + DST_WR_INDEX_ADDRESS, (n))

#define CE_DEST_RING_WRITE_IDX_GET(targid, ce_ctrl_addr) \
	TARGET_READ((targid), (ce_ctrl_addr) + DST_WR_INDEX_ADDRESS)

#define CE_DEST_RING_READ_IDX_GET(targid, ce_ctrl_addr) \
	TARGET_READ((targid), (ce_ctrl_addr) + CURRENT_DRRI_ADDRESS)

#define CE_DEST_RING_BASE_ADDR_SET(ar, targid, ce_ctrl_addr, addr) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + DR_BA_ADDRESS, (addr))

#define CE_DEST_RING_SZ_SET(ar, targid, ce_ctrl_addr, n) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + DR_SIZE_ADDRESS, (n))

#define CE_SRC_RING_HIGHMARK_SET(ar, targid, ce_ctrl_addr, n) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + SRC_WATERMARK_ADDRESS, \
	(TARGET_READ((targid), (ce_ctrl_addr) + SRC_WATERMARK_ADDRESS) & \
		~SRC_WATERMARK_HIGH_MASK) | SRC_WATERMARK_HIGH_SET(n))

#define CE_SRC_RING_LOWMARK_SET(ar, targid, ce_ctrl_addr, n) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + SRC_WATERMARK_ADDRESS, \
	(TARGET_READ((targid), (ce_ctrl_addr) + SRC_WATERMARK_ADDRESS) & \
		~SRC_WATERMARK_LOW_MASK) | SRC_WATERMARK_LOW_SET(n))

#define CE_DEST_RING_HIGHMARK_SET(ar, targid, ce_ctrl_addr, n) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + DST_WATERMARK_ADDRESS, \
	(TARGET_READ((targid), (ce_ctrl_addr) + DST_WATERMARK_ADDRESS) & \
		~DST_WATERMARK_HIGH_MASK) | DST_WATERMARK_HIGH_SET(n))

#define CE_DEST_RING_LOWMARK_SET(ar, targid, ce_ctrl_addr, n) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + DST_WATERMARK_ADDRESS, \
	(TARGET_READ((targid), (ce_ctrl_addr) + DST_WATERMARK_ADDRESS) & \
		~DST_WATERMARK_LOW_MASK) | DST_WATERMARK_LOW_SET(n))

#define CE_COPY_COMPLETE_INTR_ENABLE(ar, targid, ce_ctrl_addr) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + HOST_IE_ADDRESS, \
	TARGET_READ((targid), (ce_ctrl_addr) + HOST_IE_ADDRESS) | \
		HOST_IE_COPY_COMPLETE_MASK)

#define CE_COPY_COMPLETE_INTR_DISABLE(ar, targid, ce_ctrl_addr) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + HOST_IE_ADDRESS, \
	TARGET_READ((targid), (ce_ctrl_addr) + HOST_IE_ADDRESS) & \
		~HOST_IE_COPY_COMPLETE_MASK)

#define CE_WATERMARK_INTR_ENABLE(ar, targid, ce_ctrl_addr) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + HOST_IE_ADDRESS, \
	TARGET_READ((targid), (ce_ctrl_addr) + HOST_IE_ADDRESS) | \
		CE_WATERMARK_MASK)

#define CE_WATERMARK_INTR_DISABLE(ar, targid, ce_ctrl_addr) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + HOST_IE_ADDRESS, \
	TARGET_READ((targid), (ce_ctrl_addr) + HOST_IE_ADDRESS) & \
		~CE_WATERMARK_MASK)

#define CE_ERROR_INTR_ENABLE(ar, targid, ce_ctrl_addr) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr) + MISC_IE_ADDRESS, \
	TARGET_READ((targid), (ce_ctrl_addr) + MISC_IE_ADDRESS) | \
		CE_ERROR_MASK)

#define CE_ENGINE_INT_STATUS_GET(targid, ce_ctrl_addr) \
	TARGET_READ((targid), (ce_ctrl_addr)+HOST_IS_ADDRESS)

#define CE_ENGINE_INT_STATUS_CLEAR(ar, targid, ce_ctrl_addr, mask) \
	TARGET_WRITE((ar), (targid), (ce_ctrl_addr)+HOST_IS_ADDRESS, (mask))

#define CE_WATERMARK_MASK (HOST_IS_SRC_RING_LOW_WATERMARK_MASK  | \
			   HOST_IS_SRC_RING_HIGH_WATERMARK_MASK | \
			   HOST_IS_DST_RING_LOW_WATERMARK_MASK  | \
			   HOST_IS_DST_RING_HIGH_WATERMARK_MASK)

#define CE_ERROR_MASK	(MISC_IS_AXI_ERR_MASK           | \
			 MISC_IS_DST_ADDR_ERR_MASK      | \
			 MISC_IS_SRC_LEN_ERR_MASK       | \
			 MISC_IS_DST_MAX_LEN_VIO_MASK   | \
			 MISC_IS_DST_RING_OVERFLOW_MASK | \
			 MISC_IS_SRC_RING_OVERFLOW_MASK)

#define CE_SRC_RING_TO_DESC(baddr, idx) \
	(&(((struct ce_desc *)baddr)[idx]))

#define CE_DEST_RING_TO_DESC(baddr, idx) \
	(&(((struct ce_desc *)baddr)[idx]))

/* Ring arithmetic (modulus number of entries in ring, which is a pwr of 2). */
#define CE_RING_DELTA(nentries_mask, fromidx, toidx) \
	(((int)(toidx)-(int)(fromidx)) & (nentries_mask))

#define CE_RING_IDX_INCR(nentries_mask, idx) (((idx) + 1) & (nentries_mask))

#define CE_WRAPPER_INTERRUPT_SUMMARY_HOST_MSI_LSB		8
#define CE_WRAPPER_INTERRUPT_SUMMARY_HOST_MSI_MASK		0x0000ff00
#define CE_WRAPPER_INTERRUPT_SUMMARY_HOST_MSI_GET(x) \
	(((x) & CE_WRAPPER_INTERRUPT_SUMMARY_HOST_MSI_MASK) >> \
		CE_WRAPPER_INTERRUPT_SUMMARY_HOST_MSI_LSB)
#define CE_WRAPPER_INTERRUPT_SUMMARY_ADDRESS			0x0000

#define CE_INTERRUPT_SUMMARY(ar, targid) \
	CE_WRAPPER_INTERRUPT_SUMMARY_HOST_MSI_GET( \
		TARGET_READ((targid), CE_WRAPPER_BASE_ADDRESS_T((ar)) + \
		CE_WRAPPER_INTERRUPT_SUMMARY_ADDRESS))

#endif /* _CE_H_ */
