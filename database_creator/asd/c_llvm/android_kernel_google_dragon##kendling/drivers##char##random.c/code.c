





#include <linux/utsname.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/poll.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/interrupt.h>
#include <linux/mm.h>
#include <linux/spinlock.h>
#include <linux/kthread.h>
#include <linux/percpu.h>
#include <linux/cryptohash.h>
#include <linux/fips.h>
#include <linux/ptrace.h>
#include <linux/kmemcheck.h>
#include <linux/workqueue.h>
#include <linux/irq.h>
#include <linux/syscalls.h>
#include <linux/completion.h>

#include <asm/processor.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/irq_regs.h>
#include <asm/io.h>

#define CREATE_TRACE_POINTS
#include <trace/events/random.h>






#define INPUT_POOL_SHIFT	12
#define INPUT_POOL_WORDS	(1 << (INPUT_POOL_SHIFT-5))
#define OUTPUT_POOL_SHIFT	10
#define OUTPUT_POOL_WORDS	(1 << (OUTPUT_POOL_SHIFT-5))
#define SEC_XFER_SIZE		512
#define EXTRACT_SIZE		10

#define DEBUG_RANDOM_BOOT 0

#define LONGS(x) (((x) + sizeof(unsigned long) - 1)/sizeof(unsigned long))



#define ENTROPY_SHIFT 3
#define ENTROPY_BITS(r) ((r)->entropy_count >> ENTROPY_SHIFT)



static int random_read_wakeup_bits = 64;



static int random_write_wakeup_bits = 28 * OUTPUT_POOL_WORDS;



static int random_min_urandom_seed = 60;



static struct poolinfo {
	int poolbitshift, poolwords, poolbytes, poolbits, poolfracbits;
#define S(x) ilog2(x)+5, (x), (x)*4, (x)*32, (x) << (ENTROPY_SHIFT+5)
	int tap1, tap2, tap3, tap4, tap5;
} poolinfo_table[] = {
	

	

	{ S(128),	104,	76,	51,	25,	1 },
	

	

	{ S(32),	26,	19,	14,	7,	1 },
#if 0
	

	{ S(2048),	1638,	1231,	819,	411,	1 },

	

	{ S(1024),	817,	615,	412,	204,	1 },

	

	{ S(1024),	819,	616,	410,	207,	2 },

	

	{ S(512),	411,	308,	208,	104,	1 },

	

	{ S(512),	409,	307,	206,	102,	2 },
	

	{ S(512),	409,	309,	205,	103,	2 },

	

	{ S(256),	205,	155,	101,	52,	1 },

	

	{ S(128),	103,	78,	51,	27,	2 },

	

	{ S(64),	52,	39,	26,	14,	1 },
#endif
};



static DECLARE_WAIT_QUEUE_HEAD(random_read_wait);
static DECLARE_WAIT_QUEUE_HEAD(random_write_wait);
static DECLARE_WAIT_QUEUE_HEAD(urandom_init_wait);
static struct fasync_struct *fasync;




struct entropy_store;
struct entropy_store {
	

	const struct poolinfo *poolinfo;
	__u32 *pool;
	const char *name;
	struct entropy_store *pull;
	struct work_struct push_work;

	

	unsigned long last_pulled;
	spinlock_t lock;
	unsigned short add_ptr;
	unsigned short input_rotate;
	int entropy_count;
	int entropy_total;
	unsigned int initialized:1;
	unsigned int limit:1;
	unsigned int last_data_init:1;
	__u8 last_data[EXTRACT_SIZE];
};

static void push_to_pool(struct work_struct *work);
static __u32 input_pool_data[INPUT_POOL_WORDS];
static __u32 blocking_pool_data[OUTPUT_POOL_WORDS];
static __u32 nonblocking_pool_data[OUTPUT_POOL_WORDS];

static struct entropy_store input_pool = {
	.poolinfo = &poolinfo_table[0],
	.name = "input",
	.limit = 1,
	.lock = __SPIN_LOCK_UNLOCKED(input_pool.lock),
	.pool = input_pool_data
};

static struct entropy_store blocking_pool = {
	.poolinfo = &poolinfo_table[1],
	.name = "blocking",
	.limit = 1,
	.pull = &input_pool,
	.lock = __SPIN_LOCK_UNLOCKED(blocking_pool.lock),
	.pool = blocking_pool_data,
	.push_work = __WORK_INITIALIZER(blocking_pool.push_work,
					push_to_pool),
};

static struct entropy_store nonblocking_pool = {
	.poolinfo = &poolinfo_table[1],
	.name = "nonblocking",
	.pull = &input_pool,
	.lock = __SPIN_LOCK_UNLOCKED(nonblocking_pool.lock),
	.pool = nonblocking_pool_data,
	.push_work = __WORK_INITIALIZER(nonblocking_pool.push_work,
					push_to_pool),
};

static __u32 const twist_table[8] = {
	0x00000000, 0x3b6e20c8, 0x76dc4190, 0x4db26158,
	0xedb88320, 0xd6d6a3e8, 0x9b64c2b0, 0xa00ae278 };



static void _mix_pool_bytes(struct entropy_store *r, const void *in,
			    int nbytes)
{
	unsigned long i, tap1, tap2, tap3, tap4, tap5;
	int input_rotate;
	int wordmask = r->poolinfo->poolwords - 1;
	const char *bytes = in;
	__u32 w;

	tap1 = r->poolinfo->tap1;
	tap2 = r->poolinfo->tap2;
	tap3 = r->poolinfo->tap3;
	tap4 = r->poolinfo->tap4;
	tap5 = r->poolinfo->tap5;

	input_rotate = r->input_rotate;
	i = r->add_ptr;

	

	while (nbytes--) {
		w = rol32(*bytes++, input_rotate);
		i = (i - 1) & wordmask;

		

		w ^= r->pool[i];
		w ^= r->pool[(i + tap1) & wordmask];
		w ^= r->pool[(i + tap2) & wordmask];
		w ^= r->pool[(i + tap3) & wordmask];
		w ^= r->pool[(i + tap4) & wordmask];
		w ^= r->pool[(i + tap5) & wordmask];

		

		r->pool[i] = (w >> 3) ^ twist_table[w & 7];

		

		input_rotate = (input_rotate + (i ? 7 : 14)) & 31;
	}

	r->input_rotate = input_rotate;
	r->add_ptr = i;
}

static void __mix_pool_bytes(struct entropy_store *r, const void *in,
			     int nbytes)
{
	trace_mix_pool_bytes_nolock(r->name, nbytes, _RET_IP_);
	_mix_pool_bytes(r, in, nbytes);
}

static void mix_pool_bytes(struct entropy_store *r, const void *in,
			   int nbytes)
{
	unsigned long flags;

	trace_mix_pool_bytes(r->name, nbytes, _RET_IP_);
	spin_lock_irqsave(&r->lock, flags);
	_mix_pool_bytes(r, in, nbytes);
	spin_unlock_irqrestore(&r->lock, flags);
}

struct fast_pool {
	__u32		pool[4];
	unsigned long	last;
	unsigned short	reg_idx;
	unsigned char	count;
};



static void fast_mix(struct fast_pool *f)
{
	__u32 a = f->pool[0],	b = f->pool[1];
	__u32 c = f->pool[2],	d = f->pool[3];

	a += b;			c += d;
	b = rol32(a, 6);	d = rol32(c, 27);
	d ^= a;			b ^= c;

	a += b;			c += d;
	b = rol32(a, 16);	d = rol32(c, 14);
	d ^= a;			b ^= c;

	a += b;			c += d;
	b = rol32(a, 6);	d = rol32(c, 27);
	d ^= a;			b ^= c;

	a += b;			c += d;
	b = rol32(a, 16);	d = rol32(c, 14);
	d ^= a;			b ^= c;

	f->pool[0] = a;  f->pool[1] = b;
	f->pool[2] = c;  f->pool[3] = d;
	f->count++;
}



static void credit_entropy_bits(struct entropy_store *r, int nbits)
{
	int entropy_count, orig;
	const int pool_size = r->poolinfo->poolfracbits;
	int nfrac = nbits << ENTROPY_SHIFT;

	if (!nbits)
		return;

retry:
	entropy_count = orig = ACCESS_ONCE(r->entropy_count);
	if (nfrac < 0) {
		

		entropy_count += nfrac;
	} else {
		

		int pnfrac = nfrac;
		const int s = r->poolinfo->poolbitshift + ENTROPY_SHIFT + 2;
		


		do {
			unsigned int anfrac = min(pnfrac, pool_size/2);
			unsigned int add =
				((pool_size - entropy_count)*anfrac*3) >> s;

			entropy_count += add;
			pnfrac -= anfrac;
		} while (unlikely(entropy_count < pool_size-2 && pnfrac));
	}

	if (unlikely(entropy_count < 0)) {
		pr_warn("random: negative entropy/overflow: pool %s count %d\n",
			r->name, entropy_count);
		WARN_ON(1);
		entropy_count = 0;
	} else if (entropy_count > pool_size)
		entropy_count = pool_size;
	if (cmpxchg(&r->entropy_count, orig, entropy_count) != orig)
		goto retry;

	r->entropy_total += nbits;
	if (!r->initialized && r->entropy_total > 128) {
		r->initialized = 1;
		r->entropy_total = 0;
		if (r == &nonblocking_pool) {
			prandom_reseed_late();
			wake_up_interruptible(&urandom_init_wait);
			pr_notice("random: %s pool is initialized\n", r->name);
		}
	}

	trace_credit_entropy_bits(r->name, nbits,
				  entropy_count >> ENTROPY_SHIFT,
				  r->entropy_total, _RET_IP_);

	if (r == &input_pool) {
		int entropy_bits = entropy_count >> ENTROPY_SHIFT;

		

		if (entropy_bits >= random_read_wakeup_bits) {
			wake_up_interruptible(&random_read_wait);
			kill_fasync(&fasync, SIGIO, POLL_IN);
		}
		

		if (entropy_bits > random_write_wakeup_bits &&
		    r->initialized &&
		    r->entropy_total >= 2*random_read_wakeup_bits) {
			static struct entropy_store *last = &blocking_pool;
			struct entropy_store *other = &blocking_pool;

			if (last == &blocking_pool)
				other = &nonblocking_pool;
			if (other->entropy_count <=
			    3 * other->poolinfo->poolfracbits / 4)
				last = other;
			if (last->entropy_count <=
			    3 * last->poolinfo->poolfracbits / 4) {
				schedule_work(&last->push_work);
				r->entropy_total = 0;
			}
		}
	}
}

static void credit_entropy_bits_safe(struct entropy_store *r, int nbits)
{
	const int nbits_max = (int)(~0U >> (ENTROPY_SHIFT + 1));

	

	nbits = min(nbits,  nbits_max);
	nbits = max(nbits, -nbits_max);

	credit_entropy_bits(r, nbits);
}






struct timer_rand_state {
	cycles_t last_time;
	long last_delta, last_delta2;
	unsigned dont_count_entropy:1;
};

#define INIT_TIMER_RAND_STATE { INITIAL_JIFFIES, };



void add_device_randomness(const void *buf, unsigned int size)
{
	unsigned long time = random_get_entropy() ^ jiffies;
	unsigned long flags;

	trace_add_device_randomness(size, _RET_IP_);
	spin_lock_irqsave(&input_pool.lock, flags);
	_mix_pool_bytes(&input_pool, buf, size);
	_mix_pool_bytes(&input_pool, &time, sizeof(time));
	spin_unlock_irqrestore(&input_pool.lock, flags);

	spin_lock_irqsave(&nonblocking_pool.lock, flags);
	_mix_pool_bytes(&nonblocking_pool, buf, size);
	_mix_pool_bytes(&nonblocking_pool, &time, sizeof(time));
	spin_unlock_irqrestore(&nonblocking_pool.lock, flags);
}
EXPORT_SYMBOL(add_device_randomness);

static struct timer_rand_state input_timer_state = INIT_TIMER_RAND_STATE;



static void add_timer_randomness(struct timer_rand_state *state, unsigned num)
{
	struct entropy_store	*r;
	struct {
		long jiffies;
		unsigned cycles;
		unsigned num;
	} sample;
	long delta, delta2, delta3;

	preempt_disable();

	sample.jiffies = jiffies;
	sample.cycles = random_get_entropy();
	sample.num = num;
	r = nonblocking_pool.initialized ? &input_pool : &nonblocking_pool;
	mix_pool_bytes(r, &sample, sizeof(sample));

	


	if (!state->dont_count_entropy) {
		delta = sample.jiffies - state->last_time;
		state->last_time = sample.jiffies;

		delta2 = delta - state->last_delta;
		state->last_delta = delta;

		delta3 = delta2 - state->last_delta2;
		state->last_delta2 = delta2;

		if (delta < 0)
			delta = -delta;
		if (delta2 < 0)
			delta2 = -delta2;
		if (delta3 < 0)
			delta3 = -delta3;
		if (delta > delta2)
			delta = delta2;
		if (delta > delta3)
			delta = delta3;

		

		credit_entropy_bits(r, min_t(int, fls(delta>>1), 11));
	}
	preempt_enable();
}

void add_input_randomness(unsigned int type, unsigned int code,
				 unsigned int value)
{
	static unsigned char last_value;

	

	if (value == last_value)
		return;

	last_value = value;
	add_timer_randomness(&input_timer_state,
			     (type << 4) ^ code ^ (code >> 4) ^ value);
	trace_add_input_randomness(ENTROPY_BITS(&input_pool));
}
EXPORT_SYMBOL_GPL(add_input_randomness);

static DEFINE_PER_CPU(struct fast_pool, irq_randomness);

#ifdef ADD_INTERRUPT_BENCH
static unsigned long avg_cycles, avg_deviation;

#define AVG_SHIFT 8     

#define FIXED_1_2 (1 << (AVG_SHIFT-1))

static void add_interrupt_bench(cycles_t start)
{
        long delta = random_get_entropy() - start;

        

        delta = delta - ((avg_cycles + FIXED_1_2) >> AVG_SHIFT);
        avg_cycles += delta;
        

        delta = abs(delta) - ((avg_deviation + FIXED_1_2) >> AVG_SHIFT);
        avg_deviation += delta;
}
#else
#define add_interrupt_bench(x)
#endif

static __u32 get_reg(struct fast_pool *f, struct pt_regs *regs)
{
	__u32 *ptr = (__u32 *) regs;

	if (regs == NULL)
		return 0;
	if (f->reg_idx >= sizeof(struct pt_regs) / sizeof(__u32))
		f->reg_idx = 0;
	return *(ptr + f->reg_idx++);
}

void add_interrupt_randomness(int irq, int irq_flags)
{
	struct entropy_store	*r;
	struct fast_pool	*fast_pool = this_cpu_ptr(&irq_randomness);
	struct pt_regs		*regs = get_irq_regs();
	unsigned long		now = jiffies;
	cycles_t		cycles = random_get_entropy();
	__u32			c_high, j_high;
	__u64			ip;
	unsigned long		seed;
	int			credit = 0;

	if (cycles == 0)
		cycles = get_reg(fast_pool, regs);
	c_high = (sizeof(cycles) > 4) ? cycles >> 32 : 0;
	j_high = (sizeof(now) > 4) ? now >> 32 : 0;
	fast_pool->pool[0] ^= cycles ^ j_high ^ irq;
	fast_pool->pool[1] ^= now ^ c_high;
	ip = regs ? instruction_pointer(regs) : _RET_IP_;
	fast_pool->pool[2] ^= ip;
	fast_pool->pool[3] ^= (sizeof(ip) > 4) ? ip >> 32 :
		get_reg(fast_pool, regs);

	fast_mix(fast_pool);
	add_interrupt_bench(cycles);

	if ((fast_pool->count < 64) &&
	    !time_after(now, fast_pool->last + HZ))
		return;

	r = nonblocking_pool.initialized ? &input_pool : &nonblocking_pool;
	if (!spin_trylock(&r->lock))
		return;

	fast_pool->last = now;
	__mix_pool_bytes(r, &fast_pool->pool, sizeof(fast_pool->pool));

	

	if (arch_get_random_seed_long(&seed)) {
		__mix_pool_bytes(r, &seed, sizeof(seed));
		credit = 1;
	}
	spin_unlock(&r->lock);

	fast_pool->count = 0;

	

	credit_entropy_bits(r, credit + 1);
}

#ifdef CONFIG_BLOCK
void add_disk_randomness(struct gendisk *disk)
{
	if (!disk || !disk->random)
		return;
	

	add_timer_randomness(disk->random, 0x100 + disk_devt(disk));
	trace_add_disk_randomness(disk_devt(disk), ENTROPY_BITS(&input_pool));
}
EXPORT_SYMBOL_GPL(add_disk_randomness);
#endif




static ssize_t extract_entropy(struct entropy_store *r, void *buf,
			       size_t nbytes, int min, int rsvd);



static void _xfer_secondary_pool(struct entropy_store *r, size_t nbytes);
static void xfer_secondary_pool(struct entropy_store *r, size_t nbytes)
{
	if (!r->pull ||
	    r->entropy_count >= (nbytes << (ENTROPY_SHIFT + 3)) ||
	    r->entropy_count > r->poolinfo->poolfracbits)
		return;

	if (r->limit == 0 && random_min_urandom_seed) {
		unsigned long now = jiffies;

		if (time_before(now,
				r->last_pulled + random_min_urandom_seed * HZ))
			return;
		r->last_pulled = now;
	}

	_xfer_secondary_pool(r, nbytes);
}

static void _xfer_secondary_pool(struct entropy_store *r, size_t nbytes)
{
	__u32	tmp[OUTPUT_POOL_WORDS];

	

	int rsvd_bytes = r->limit ? 0 : random_read_wakeup_bits / 4;
	int bytes = nbytes;

	

	bytes = max_t(int, bytes, random_read_wakeup_bits / 8);
	

	bytes = min_t(int, bytes, sizeof(tmp));

	trace_xfer_secondary_pool(r->name, bytes * 8, nbytes * 8,
				  ENTROPY_BITS(r), ENTROPY_BITS(r->pull));
	bytes = extract_entropy(r->pull, tmp, bytes,
				random_read_wakeup_bits / 8, rsvd_bytes);
	mix_pool_bytes(r, tmp, bytes);
	credit_entropy_bits(r, bytes*8);
}



static void push_to_pool(struct work_struct *work)
{
	struct entropy_store *r = container_of(work, struct entropy_store,
					      push_work);
	BUG_ON(!r);
	_xfer_secondary_pool(r, random_read_wakeup_bits/8);
	trace_push_to_pool(r->name, r->entropy_count >> ENTROPY_SHIFT,
			   r->pull->entropy_count >> ENTROPY_SHIFT);
}



static size_t account(struct entropy_store *r, size_t nbytes, int min,
		      int reserved)
{
	int entropy_count, orig;
	size_t ibytes, nfrac;

	BUG_ON(r->entropy_count > r->poolinfo->poolfracbits);

	

retry:
	entropy_count = orig = ACCESS_ONCE(r->entropy_count);
	ibytes = nbytes;
	

	if (r->limit) {
		int have_bytes = entropy_count >> (ENTROPY_SHIFT + 3);

		if ((have_bytes -= reserved) < 0)
			have_bytes = 0;
		ibytes = min_t(size_t, ibytes, have_bytes);
	}
	if (ibytes < min)
		ibytes = 0;

	if (unlikely(entropy_count < 0)) {
		pr_warn("random: negative entropy count: pool %s count %d\n",
			r->name, entropy_count);
		WARN_ON(1);
		entropy_count = 0;
	}
	nfrac = ibytes << (ENTROPY_SHIFT + 3);
	if ((size_t) entropy_count > nfrac)
		entropy_count -= nfrac;
	else
		entropy_count = 0;

	if (cmpxchg(&r->entropy_count, orig, entropy_count) != orig)
		goto retry;

	trace_debit_entropy(r->name, 8 * ibytes);
	if (ibytes &&
	    (r->entropy_count >> ENTROPY_SHIFT) < random_write_wakeup_bits) {
		wake_up_interruptible(&random_write_wait);
		kill_fasync(&fasync, SIGIO, POLL_OUT);
	}

	return ibytes;
}



static void extract_buf(struct entropy_store *r, __u8 *out)
{
	int i;
	union {
		__u32 w[5];
		unsigned long l[LONGS(20)];
	} hash;
	__u32 workspace[SHA_WORKSPACE_WORDS];
	unsigned long flags;

	

	sha_init(hash.w);
	for (i = 0; i < LONGS(20); i++) {
		unsigned long v;
		if (!arch_get_random_long(&v))
			break;
		hash.l[i] = v;
	}

	

	spin_lock_irqsave(&r->lock, flags);
	for (i = 0; i < r->poolinfo->poolwords; i += 16)
		sha_transform(hash.w, (__u8 *)(r->pool + i), workspace);

	

	__mix_pool_bytes(r, hash.w, sizeof(hash.w));
	spin_unlock_irqrestore(&r->lock, flags);

	memzero_explicit(workspace, sizeof(workspace));

	

	hash.w[0] ^= hash.w[3];
	hash.w[1] ^= hash.w[4];
	hash.w[2] ^= rol32(hash.w[2], 16);

	memcpy(out, &hash, EXTRACT_SIZE);
	memzero_explicit(&hash, sizeof(hash));
}



static ssize_t extract_entropy(struct entropy_store *r, void *buf,
				 size_t nbytes, int min, int reserved)
{
	ssize_t ret = 0, i;
	__u8 tmp[EXTRACT_SIZE];
	unsigned long flags;

	

	if (fips_enabled) {
		spin_lock_irqsave(&r->lock, flags);
		if (!r->last_data_init) {
			r->last_data_init = 1;
			spin_unlock_irqrestore(&r->lock, flags);
			trace_extract_entropy(r->name, EXTRACT_SIZE,
					      ENTROPY_BITS(r), _RET_IP_);
			xfer_secondary_pool(r, EXTRACT_SIZE);
			extract_buf(r, tmp);
			spin_lock_irqsave(&r->lock, flags);
			memcpy(r->last_data, tmp, EXTRACT_SIZE);
		}
		spin_unlock_irqrestore(&r->lock, flags);
	}

	trace_extract_entropy(r->name, nbytes, ENTROPY_BITS(r), _RET_IP_);
	xfer_secondary_pool(r, nbytes);
	nbytes = account(r, nbytes, min, reserved);

	while (nbytes) {
		extract_buf(r, tmp);

		if (fips_enabled) {
			spin_lock_irqsave(&r->lock, flags);
			if (!memcmp(tmp, r->last_data, EXTRACT_SIZE))
				panic("Hardware RNG duplicated output!\n");
			memcpy(r->last_data, tmp, EXTRACT_SIZE);
			spin_unlock_irqrestore(&r->lock, flags);
		}
		i = min_t(int, nbytes, EXTRACT_SIZE);
		memcpy(buf, tmp, i);
		nbytes -= i;
		buf += i;
		ret += i;
	}

	

	memzero_explicit(tmp, sizeof(tmp));

	return ret;
}



static ssize_t extract_entropy_user(struct entropy_store *r, void __user *buf,
				    size_t nbytes)
{
	ssize_t ret = 0, i;
	__u8 tmp[EXTRACT_SIZE];
	int large_request = (nbytes > 256);

	trace_extract_entropy_user(r->name, nbytes, ENTROPY_BITS(r), _RET_IP_);
	xfer_secondary_pool(r, nbytes);
	nbytes = account(r, nbytes, 0, 0);

	while (nbytes) {
		if (large_request && need_resched()) {
			if (signal_pending(current)) {
				if (ret == 0)
					ret = -ERESTARTSYS;
				break;
			}
			schedule();
		}

		extract_buf(r, tmp);
		i = min_t(int, nbytes, EXTRACT_SIZE);
		if (copy_to_user(buf, tmp, i)) {
			ret = -EFAULT;
			break;
		}

		nbytes -= i;
		buf += i;
		ret += i;
	}

	

	memzero_explicit(tmp, sizeof(tmp));

	return ret;
}



void get_random_bytes(void *buf, int nbytes)
{
#if DEBUG_RANDOM_BOOT > 0
	if (unlikely(nonblocking_pool.initialized == 0))
		printk(KERN_NOTICE "random: %pF get_random_bytes called "
		       "with %d bits of entropy available\n",
		       (void *) _RET_IP_,
		       nonblocking_pool.entropy_total);
#endif
	trace_get_random_bytes(nbytes, _RET_IP_);
	extract_entropy(&nonblocking_pool, buf, nbytes, 0, 0);
}
EXPORT_SYMBOL(get_random_bytes);



void get_random_bytes_arch(void *buf, int nbytes)
{
	char *p = buf;

	trace_get_random_bytes_arch(nbytes, _RET_IP_);
	while (nbytes) {
		unsigned long v;
		int chunk = min(nbytes, (int)sizeof(unsigned long));

		if (!arch_get_random_long(&v))
			break;
		
		memcpy(p, &v, chunk);
		p += chunk;
		nbytes -= chunk;
	}

	if (nbytes)
		extract_entropy(&nonblocking_pool, p, nbytes, 0, 0);
}
EXPORT_SYMBOL(get_random_bytes_arch);




static void init_std_data(struct entropy_store *r)
{
	int i;
	ktime_t now = ktime_get_real();
	unsigned long rv;

	r->last_pulled = jiffies;
	mix_pool_bytes(r, &now, sizeof(now));
	for (i = r->poolinfo->poolbytes; i > 0; i -= sizeof(rv)) {
		if (!arch_get_random_seed_long(&rv) &&
		    !arch_get_random_long(&rv))
			rv = random_get_entropy();
		mix_pool_bytes(r, &rv, sizeof(rv));
	}
	mix_pool_bytes(r, utsname(), sizeof(*(utsname())));
}



static int rand_initialize(void)
{
	init_std_data(&input_pool);
	init_std_data(&blocking_pool);
	init_std_data(&nonblocking_pool);
	return 0;
}
early_initcall(rand_initialize);

#ifdef CONFIG_BLOCK
void rand_initialize_disk(struct gendisk *disk)
{
	struct timer_rand_state *state;

	

	state = kzalloc(sizeof(struct timer_rand_state), GFP_KERNEL);
	if (state) {
		state->last_time = INITIAL_JIFFIES;
		disk->random = state;
	}
}
#endif

static ssize_t
_random_read(int nonblock, char __user *buf, size_t nbytes)
{
	ssize_t n;

	if (nbytes == 0)
		return 0;

	nbytes = min_t(size_t, nbytes, SEC_XFER_SIZE);
	while (1) {
		n = extract_entropy_user(&blocking_pool, buf, nbytes);
		if (n < 0)
			return n;
		trace_random_read(n*8, (nbytes-n)*8,
				  ENTROPY_BITS(&blocking_pool),
				  ENTROPY_BITS(&input_pool));
		if (n > 0)
			return n;

		

		if (nonblock)
			return -EAGAIN;

		wait_event_interruptible(random_read_wait,
			ENTROPY_BITS(&input_pool) >=
			random_read_wakeup_bits);
		if (signal_pending(current))
			return -ERESTARTSYS;
	}
}

static ssize_t
random_read(struct file *file, char __user *buf, size_t nbytes, loff_t *ppos)
{
	return _random_read(file->f_flags & O_NONBLOCK, buf, nbytes);
}

static ssize_t
urandom_read(struct file *file, char __user *buf, size_t nbytes, loff_t *ppos)
{
	int ret;

	if (unlikely(nonblocking_pool.initialized == 0))
		printk_once(KERN_NOTICE "random: %s urandom read "
			    "with %d bits of entropy available\n",
			    current->comm, nonblocking_pool.entropy_total);

	nbytes = min_t(size_t, nbytes, INT_MAX >> (ENTROPY_SHIFT + 3));
	ret = extract_entropy_user(&nonblocking_pool, buf, nbytes);

	trace_urandom_read(8 * nbytes, ENTROPY_BITS(&nonblocking_pool),
			   ENTROPY_BITS(&input_pool));
	return ret;
}

static unsigned int
random_poll(struct file *file, poll_table * wait)
{
	unsigned int mask;

	poll_wait(file, &random_read_wait, wait);
	poll_wait(file, &random_write_wait, wait);
	mask = 0;
	if (ENTROPY_BITS(&input_pool) >= random_read_wakeup_bits)
		mask |= POLLIN | POLLRDNORM;
	if (ENTROPY_BITS(&input_pool) < random_write_wakeup_bits)
		mask |= POLLOUT | POLLWRNORM;
	return mask;
}

static int
write_pool(struct entropy_store *r, const char __user *buffer, size_t count)
{
	size_t bytes;
	__u32 buf[16];
	const char __user *p = buffer;

	while (count > 0) {
		bytes = min(count, sizeof(buf));
		if (copy_from_user(&buf, p, bytes))
			return -EFAULT;

		count -= bytes;
		p += bytes;

		mix_pool_bytes(r, buf, bytes);
		cond_resched();
	}

	return 0;
}

static ssize_t random_write(struct file *file, const char __user *buffer,
			    size_t count, loff_t *ppos)
{
	size_t ret;

	ret = write_pool(&blocking_pool, buffer, count);
	if (ret)
		return ret;
	ret = write_pool(&nonblocking_pool, buffer, count);
	if (ret)
		return ret;

	return (ssize_t)count;
}

static long random_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	int size, ent_count;
	int __user *p = (int __user *)arg;
	int retval;

	switch (cmd) {
	case RNDGETENTCNT:
		

		ent_count = ENTROPY_BITS(&input_pool);
		if (put_user(ent_count, p))
			return -EFAULT;
		return 0;
	case RNDADDTOENTCNT:
		if (!capable(CAP_SYS_ADMIN))
			return -EPERM;
		if (get_user(ent_count, p))
			return -EFAULT;
		credit_entropy_bits_safe(&input_pool, ent_count);
		return 0;
	case RNDADDENTROPY:
		if (!capable(CAP_SYS_ADMIN))
			return -EPERM;
		if (get_user(ent_count, p++))
			return -EFAULT;
		if (ent_count < 0)
			return -EINVAL;
		if (get_user(size, p++))
			return -EFAULT;
		retval = write_pool(&input_pool, (const char __user *)p,
				    size);
		if (retval < 0)
			return retval;
		credit_entropy_bits_safe(&input_pool, ent_count);
		return 0;
	case RNDZAPENTCNT:
	case RNDCLEARPOOL:
		

		if (!capable(CAP_SYS_ADMIN))
			return -EPERM;
		input_pool.entropy_count = 0;
		nonblocking_pool.entropy_count = 0;
		blocking_pool.entropy_count = 0;
		return 0;
	default:
		return -EINVAL;
	}
}

static int random_fasync(int fd, struct file *filp, int on)
{
	return fasync_helper(fd, filp, on, &fasync);
}

const struct file_operations random_fops = {
	.read  = random_read,
	.write = random_write,
	.poll  = random_poll,
	.unlocked_ioctl = random_ioctl,
	.fasync = random_fasync,
	.llseek = noop_llseek,
};

const struct file_operations urandom_fops = {
	.read  = urandom_read,
	.write = random_write,
	.unlocked_ioctl = random_ioctl,
	.fasync = random_fasync,
	.llseek = noop_llseek,
};

SYSCALL_DEFINE3(getrandom, char __user *, buf, size_t, count,
		unsigned int, flags)
{
	if (flags & ~(GRND_NONBLOCK|GRND_RANDOM))
		return -EINVAL;

	if (count > INT_MAX)
		count = INT_MAX;

	if (flags & GRND_RANDOM)
		return _random_read(flags & GRND_NONBLOCK, buf, count);

	if (unlikely(nonblocking_pool.initialized == 0)) {
		if (flags & GRND_NONBLOCK)
			return -EAGAIN;
		wait_event_interruptible(urandom_init_wait,
					 nonblocking_pool.initialized);
		if (signal_pending(current))
			return -ERESTARTSYS;
	}
	return urandom_read(NULL, buf, count, NULL);
}






void generate_random_uuid(unsigned char uuid_out[16])
{
	get_random_bytes(uuid_out, 16);
	

	uuid_out[6] = (uuid_out[6] & 0x0F) | 0x40;
	

	uuid_out[8] = (uuid_out[8] & 0x3F) | 0x80;
}
EXPORT_SYMBOL(generate_random_uuid);




#ifdef CONFIG_SYSCTL

#include <linux/sysctl.h>

static int min_read_thresh = 8, min_write_thresh;
static int max_read_thresh = OUTPUT_POOL_WORDS * 32;
static int max_write_thresh = INPUT_POOL_WORDS * 32;
static char sysctl_bootid[16];



static int proc_do_uuid(struct ctl_table *table, int write,
			void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct ctl_table fake_table;
	unsigned char buf[64], tmp_uuid[16], *uuid;

	uuid = table->data;
	if (!uuid) {
		uuid = tmp_uuid;
		generate_random_uuid(uuid);
	} else {
		static DEFINE_SPINLOCK(bootid_spinlock);

		spin_lock(&bootid_spinlock);
		if (!uuid[8])
			generate_random_uuid(uuid);
		spin_unlock(&bootid_spinlock);
	}

	sprintf(buf, "%pU", uuid);

	fake_table.data = buf;
	fake_table.maxlen = sizeof(buf);

	return proc_dostring(&fake_table, write, buffer, lenp, ppos);
}



static int proc_do_entropy(struct ctl_table *table, int write,
			   void __user *buffer, size_t *lenp, loff_t *ppos)
{
	struct ctl_table fake_table;
	int entropy_count;

	entropy_count = *(int *)table->data >> ENTROPY_SHIFT;

	fake_table.data = &entropy_count;
	fake_table.maxlen = sizeof(entropy_count);

	return proc_dointvec(&fake_table, write, buffer, lenp, ppos);
}

static int sysctl_poolsize = INPUT_POOL_WORDS * 32;
extern struct ctl_table random_table[];
struct ctl_table random_table[] = {
	{
		.procname	= "poolsize",
		.data		= &sysctl_poolsize,
		.maxlen		= sizeof(int),
		.mode		= 0444,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "entropy_avail",
		.maxlen		= sizeof(int),
		.mode		= 0444,
		.proc_handler	= proc_do_entropy,
		.data		= &input_pool.entropy_count,
	},
	{
		.procname	= "read_wakeup_threshold",
		.data		= &random_read_wakeup_bits,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &min_read_thresh,
		.extra2		= &max_read_thresh,
	},
	{
		.procname	= "write_wakeup_threshold",
		.data		= &random_write_wakeup_bits,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= &min_write_thresh,
		.extra2		= &max_write_thresh,
	},
	{
		.procname	= "urandom_min_reseed_secs",
		.data		= &random_min_urandom_seed,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "boot_id",
		.data		= &sysctl_bootid,
		.maxlen		= 16,
		.mode		= 0444,
		.proc_handler	= proc_do_uuid,
	},
	{
		.procname	= "uuid",
		.maxlen		= 16,
		.mode		= 0444,
		.proc_handler	= proc_do_uuid,
	},
#ifdef ADD_INTERRUPT_BENCH
	{
		.procname	= "add_interrupt_avg_cycles",
		.data		= &avg_cycles,
		.maxlen		= sizeof(avg_cycles),
		.mode		= 0444,
		.proc_handler	= proc_doulongvec_minmax,
	},
	{
		.procname	= "add_interrupt_avg_deviation",
		.data		= &avg_deviation,
		.maxlen		= sizeof(avg_deviation),
		.mode		= 0444,
		.proc_handler	= proc_doulongvec_minmax,
	},
#endif
	{ }
};
#endif 	


static u32 random_int_secret[MD5_MESSAGE_BYTES / 4] ____cacheline_aligned;

int random_int_secret_init(void)
{
	get_random_bytes(random_int_secret, sizeof(random_int_secret));
	return 0;
}



static DEFINE_PER_CPU(__u32 [MD5_DIGEST_WORDS], get_random_int_hash);
unsigned int get_random_int(void)
{
	__u32 *hash;
	unsigned int ret;

	if (arch_get_random_int(&ret))
		return ret;

	hash = get_cpu_var(get_random_int_hash);

	hash[0] += current->pid + jiffies + random_get_entropy();
	md5_transform(hash, random_int_secret);
	ret = hash[0];
	put_cpu_var(get_random_int_hash);

	return ret;
}
EXPORT_SYMBOL(get_random_int);



unsigned long get_random_long(void)
{
	__u32 *hash;
	unsigned long ret;

	if (arch_get_random_long(&ret))
		return ret;

	hash = get_cpu_var(get_random_int_hash);

	hash[0] += current->pid + jiffies + random_get_entropy();
	md5_transform(hash, random_int_secret);
	ret = *(unsigned long *)hash;
	put_cpu_var(get_random_int_hash);

	return ret;
}
EXPORT_SYMBOL(get_random_long);



unsigned long
randomize_range(unsigned long start, unsigned long end, unsigned long len)
{
	unsigned long range = end - len - start;

	if (end <= start + len)
		return 0;
	return PAGE_ALIGN(get_random_int() % range + start);
}



void add_hwgenerator_randomness(const char *buffer, size_t count,
				size_t entropy)
{
	struct entropy_store *poolp = &input_pool;

	

	wait_event_interruptible(random_write_wait, kthread_should_stop() ||
			ENTROPY_BITS(&input_pool) <= random_write_wakeup_bits);
	mix_pool_bytes(poolp, buffer, count);
	credit_entropy_bits(poolp, entropy);
}
EXPORT_SYMBOL_GPL(add_hwgenerator_randomness);
