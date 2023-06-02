#include <linux/mm.h>
#include <linux/module.h>
#include <linux/gfp.h>
#include <linux/kernel_stat.h>
#include <linux/swap.h>
#include <linux/pagemap.h>
#include <linux/init.h>
#include <linux/highmem.h>
#include <linux/vmpressure.h>
#include <linux/vmstat.h>
#include <linux/file.h>
#include <linux/writeback.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>	

#include <linux/mm_inline.h>
#include <linux/backing-dev.h>
#include <linux/rmap.h>
#include <linux/topology.h>
#include <linux/cpu.h>
#include <linux/cpuset.h>
#include <linux/compaction.h>
#include <linux/notifier.h>
#include <linux/rwsem.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/freezer.h>
#include <linux/memcontrol.h>
#include <linux/delayacct.h>
#include <linux/sysctl.h>
#include <linux/oom.h>
#include <linux/prefetch.h>
#include <linux/debugfs.h>

#include <asm/tlbflush.h>
#include <asm/div64.h>

#include <linux/swapops.h>
#include <linux/balloon_compaction.h>

#include STR

#define var_58
#include <trace/events/vmscan.h>

struct scan_control {
	

	unsigned long field_57;

	

	unsigned long field_26;

	

	unsigned long field_45;

	unsigned long field_55;

	

	gfp_t field_1;

	int field_53;

	

	int field_32;

	

	int field_36;

	int field_8;

	

	int field_20;

	

	struct mem_cgroup *field_3;

	

	nodemask_t	*field_4;
};

#define func_167(var_118) (list_entry((_head)->prev, struct page, lru))

#ifdef var_105
#define var_284(var_89, var_24, var_115)			\
	do {								\
		if ((_page)->lru.prev != _base) {			\
			struct page *prev;				\
									\
			prev = lru_to_page(&(_page->lru));		\
			prefetch(&prev->_field);			\
		}							\
	} while (NUM)
#else
#define var_284(var_89, var_24, var_115) do { } while (NUM)
#endif

#ifdef var_309
#define func_274(var_89, var_24, var_115)			\
	do {								\
		if ((_page)->lru.prev != _base) {			\
			struct page *prev;				\
									\
			prev = lru_to_page(&(_page->lru));		\
			prefetchw(&prev->_field);			\
		}							\
	} while (NUM)
#else
#define func_274(var_89, var_24, var_115) do { } while (NUM)
#endif



int var_110 = NUM;
unsigned long var_130;	


#ifdef var_142
char *var_20 = var_142;
#else
char *var_20 = NULL;
#endif

static LIST_HEAD(var_122);
static DECLARE_RWSEM(var_262);

#ifdef var_267
static bool func_309(struct scan_control *var_74)
{
	return !var_74->field_3;
}
#else
static bool func_309(struct scan_control *var_74)
{
	return true;
}
#endif

static unsigned long func_118(struct zone *field_39)
{
	int arr_0;

	arr_0 = func_19(field_39, var_214) +
	     func_19(field_39, var_296);

	if (func_220() > NUM)
		arr_0 += func_19(field_39, var_75) +
		      func_19(field_39, var_36);

	return arr_0;
}

bool func_8(struct zone *field_39)
{
	return field_39->field_30 < func_118(field_39) * NUM;
}

static unsigned long func_111(struct lruvec *var_331, enum lru_list field_23)
{
	if (!func_260())
		return func_176(var_331, field_23);

	return func_19(func_174(var_331), var_304 + field_23);
}

struct dentry *var_307;

static int func_65(struct seq_file *var_220, void *var_46)
{
	struct shrinker *var_84;
	struct shrink_control var_74;

	var_74.field_1 = NUM;
	var_74.field_43 = NUM;

	func_28(&var_262);
	func_291(var_84, &var_122, field_47) {
		int var_248;

		var_248 = var_84->field_29(var_84, &var_74);
		func_100(var_220, STR, var_84->field_29, var_248);
	}
	func_77(&var_262);
	return NUM;
}

static int func_75(struct inode *var_70, struct file *var_238)
{
        return func_238(var_238, func_65, var_70->field_19);
}

static const struct file_operations var_44 = {
        .field_14 = func_75,
        .field_25 = var_141,
        .field_13 = var_290,
        .field_15 = var_327,
};



void func_94(struct shrinker *var_84)
{
	func_101(&var_84->field_58, NUM);
	func_9(&var_262);
	func_289(&var_84->field_47, &var_122);
	func_283(&var_262);
}
func_237(func_94);

static int var_346 func_21(void)
{
	func_97(STR, NUM, NULL, NULL,
			    &var_44);
	return NUM;
}

func_275(func_21);



void func_262(struct shrinker *var_84)
{
	func_9(&var_262);
	func_52(&var_84->field_47);
	func_283(&var_262);
}
func_237(func_262);

static inline int func_147(struct shrinker *var_84,
				     struct shrink_control *var_74,
				     unsigned long field_43)
{
	var_74->field_43 = field_43;
	return (*var_84->field_29)(var_84, var_74);
}

#define var_140 NUM


unsigned long func_301(struct shrink_control *field_29,
			  unsigned long var_229,
			  unsigned long var_197)
{
	struct shrinker *var_84;
	unsigned long var_91 = NUM;

	if (var_229 == NUM)
		var_229 = var_149;

	if (!func_79(&var_262)) {
		

		var_91 = NUM;
		goto out;
	}

	func_291(var_84, &var_122, field_47) {
		unsigned long long var_232;
		long var_219;
		long var_313;
		int var_104 = NUM;
		long arr_0;
		long var_102;
		long var_3 = var_84->field_6 ? var_84->field_6
						  : var_140;
		long var_259 = var_3;

		if (func_186())
			var_259 = NUM;

		var_313 = func_147(var_84, field_29, NUM);
		if (var_313 <= NUM)
			continue;

		

		arr_0 = func_287(&var_84->field_58, NUM);

		var_219 = arr_0;
		var_232 = (NUM * var_229) / var_84->field_49;
		var_232 *= var_313;
		func_81(var_232, var_197 + NUM);
		var_219 += var_232;
		if (var_219 < NUM) {
			func_245(var_185 STR
			       STR,
			       var_84->field_29, var_219);
			var_219 = var_313;
		}

		

		if (var_232 < var_313 / NUM)
			var_219 = func_109(var_219, var_313 / NUM);

		

		if (var_219 > var_313 * NUM)
			var_219 = var_313 * NUM;

		func_217(var_84, field_29, arr_0,
					var_229, var_197,
					var_313, var_232, var_219);

		while (var_219 > var_259) {
			int var_93;

			if (var_219 < var_3)
				var_3 = var_219;

			var_93 = func_147(var_84, field_29, NUM);
			var_104 = func_147(var_84, field_29,
							var_3);
			if (var_104 == NUM)
				break;
			if (var_104 < var_93)
				var_91 += var_93 - var_104;
			func_158(var_254, var_3);
			var_219 -= var_3;

			func_76();
		}

		

		if (var_219 > NUM)
			var_102 = func_135(var_219,
					&var_84->field_58);
		else
			var_102 = func_116(&var_84->field_58);

		func_227(var_84, var_104, arr_0, var_102);
	}
	func_77(&var_262);
out:
	func_76();
	return var_91;
}

static inline int func_14(struct page *var_203)
{
	

	return func_171(var_203) - func_155(var_203) == NUM;
}

static int func_285(struct backing_dev_info *var_302,
			      struct scan_control *var_74)
{
	if (var_66->field_51 & var_322)
		return NUM;
	if (!func_42(var_302))
		return NUM;
	if (var_302 == var_66->field_22)
		return NUM;
	return NUM;
}



static void func_128(struct address_space *var_328,
				struct page *var_203, int var_49)
{
	func_234(var_203);
	if (func_2(var_203) == var_328)
		func_45(var_328, var_49);
	func_218(var_203);
}



typedef enum {
	

	var_0,
	

	var_318,
	

	var_252,
	

	var_301,
} pageout_t;



static pageout_t func_50(struct page *var_203, struct address_space *var_328,
			 struct scan_control *var_74)
{
	

	if (!func_14(var_203))
		return var_0;
	if (!var_328) {
		

		if (func_155(var_203)) {
			if (func_271(var_203)) {
				func_31(var_203);
				func_245(STR, var_315);
				return var_301;
			}
		}
		return var_0;
	}
	if (var_328->field_9->field_62 == NULL)
		return var_318;
	if (!func_285(var_328->field_22, var_74))
		return var_0;

	if (func_292(var_203)) {
		int var_278;
		struct writeback_control var_329 = {
			.field_5 = var_312,
			.field_21 = var_149,
			.field_42 = NUM,
			.field_37 = var_67,
			.field_16 = NUM,
		};

		func_178(var_203);
		var_278 = var_328->field_9->field_62(var_203, &var_329);
		if (var_278 < NUM)
			func_128(var_328, var_203, var_278);
		if (var_278 == var_280) {
			func_204(var_203);
			return var_318;
		}

		if (!func_270(var_203)) {
			

			func_204(var_203);
			if (func_139(var_203) && func_53(var_203)) {
				func_248(var_203);
				

				func_214(var_203);
				return var_318;
			}
		}
		func_254(var_203, func_39(var_203));
		func_226(var_203, var_222);
		return var_252;
	}

	return var_301;
}



static int func_58(struct address_space *var_328, struct page *var_203)
{
	func_202(!func_229(var_203));
	func_202(var_328 != func_2(var_203));

	func_67(&var_328->field_60);
	

	if (!func_87(var_203, NUM))
		goto cannot_free;
	

	if (func_172(func_203(var_203))) {
		func_107(var_203, NUM);
		goto cannot_free;
	}

	if (func_53(var_203)) {
		swp_entry_t var_63 = { .field_52 = func_264(var_203) };
		func_159(var_203);
		func_56(&var_328->field_60);
		func_199(var_63, var_203);
	} else {
		void (*field_7)(struct page *);

		field_7 = var_328->field_9->field_7;

		func_305(var_203);
		func_56(&var_328->field_60);
		func_191(var_203);

		if (field_7 != NULL)
			field_7(var_203);
	}

	return NUM;

cannot_free:
	func_56(&var_328->field_60);
	return NUM;
}



int func_59(struct address_space *var_328, struct page *var_203)
{
	if (func_58(var_328, var_203)) {
		

		func_107(var_203, NUM);
		return NUM;
	}
	return NUM;
}



void func_51(struct page *var_203)
{
	int field_23;
	int var_295 = !!func_290(var_203);
	int var_124 = func_106(var_203);

	func_288(func_121(var_203));

redo:
	func_160(var_203);

	if (func_146(var_203)) {
		

		field_23 = var_295 + func_261(var_203);
		func_221(var_203, field_23);
	} else {
		

		field_23 = var_168;
		func_99(var_203);
		

		func_78();
	}

	

	if (field_23 == var_168 && func_146(var_203)) {
		if (!func_27(var_203)) {
			func_74(var_203);
			goto redo;
		}
		

	}

	if (var_124 && field_23 != var_168)
		func_35(var_114);
	else if (!var_124 && field_23 == var_168)
		func_35(var_314);

	func_74(var_203);		

}

enum page_references {
	var_150,
	var_56,
	var_240,
	var_320,
};

static enum page_references func_143(struct page *var_203,
						  struct scan_control *var_74)
{
	int var_23, var_119;
	unsigned long var_228;

	var_23 = func_161(var_203, NUM, var_74->field_3,
					  &var_228);
	var_119 = func_123(var_203);

	

	if (var_228 & var_131)
		return var_150;

	if (var_23) {
		if (func_36(var_203))
			return var_320;
		

		func_162(var_203);

		if (var_119 || var_23 > NUM)
			return var_320;

		

		if (var_228 & var_241)
			return var_320;

		return var_240;
	}

	

	if (var_119 && !func_36(var_203))
		return var_56;

	return var_150;
}



static void func_278(struct page *var_203,
				       bool *var_272, bool *var_249)
{
	struct address_space *var_328;

	

	if (!func_83(var_203)) {
		*var_272 = false;
		*var_249 = false;
		return;
	}

	

	*var_272 = func_203(var_203);
	*var_249 = func_270(var_203);

	

	if (!func_155(var_203))
		return;

	var_328 = func_2(var_203);
	if (var_328 && var_328->field_9->field_41)
		var_328->field_9->field_41(var_203, var_272, var_249);
}



static unsigned long func_57(struct list_head *var_190,
				      struct zone *field_39,
				      struct scan_control *var_74,
				      enum ttu_flags var_2,
				      unsigned long *var_343,
				      unsigned long *var_223,
				      unsigned long *var_299,
				      unsigned long *var_255,
				      unsigned long *var_37,
				      bool var_57)
{
	func_43(var_158);
	func_43(var_135);
	int var_317 = NUM;
	unsigned long var_53 = NUM;
	unsigned long var_121 = NUM;
	unsigned long var_182 = NUM;
	unsigned long field_26 = NUM;
	unsigned long var_64 = NUM;
	unsigned long var_164 = NUM;

	func_76();

	func_85();
	while (!func_209(var_190)) {
		struct address_space *var_328;
		struct page *var_203;
		int var_133;
		enum page_references var_61 = var_56;
		bool var_272, var_249;

		func_76();

		var_203 = func_167(var_190);
		func_52(&var_203->field_23);

		if (!func_198(var_203))
			goto keep;

		func_288(func_231(var_203));
		func_288(func_233(var_203) != field_39);

		var_74->field_57++;

		if (func_172(!func_146(var_203)))
			goto cull_mlocked;

		if (!var_74->field_32 && func_23(var_203))
			goto keep_locked;

		

		if (func_23(var_203) || func_53(var_203))
			var_74->field_57++;

		var_133 = (var_74->field_1 & var_147) ||
			(func_53(var_203) && (var_74->field_1 & var_111));

		

		func_278(var_203, &var_272, &var_249);
		if (var_272 || var_249)
			var_121++;

		if (var_272 && !var_249)
			var_53++;

		

		var_328 = func_2(var_203);
		if ((var_328 && func_42(var_328->field_22)) ||
		    (var_249 && func_60(var_203)))
			var_182++;

		

		if (func_270(var_203)) {
			

			if (func_186() &&
			    func_60(var_203) &&
			    func_68(field_39)) {
				var_164++;
				goto keep_locked;

			

			} else if (func_309(var_74) ||
			    !func_60(var_203) || !(var_74->field_1 & var_111)) {
				

				func_178(var_203);
				var_64++;

				goto keep_locked;

			

			} else {
				func_235(var_203);
			}
		}

		if (!var_57)
			var_61 = func_143(var_203, var_74);

		switch (var_61) {
		case var_320:
			goto activate_locked;
		case var_240:
			goto keep_locked;
		case var_150:
		case var_56:
			; 

		}

		

		if (func_308(var_203) && !func_53(var_203)) {
			if (!(var_74->field_1 & var_111))
				goto keep_locked;
			if (!func_126(var_203, var_190))
				goto activate_locked;
			var_133 = NUM;

			

			var_328 = func_2(var_203);
		}

		

		if (func_23(var_203) && var_328) {
			switch (func_253(var_203, var_2)) {
			case var_154:
				goto activate_locked;
			case var_62:
				goto keep_locked;
			case var_59:
				goto cull_mlocked;
			case var_160:
				; 

			}
		}

		if (func_203(var_203)) {
			

			if (func_83(var_203) &&
					(!func_186() ||
					 !func_48(field_39))) {
				

				func_226(var_203, var_303);
				func_178(var_203);

				goto keep_locked;
			}

			if (var_61 == var_56)
				goto keep_locked;
			if (!var_133)
				goto keep_locked;
			if (!var_74->field_53)
				goto keep_locked;

			

			switch (func_50(var_203, var_328, var_74)) {
			case var_0:
				goto keep_locked;
			case var_318:
				goto activate_locked;
			case var_252:
				if (func_270(var_203))
					goto keep;
				if (func_203(var_203))
					goto keep;

				

				if (!func_198(var_203))
					goto keep;
				if (func_203(var_203) || func_270(var_203))
					goto keep_locked;
				var_328 = func_2(var_203);
			case var_301:
				; 

			}
		}

		

		if (func_155(var_203)) {
			if (!func_295(var_203, var_74->field_1))
				goto activate_locked;
			if (!var_328 && func_171(var_203) == NUM) {
				func_218(var_203);
				if (func_242(var_203))
					goto free_it;
				else {
					

					field_26++;
					continue;
				}
			}
		}

		if (!var_328 || !func_58(var_328, var_203))
			goto keep_locked;

		

		func_258(var_203);
free_it:
		field_26++;

		

		func_115(&var_203->field_23, &var_135);
		continue;

cull_mlocked:
		if (func_53(var_203))
			func_102(var_203);
		func_218(var_203);
		func_115(&var_203->field_23, &var_158);
		continue;

activate_locked:
		

		if (func_53(var_203) && func_63(func_136(var_203)))
			func_102(var_203);
		func_288(func_231(var_203));
		func_142(var_203);
		var_317++;
keep_locked:
		func_218(var_203);
keep:
		func_115(&var_203->field_23, &var_158);
		func_288(func_121(var_203) || func_106(var_203));
	}

	func_61(&var_135, NUM);

	func_18(&var_158, var_190);
	func_158(var_108, var_317);
	func_244();
	*var_343 += var_121;
	*var_299 += var_182;
	*var_223 += var_53;
	*var_255 += var_64;
	*var_37 += var_164;
	return field_26;
}

unsigned long func_124(struct zone *field_39,
					    struct list_head *var_190)
{
	struct scan_control var_74 = {
		.field_1 = var_1,
		.field_20 = var_298,
		.field_32 = NUM,
	};
	unsigned long var_91, var_116, var_253, var_139, var_287, var_300;
	struct page *var_203, *var_348;
	func_43(var_193);

	func_246(var_203, var_348, var_190, field_23) {
		if (func_83(var_203) && !func_203(var_203) &&
		    !func_243(var_203)) {
			func_105(var_203);
			func_141(&var_203->field_23, &var_193);
		}
	}

	var_91 = func_57(&var_193, field_39, &var_74,
			var_332|var_325,
			&var_116, &var_253, &var_139, &var_287, &var_300, true);
	func_18(&var_193, var_190);
	func_37(field_39, var_65, -var_91);
	return var_91;
}



int func_88(struct page *var_203, isolate_mode_t var_31)
{
	int var_91 = -var_310;

	

	if (!func_121(var_203))
		return var_91;

	

	if (func_106(var_203) && !(var_31 & var_239))
		return var_91;

	var_91 = -var_279;

	

	if (var_31 & (var_344|var_22)) {
		

		if (func_270(var_203))
			return var_91;

		if (func_203(var_203)) {
			struct address_space *var_328;

			

			if (var_31 & var_344)
				return var_91;

			

			var_328 = func_2(var_203);
			if (var_328 && !var_328->field_9->field_50)
				return var_91;
		}
	}

	if ((var_31 & var_106) && func_23(var_203))
		return var_91;

	if (func_279(func_133(var_203))) {
		

		func_249(var_203);
		var_91 = NUM;
	}

	return var_91;
}



static unsigned long func_293(unsigned long field_43,
		struct lruvec *var_331, struct list_head *var_96,
		unsigned long *field_57, struct scan_control *var_74,
		isolate_mode_t var_31, enum lru_list field_23)
{
	struct list_head *var_208 = &var_331->field_33[field_23];
	unsigned long var_195 = NUM;
	unsigned long var_145;

	for (var_145 = NUM; var_145 < field_43 && !func_209(var_208); var_145++) {
		struct page *var_203;
		int var_192;

		var_203 = func_167(var_208);
		func_274(var_203, var_208, field_51);

		func_288(!func_121(var_203));

		switch (func_88(var_203, var_31)) {
		case NUM:
			var_192 = func_152(var_203);
			func_33(var_331, field_23, -var_192);
			func_141(&var_203->field_23, var_96);
			var_195 += var_192;
			break;

		case -var_279:
			

			func_141(&var_203->field_23, var_208);
			continue;

		default:
			func_272();
		}
	}

	*field_57 = var_145;
	func_55(var_74->field_8, field_43, var_145,
				    var_195, var_31, func_184(field_23));
	return var_195;
}



int func_27(struct page *var_203)
{
	int var_91 = -var_279;

	func_288(!func_171(var_203));

	if (func_121(var_203)) {
		struct zone *field_39 = func_233(var_203);
		struct lruvec *var_331;

		func_67(&field_39->field_12);
		var_331 = func_175(var_203, field_39);
		if (func_121(var_203)) {
			int field_23 = func_7(var_203);
			func_182(var_203);
			func_249(var_203);
			func_201(var_203, var_331, field_23);
			var_91 = NUM;
		}
		func_56(&field_39->field_12);
	}
	return var_91;
}

static int func_127(struct zone *field_39, int var_238,
	struct scan_control *var_74, int var_275)
{
	unsigned long var_87, var_211;

	if (var_238) {
		if (var_275) {
			var_87 = func_247(field_39,
					var_296);
			var_211 = func_247(field_39,
					var_65);
		} else {
			var_87 = func_19(field_39, var_296);
			var_211 = func_19(field_39, var_65);
		}
	} else {
		if (var_275) {
			var_87 = func_247(field_39,
					var_36);
			var_211 = func_247(field_39,
					var_112);
		} else {
			var_87 = func_19(field_39, var_36);
			var_211 = func_19(field_39, var_112);
		}
	}

	

	if ((var_74->field_1 & var_237) == var_237)
		var_87 >>= NUM;

	return var_211 > var_87;
}



static int func_269(struct zone *field_39, int var_238,
		struct scan_control *var_74, int var_275)
{
	if (func_186())
		return NUM;

	if (!func_309(var_74))
		return NUM;

	if (func_172(func_127(field_39, var_238, var_74, NUM))) {
		if (var_275)
			return func_127(field_39, var_238, var_74, var_275);
		else
			return NUM;
	}

	return NUM;
}

static noinline_for_stack var_256
func_125(struct lruvec *var_331, struct list_head *var_190)
{
	struct zone_reclaim_stat *field_61 = &var_331->field_61;
	struct zone *field_39 = func_174(var_331);
	func_43(var_123);

	

	while (!func_209(var_190)) {
		struct page *var_203 = func_167(var_190);
		int field_23;

		func_288(func_121(var_203));
		func_52(&var_203->field_23);
		if (func_172(!func_146(var_203))) {
			func_56(&field_39->field_12);
			func_51(var_203);
			func_67(&field_39->field_12);
			continue;
		}

		var_331 = func_175(var_203, field_39);

		func_168(var_203);
		field_23 = func_7(var_203);
		func_16(var_203, var_331, field_23);

		if (func_157(field_23)) {
			int var_238 = func_184(field_23);
			int var_10 = func_152(var_203);
			field_61->field_31[var_238] += var_10;
		}
		if (func_242(var_203)) {
			func_193(var_203);
			func_41(var_203);
			func_201(var_203, var_331, field_23);

			if (func_172(func_296(var_203))) {
				func_56(&field_39->field_12);
				(*func_89(var_203))(var_203);
				func_67(&field_39->field_12);
			} else
				func_115(&var_203->field_23, &var_123);
		}
	}

	

	func_18(&var_123, var_190);
}



static noinline_for_stack var_333 long
func_20(unsigned long field_43, struct lruvec *var_331,
		     struct scan_control *var_74, enum lru_list field_23)
{
	func_43(var_190);
	unsigned long field_57;
	unsigned long field_26 = NUM;
	unsigned long var_195;
	unsigned long var_121 = NUM;
	unsigned long var_182 = NUM;
	unsigned long var_53 = NUM;
	unsigned long var_64 = NUM;
	unsigned long var_164 = NUM;
	isolate_mode_t var_191 = NUM;
	int var_238 = func_184(field_23);
	int var_275 = NUM;
	struct zone *field_39 = func_174(var_331);
	struct zone_reclaim_stat *field_61 = &var_331->field_61;

	while (func_172(func_269(field_39, var_238, var_74, var_275))) {
		func_5(var_155, var_7/NUM);

		

		if (func_69(var_66))
			return var_149;

		var_275 = NUM;
	}

	func_122();

	if (!var_74->field_32)
		var_191 |= var_106;
	if (!var_74->field_53)
		var_191 |= var_344;

	func_67(&field_39->field_12);

	var_195 = func_293(field_43, var_331, &var_190,
				     &field_57, var_74, var_191, field_23);

	func_37(field_39, var_304 + field_23, -var_195);
	func_37(field_39, var_112 + var_238, var_195);

	if (func_309(var_74)) {
		field_39->field_30 += field_57;
		if (func_186())
			func_29(var_201, field_39, field_57);
		else
			func_29(var_338, field_39, field_57);
	}
	func_56(&field_39->field_12);

	if (var_195 == NUM)
		return NUM;

	field_26 = func_57(&var_190, field_39, var_74, var_332,
				&var_121, &var_53, &var_182,
				&var_64, &var_164,
				false);

	func_67(&field_39->field_12);

	field_61->field_28[var_238] += var_195;

	if (func_309(var_74)) {
		if (func_186())
			func_29(var_347, field_39,
					       field_26);
		else
			func_29(var_138, field_39,
					       field_26);
	}

	func_125(var_331, &var_190);

	func_37(field_39, var_112 + var_238, -var_195);

	func_56(&field_39->field_12);

	func_61(&var_190, NUM);

	

	if (var_64 && var_64 == var_195)
		func_196(field_39, var_202);

	

	if (func_309(var_74)) {
		

		if (var_121 && var_121 == var_182)
			func_196(field_39, var_336);

		

		if (var_53 == var_195)
			func_196(field_39, var_209);

		

		if (var_164)
			func_5(var_155, var_7/NUM);
	}

	

	if (!var_74->field_55 && !func_186())
		func_294(field_39, var_155, var_7/NUM);

	func_265(field_39->field_2->field_44,
		func_117(field_39),
		field_57, field_26,
		var_74->field_20,
		func_110(var_238));
	return field_26;
}




static void func_150(struct lruvec *var_331,
				     struct list_head *field_47,
				     struct list_head *var_123,
				     enum lru_list field_23)
{
	struct zone *field_39 = func_174(var_331);
	unsigned long var_68 = NUM;
	struct page *var_203;
	int var_192;

	while (!func_209(field_47)) {
		var_203 = func_167(field_47);
		var_331 = func_175(var_203, field_39);

		func_288(func_121(var_203));
		func_168(var_203);

		var_192 = func_152(var_203);
		func_33(var_331, field_23, var_192);
		func_141(&var_203->field_23, &var_331->field_33[field_23]);
		var_68 += var_192;

		if (func_242(var_203)) {
			func_193(var_203);
			func_41(var_203);
			func_201(var_203, var_331, field_23);

			if (func_172(func_296(var_203))) {
				func_56(&field_39->field_12);
				(*func_89(var_203))(var_203);
				func_67(&field_39->field_12);
			} else
				func_115(&var_203->field_23, var_123);
		}
	}
	func_37(field_39, var_304 + field_23, var_68);
	if (!func_157(field_23))
		func_298(var_13, var_68);
}

static void func_40(unsigned long field_43,
			       struct lruvec *var_331,
			       struct scan_control *var_74,
			       enum lru_list field_23)
{
	unsigned long var_195;
	unsigned long field_57;
	unsigned long var_228;
	func_43(var_55);	

	func_43(var_78);
	func_43(var_269);
	struct page *var_203;
	struct zone_reclaim_stat *field_61 = &var_331->field_61;
	unsigned long var_95 = NUM;
	isolate_mode_t var_191 = NUM;
	int var_238 = func_184(field_23);
	struct zone *field_39 = func_174(var_331);

	func_122();

	if (!var_74->field_32)
		var_191 |= var_106;
	if (!var_74->field_53)
		var_191 |= var_344;

	func_67(&field_39->field_12);

	var_195 = func_293(field_43, var_331, &var_55,
				     &field_57, var_74, var_191, field_23);
	if (func_309(var_74))
		field_39->field_30 += field_57;

	field_61->field_28[var_238] += var_195;

	func_29(var_236, field_39, field_57);
	func_37(field_39, var_304 + field_23, -var_195);
	func_37(field_39, var_112 + var_238, var_195);
	func_56(&field_39->field_12);

	while (!func_209(&var_55)) {
		func_76();
		var_203 = func_167(&var_55);
		func_52(&var_203->field_23);

		if (func_172(!func_146(var_203))) {
			func_51(var_203);
			continue;
		}

		if (func_172(var_245)) {
			if (func_155(var_203) && func_198(var_203)) {
				if (func_155(var_203))
					func_295(var_203, NUM);
				func_218(var_203);
			}
		}

		if (func_161(var_203, NUM, var_74->field_3,
				    &var_228)) {
			var_95 += func_152(var_203);
			

			if ((var_228 & var_241) && func_83(var_203)) {
				func_115(&var_203->field_23, &var_78);
				continue;
			}
		}

		func_105(var_203);	

		func_115(&var_203->field_23, &var_269);
	}

	

	func_67(&field_39->field_12);
	

	field_61->field_31[var_238] += var_95;

	func_150(var_331, &var_78, &var_55, field_23);
	func_150(var_331, &var_269, &var_55, field_23 - var_35);
	func_37(field_39, var_112 + var_238, -var_195);
	func_56(&field_39->field_12);

	func_61(&var_55, NUM);
}

#ifdef var_167
static int func_164(struct zone *field_39)
{
	unsigned long var_295, var_87;

	var_295 = func_19(field_39, var_75);
	var_87 = func_19(field_39, var_36);

	if (var_87 * field_39->field_56 < var_295)
		return NUM;

	return NUM;
}



static int func_240(struct lruvec *var_331)
{
	

	if (!var_100)
		return NUM;

	if (!func_260())
		return func_282(var_331);

	return func_164(func_174(var_331));
}
#else
static inline int func_240(struct lruvec *var_331)
{
	return NUM;
}
#endif



static int func_166(struct lruvec *var_331)
{
	unsigned long var_87;
	unsigned long var_295;

	var_87 = func_111(var_331, var_216);
	var_295 = func_111(var_331, var_148);

	return var_295 > var_87;
}

static int func_104(struct lruvec *var_331, enum lru_list field_23)
{
	if (func_184(field_23))
		return func_166(var_331);
	else
		return func_240(var_331);
}

static unsigned long func_24(enum lru_list field_23, unsigned long field_43,
				 struct lruvec *var_331, struct scan_control *var_74)
{
	if (func_157(field_23)) {
		if (func_104(var_331, field_23))
			func_40(field_43, var_331, var_74, field_23);
		return NUM;
	}

	return func_20(field_43, var_331, var_74, field_23);
}

static int func_26(struct scan_control *var_74)
{
	if (func_309(var_74))
		return var_110;
	return func_169(var_74->field_3);
}

enum scan_balance {
	var_165,
	var_289,
	var_265,
	var_5,
};



static void func_276(struct lruvec *var_331, struct scan_control *var_74,
			   unsigned long *arr_0)
{
	struct zone_reclaim_stat *field_61 = &var_331->field_61;
	u64 arr_3[NUM];
	u64 var_14 = NUM;	

	struct zone *field_39 = func_174(var_331);
	unsigned long var_60, var_151;
	enum scan_balance var_77;
	unsigned long var_113, var_238, var_29;
	bool var_85 = false;
	unsigned long var_126, var_97;
	enum lru_list field_23;

	

	if (func_186() && !func_8(field_39))
		var_85 = true;
	if (!func_309(var_74))
		var_85 = true;

	

	if (!var_74->field_36 || (func_220() <= NUM)) {
		var_77 = var_5;
		goto out;
	}

	

	if (!func_309(var_74) && !func_26(var_74)) {
		var_77 = var_5;
		goto out;
	}

	

	if (!var_74->field_20 && func_26(var_74)) {
		var_77 = var_165;
		goto out;
	}

	var_113  = func_111(var_331, var_71) +
		func_111(var_331, var_125);
	var_238  = func_111(var_331, var_148) +
		func_111(var_331, var_216);

	

	if (func_309(var_74)) {
		var_29 = func_19(field_39, var_21);
		if (func_172(var_238 + var_29 <= func_192(field_39))) {
			var_77 = var_265;
			goto out;
		}
	}

	

	if (!func_120(var_38) &&
			!func_166(var_331)) {
		var_77 = var_5;
		goto out;
	}

	var_77 = var_289;

	

	var_60 = func_26(var_74);
	var_151 = NUM - var_60;

	

	func_67(&field_39->field_12);
	if (func_172(field_61->field_28[NUM] > var_113 / NUM)) {
		field_61->field_28[NUM] /= NUM;
		field_61->field_31[NUM] /= NUM;
	}

	if (func_172(field_61->field_28[NUM] > var_238 / NUM)) {
		field_61->field_28[NUM] /= NUM;
		field_61->field_31[NUM] /= NUM;
	}

	

	var_126 = var_60 * (field_61->field_28[NUM] + NUM);
	var_126 /= field_61->field_31[NUM] + NUM;

	var_97 = var_151 * (field_61->field_28[NUM] + NUM);
	var_97 /= field_61->field_31[NUM] + NUM;
	func_56(&field_39->field_12);

	arr_3[NUM] = var_126;
	arr_3[NUM] = var_97;
	var_14 = var_126 + var_97 + NUM;
out:
	func_181(field_23) {
		int var_238 = func_184(field_23);
		unsigned long var_294;
		unsigned long var_145;

		var_294 = func_111(var_331, field_23);
		var_145 = var_294 >> var_74->field_20;

		if (!var_145 && var_85)
			var_145 = func_109(var_294, var_149);

		switch (var_77) {
		case var_165:
			

			break;
		case var_289:
			

			var_145 = func_225(var_145 * arr_3[var_238], var_14);
			break;
		case var_5:
		case var_265:
			

			if ((var_77 == var_5) != var_238)
				var_145 = NUM;
			break;
		default:
			

			func_272();
		}
		arr_0[field_23] = var_145;
	}
}



static void func_223(struct lruvec *var_331, struct scan_control *var_74)
{
	unsigned long arr_0[arr_1];
	unsigned long arr_2[arr_1];
	unsigned long field_43;
	enum lru_list field_23;
	unsigned long field_26 = NUM;
	unsigned long field_45 = var_74->field_45;
	struct blk_plug var_146;
	bool var_227;

	func_276(var_331, var_74, arr_0);

	

	func_200(arr_2, arr_0, sizeof(arr_0));

	

	var_227 = (func_309(var_74) && !func_186() &&
			 var_74->field_20 == var_298);

	func_211(&var_146);
	while (arr_0[var_125] || arr_0[var_148] ||
					arr_0[var_216]) {
		unsigned long var_286, var_244, var_6;
		unsigned long field_57;

		for_each_evictable_lru(field_23) {
			if (arr_0[field_23]) {
				field_43 = func_109(arr_0[field_23], var_149);
				arr_0[field_23] -= field_43;

				field_26 += func_24(field_23, field_43,
							    var_331, var_74);
			}
		}

		if (field_26 < field_45 || var_227)
			continue;

		

		var_244 = arr_0[var_216] + arr_0[var_148];
		var_286 = arr_0[var_125] + arr_0[var_71];

		

		if (!var_244 || !var_286)
			break;

		if (var_244 > var_286) {
			unsigned long var_33 = arr_2[var_125] +
						arr_2[var_71] + NUM;
			field_23 = var_79;
			var_6 = var_286 * NUM / var_33;
		} else {
			unsigned long var_33 = arr_2[var_216] +
						arr_2[var_148] + NUM;
			field_23 = var_156;
			var_6 = var_244 * NUM / var_33;
		}

		

		arr_0[field_23] = NUM;
		arr_0[field_23 + var_35] = NUM;

		

		field_23 = (field_23 == var_156) ? var_79 : var_156;
		field_57 = arr_2[field_23] - arr_0[field_23];
		arr_0[field_23] = arr_2[field_23] * (NUM - var_6) / NUM;
		arr_0[field_23] -= func_109(arr_0[field_23], field_57);

		field_23 += var_35;
		field_57 = arr_2[field_23] - arr_0[field_23];
		arr_0[field_23] = arr_2[field_23] * (NUM - var_6) / NUM;
		arr_0[field_23] -= func_109(arr_0[field_23], field_57);

		var_227 = true;
	}
	func_25(&var_146);
	var_74->field_26 += field_26;

	

	if (func_240(var_331))
		func_40(var_149, var_331,
				   var_74, var_71);

	func_1(var_74->field_1);
}



static bool func_71(struct scan_control *var_74)
{
	if (func_120(var_161) && var_74->field_8 &&
			(var_74->field_8 > var_187 ||
			 var_74->field_20 < var_298 - NUM))
		return true;

	return false;
}



static inline bool func_82(struct zone *field_39,
					unsigned long field_26,
					unsigned long field_57,
					struct scan_control *var_74)
{
	unsigned long var_39;
	unsigned long var_86;

	

	if (!func_71(var_74))
		return false;

	

	if (var_74->field_1 & var_48) {
		

		if (!field_26 && !field_57)
			return false;
	} else {
		

		if (!field_26)
			return false;
	}

	

	var_39 = (2UL << var_74->field_8);
	var_86 = func_19(field_39, var_296);
	if (func_220() > NUM)
		var_86 += func_19(field_39, var_36);
	if (var_74->field_26 < var_39 &&
			var_86 > var_39)
		return true;

	

	switch (func_10(field_39, var_74->field_8)) {
	case var_41:
	case var_261:
		return false;
	default:
		return true;
	}
}

static void func_95(struct zone *field_39, struct scan_control *var_74)
{
	unsigned long field_26, field_57;

	do {
		struct mem_cgroup *var_72 = var_74->field_3;
		struct mem_cgroup_reclaim_cookie var_281 = {
			.field_39 = field_39,
			.field_20 = var_74->field_20,
		};
		struct mem_cgroup *var_319;

		field_26 = var_74->field_26;
		field_57 = var_74->field_57;

		var_319 = func_15(var_72, NULL, &var_281);
		do {
			struct lruvec *var_331;

			var_331 = func_108(field_39, var_319);

			func_223(var_331, var_74);

			

			if (!func_309(var_74) &&
					var_74->field_26 >= var_74->field_45) {
				func_302(var_72, var_319);
				break;
			}
			var_319 = func_15(var_72, var_319, &var_281);
		} while (var_319);

		func_303(var_74->field_1, var_74->field_3,
			   var_74->field_57 - field_57,
			   var_74->field_26 - field_26);

	} while (func_82(field_39, var_74->field_26 - field_26,
					 var_74->field_57 - field_57, var_74));
}



static inline bool func_206(struct zone *field_39, struct scan_control *var_74)
{
	unsigned long var_69, var_242;
	bool var_263;

	

	if (var_74->field_8 <= var_187)
		return false;

	

	var_69 = func_109(func_137(field_39),
		(field_39->field_11 + var_233NUM) /
			var_233);
	var_242 = func_192(field_39) + var_69 + (2UL << var_74->field_8);
	var_263 = func_257(field_39, NUM, var_242, NUM, NUM);

	

	if (func_93(field_39, var_74->field_8))
		return var_263;

	

	if (!func_10(field_39, var_74->field_8))
		return false;

	return var_263;
}



static bool func_0(struct zonelist *var_9, struct scan_control *var_74)
{
	struct zoneref *var_271;
	struct zone *field_39;
	unsigned long var_181;
	unsigned long var_251;
	bool var_205 = false;

	

	if (var_245)
		var_74->field_1 |= var_159;

	func_134(field_39, var_271, var_9,
					func_194(var_74->field_1), var_74->field_4) {
		if (!func_300(field_39))
			continue;
		

		if (func_309(var_74)) {
			if (!func_170(field_39, var_1))
				continue;
			if (var_74->field_20 != var_298 &&
			    !func_8(field_39))
				continue;	

			if (func_120(var_161)) {
				

				if (func_206(field_39, var_74)) {
					var_205 = true;
					continue;
				}
			}
			

			var_251 = NUM;
			var_181 = func_156(field_39,
						var_74->field_8, var_74->field_1,
						&var_251);
			var_74->field_26 += var_181;
			var_74->field_57 += var_251;
			

		}

		func_95(field_39, var_74);
	}

	return var_205;
}



static bool func_86(struct zonelist *var_9,
		struct scan_control *var_74)
{
	struct zoneref *var_271;
	struct zone *field_39;

	func_134(field_39, var_271, var_9,
			func_194(var_74->field_1), var_74->field_4) {
		if (!func_300(field_39))
			continue;
		if (!func_170(field_39, var_1))
			continue;
		if (func_8(field_39))
			return false;
	}

	return true;
}



static unsigned long func_286(struct zonelist *var_9,
					struct scan_control *var_74,
					struct shrink_control *field_29)
{
	unsigned long var_90 = NUM;
	struct reclaim_state *field_0 = var_66->field_0;
	struct zoneref *var_271;
	struct zone *field_39;
	unsigned long var_260;
	bool var_205;

	func_205();

	if (func_309(var_74))
		func_35(var_337);

	do {
		func_259(var_74->field_1, var_74->field_3,
				var_74->field_20);
		var_74->field_57 = NUM;
		var_205 = func_0(var_9, var_74);

		

		if (func_309(var_74)) {
			unsigned long var_197 = NUM;
			func_129(field_39, var_271, var_9,
					func_194(var_74->field_1)) {
				if (!func_170(field_39, var_1))
					continue;

				var_197 += func_118(field_39);
			}

			func_301(field_29, var_74->field_57, var_197);
			if (field_0) {
				var_74->field_26 += field_0->field_27;
				field_0->field_27 = NUM;
			}
		}
		var_90 += var_74->field_57;
		if (var_74->field_26 >= var_74->field_45)
			goto out;

		

		if (var_74->field_20 < var_298 - NUM)
			var_74->field_53 = NUM;

		

		var_260 = var_74->field_45 + var_74->field_45 / NUM;
		if (var_90 > var_260) {
			func_149(var_230 ? NUM : var_90,
						var_206);
			var_74->field_53 = NUM;
		}
	} while (--var_74->field_20 >= NUM && !var_205);

out:
	func_232();

	if (var_74->field_26)
		return var_74->field_26;

	

	if (var_339)
		return NUM;

	

	if (var_205)
		return NUM;

	

	if (func_309(var_74) && !func_86(var_9, var_74))
		return NUM;

	return NUM;
}

static bool func_212(pg_data_t *var_184)
{
	struct zone *field_39;
	unsigned long var_212 = NUM;
	unsigned long var_135 = NUM;
	int var_30;
	bool var_247;

	for (var_30 = NUM; var_30 <= var_217; var_30++) {
		field_39 = &var_184->field_59[var_30];
		if (!func_300(field_39))
			continue;

		var_212 += func_98(field_39);
		var_135 += func_19(field_39, var_21);
	}

	

	if (!var_212)
		return true;

	var_247 = var_135 > var_212 / NUM;

	

	if (!var_247 && func_197(&var_184->field_17)) {
		var_184->field_24 = func_109(var_184->field_24,
						(enum zone_type)var_217);
		func_230(&var_184->field_17);
	}

	return var_247;
}



static bool func_277(gfp_t field_1, struct zonelist *var_9,
					nodemask_t *field_4)
{
	struct zoneref *var_271;
	struct zone *field_39;
	pg_data_t *var_184 = NULL;

	

	if (var_66->field_51 & var_43)
		goto out;

	

	if (func_69(var_66))
		goto out;

	

	func_134(field_39, var_271, var_9,
					field_1, field_4) {
		if (func_117(field_39) > var_217)
			continue;

		

		var_184 = field_39->field_2;
		if (func_212(var_184))
			goto out;
		break;
	}

	

	if (!var_184)
		goto out;

	

	func_35(var_54);

	

	if (!(field_1 & var_147)) {
		func_177(var_184->field_18,
			func_212(var_184), var_7);

		goto check_pending;
	}

	

	func_49(field_39->field_2->field_18,
		func_212(var_184));

check_pending:
	if (func_69(var_66))
		return true;

out:
	return false;
}

unsigned long func_187(struct zonelist *var_9, int field_8,
				gfp_t field_1, nodemask_t *field_4)
{
	unsigned long field_26;
	struct scan_control var_74 = {
		.field_1 = (field_1 = func_255(field_1)),
		.field_53 = !var_230,
		.field_45 = var_149,
		.field_32 = NUM,
		.field_36 = NUM,
		.field_8 = field_8,
		.field_20 = var_298,
		.field_3 = NULL,
		.field_4 = field_4,
	};
	struct shrink_control field_29 = {
		.field_1 = var_74.field_1,
	};

	

	if (func_277(field_1, var_9, field_4))
		return NUM;

	func_44(field_8,
				var_74.field_53,
				field_1);

	field_26 = func_286(var_9, &var_74, &field_29);

	func_132(field_26);

	return field_26;
}

#ifdef var_267

unsigned long func_72(struct mem_cgroup *var_319,
						gfp_t field_1, bool var_94,
						struct zone *field_39,
						unsigned long *field_57)
{
	struct scan_control var_74 = {
		.field_57 = NUM,
		.field_45 = var_149,
		.field_53 = !var_230,
		.field_32 = NUM,
		.field_36 = !var_94,
		.field_8 = NUM,
		.field_20 = NUM,
		.field_3 = var_319,
	};
	struct lruvec *var_331 = func_108(field_39, var_319);

	var_74.field_1 = (field_1 & var_234) |
			(var_183 & ~var_234);

	func_173(var_74.field_8,
						      var_74.field_53,
						      var_74.field_1);

	

	func_223(var_331, &var_74);

	func_281(var_74.field_26);

	*field_57 = var_74.field_57;
	return var_74.field_26;
}

unsigned long func_266(struct mem_cgroup *var_319,
					   gfp_t field_1,
					   bool var_94)
{
	struct zonelist *var_9;
	unsigned long field_26;
	int var_283;
	struct scan_control var_74 = {
		.field_53 = !var_230,
		.field_32 = NUM,
		.field_36 = !var_94,
		.field_45 = var_149,
		.field_8 = NUM,
		.field_20 = var_298,
		.field_3 = var_319,
		.field_4 = NULL, 

		.field_1 = (field_1 & var_234) |
				(var_183 & ~var_234),
	};
	struct shrink_control field_29 = {
		.field_1 = var_74.field_1,
	};

	

	var_283 = func_4(var_319);

	var_9 = func_280(var_283)->field_40;

	func_310(NUM,
					    var_74.field_53,
					    var_74.field_1);

	field_26 = func_286(var_9, &var_74, &field_29);

	func_188(field_26);

	return field_26;
}
#endif

static void func_216(struct zone *field_39, struct scan_control *var_74)
{
	struct mem_cgroup *var_319;

	if (!var_100)
		return;

	var_319 = func_15(NULL, NULL, NULL);
	do {
		struct lruvec *var_331 = func_108(field_39, var_319);

		if (func_240(var_331))
			func_40(var_149, var_331,
					   var_74, var_71);

		var_319 = func_15(NULL, var_319, NULL);
	} while (var_319);
}

static bool func_80(struct zone *field_39, int field_8,
			  unsigned long var_69, int field_24)
{
	if (!func_257(field_39, field_8, func_192(field_39) +
				    var_69, field_24, NUM))
		return false;

	if (func_120(var_161) && field_8 &&
	    !func_10(field_39, field_8))
		return false;

	return true;
}



static bool func_3(pg_data_t *var_184, int field_8, int field_24)
{
	unsigned long field_11 = NUM;
	unsigned long var_109 = NUM;
	int var_30;

	

	for (var_30 = NUM; var_30 <= field_24; var_30++) {
		struct zone *field_39 = var_184->field_59 + var_30;

		if (!func_300(field_39))
			continue;

		field_11 += field_39->field_11;

		

		if (!func_8(field_39)) {
			var_109 += field_39->field_11;
			continue;
		}

		if (func_80(field_39, field_8, NUM, var_30))
			var_109 += field_39->field_11;
		else if (!field_8)
			return false;
	}

	if (field_8)
		return var_109 >= (field_11 >> NUM);
	else
		return true;
}



static bool func_91(pg_data_t *var_184, int field_8, long var_330,
					int field_24)
{
	

	if (var_330)
		return false;

	

	if (func_197(&var_184->field_18))
		func_64(&var_184->field_18);

	return func_3(var_184, field_8, field_24);
}



static bool func_241(struct zone *field_39,
			       int field_24,
			       struct scan_control *var_74,
			       unsigned long var_197,
			       unsigned long *var_250)
{
	unsigned long var_82;
	int var_257 = var_74->field_8;
	unsigned long var_69;
	struct reclaim_state *field_0 = var_66->field_0;
	struct shrink_control field_29 = {
		.field_1 = var_74->field_1,
	};
	bool var_213;

	

	var_74->field_45 = func_210(var_149, func_192(field_39));

	

	if (func_120(var_161) && var_74->field_8 &&
			func_10(field_39, var_74->field_8) !=
				var_152)
		var_257 = NUM;

	

	var_69 = func_109(func_137(field_39),
		(field_39->field_11 + var_233NUM) /
		var_233);

	

	var_213 = (var_245 && func_256(field_39));
	if (!var_213 && func_80(field_39, var_257,
						var_69, field_24))
		return true;

	func_95(field_39, var_74);

	field_0->field_27 = NUM;
	var_82 = func_301(&field_29, var_74->field_57, var_197);
	var_74->field_26 += field_0->field_27;

	

	*var_250 += var_74->field_45;

	func_263(field_39, var_202);

	

	if (func_8(field_39) &&
	    func_80(field_39, var_257, NUM, field_24)) {
		func_263(field_39, var_336);
		func_263(field_39, var_209);
	}

	return var_74->field_57 >= var_74->field_45;
}



static unsigned long func_307(pg_data_t *var_184, int field_8,
							int *field_24)
{
	int var_30;
	int var_258 = NUM;	

	unsigned long var_181;
	unsigned long var_251;
	struct scan_control var_74 = {
		.field_1 = var_1,
		.field_20 = var_298,
		.field_32 = NUM,
		.field_36 = NUM,
		.field_53 = !var_230,
		.field_8 = field_8,
		.field_3 = NULL,
	};
	func_35(var_73);

	do {
		unsigned long var_197 = NUM;
		unsigned long var_250 = NUM;
		bool var_8 = true;
		bool var_15 = (field_8 > NUM);

		var_74.field_26 = NUM;

		

		for (var_30 = var_184->field_54 - NUM; var_30 >= NUM; var_30--) {
			struct zone *field_39 = var_184->field_59 + var_30;

			if (!func_300(field_39))
				continue;

			if (var_74.field_20 != var_298 &&
			    !func_8(field_39))
				continue;

			

			func_216(field_39, &var_74);

			

			if (var_245 && func_114(var_30)) {
				var_258 = var_30;
				break;
			}

			if (!func_80(field_39, field_8, NUM, NUM)) {
				var_258 = var_30;
				break;
			} else {
				

				func_263(field_39, var_336);
				func_263(field_39, var_209);
			}
		}

		if (var_30 < NUM)
			goto out;

		for (var_30 = NUM; var_30 <= var_258; var_30++) {
			struct zone *field_39 = var_184->field_59 + var_30;

			if (!func_300(field_39))
				continue;

			var_197 += func_118(field_39);

			

			if (var_15 &&
					func_84(field_39, field_8,
						func_137(field_39),
						*field_24, NUM))
				var_15 = false;
		}

		

		if (var_74.field_20 < var_298 - NUM)
			var_74.field_53 = NUM;

		

		for (var_30 = NUM; var_30 <= var_258; var_30++) {
			struct zone *field_39 = var_184->field_59 + var_30;

			if (!func_300(field_39))
				continue;

			if (var_74.field_20 != var_298 &&
			    !func_8(field_39))
				continue;

			var_74.field_57 = NUM;

			var_251 = NUM;
			

			var_181 = func_156(field_39,
							field_8, var_74.field_1,
							&var_251);
			var_74.field_26 += var_181;

			

			if (func_241(field_39, var_258, &var_74,
					var_197, &var_250))
				var_8 = false;
		}

		

		if (func_197(&var_184->field_18) &&
				func_212(var_184))
			func_267(&var_184->field_18);

		

		if (field_8 && var_74.field_26 >= 2UL << field_8)
			field_8 = var_74.field_8 = NUM;

		

		if (func_268() || func_130())
			break;

		

		if (var_15 && var_74.field_26 > var_250)
			func_185(var_184, field_8);

		

		if (var_8 || !var_74.field_26)
			var_74.field_20--;
	} while (var_74.field_20 >= NUM &&
		 !func_3(var_184, field_8, *field_24));

out:
	

	*field_24 = var_258;
	return field_8;
}

static void func_306(pg_data_t *var_184, int field_8, int field_24)
{
	long var_330 = NUM;
	func_73(var_335);

	if (func_215(var_66) || func_130())
		return;

	func_34(&var_184->field_17, &var_335, var_186);

	

	if (func_91(var_184, field_8, var_330, field_24)) {
		var_330 = func_62(var_7/NUM);
		func_180(&var_184->field_17, &var_335);
		func_34(&var_184->field_17, &var_335, var_186);
	}

	

	if (func_91(var_184, field_8, var_330, field_24)) {
		func_113(var_184->field_44);

		

		func_153(var_184, var_129);

		

		func_66(var_184);

		if (!func_130())
			func_30();

		func_153(var_184, var_179);
	} else {
		if (var_330)
			func_35(var_204);
		else
			func_35(var_210);
	}
	func_180(&var_184->field_17, &var_335);
}



static int field_34(void *var_218)
{
	unsigned long field_8, var_25;
	unsigned var_200;
	int field_24, var_4;
	int var_226;
	pg_data_t *var_184 = (pg_data_t*)var_218;
	struct task_struct *var_349 = var_66;

	struct reclaim_state field_0 = {
		.field_27 = NUM,
	};
	const struct cpumask *var_28 = func_131(var_184->field_44);

	func_208(var_1);

	if (var_20 == NULL && !func_299(var_28))
		func_148(var_349, var_28);
	var_66->field_0 = &field_0;

	

	var_349->field_51 |= var_323 | var_322 | var_291;
	func_138();

	field_8 = var_25 = NUM;
	var_200 = NUM;
	field_24 = var_4 = var_184->field_54 - NUM;
	var_226 = field_24;
	for ( ; ; ) {
		bool var_91;

		

		if (var_226 >= var_4 &&
					var_200 == var_25) {
			var_25 = var_184->field_46;
			var_4 = var_184->field_24;
			var_184->field_46 =  NUM;
			var_184->field_24 = var_184->field_54 - NUM;
		}

		if (field_8 < var_25 || field_24 > var_4) {
			

			field_8 = var_25;
			field_24 = var_4;
		} else {
			func_306(var_184, var_200,
						var_226);
			field_8 = var_184->field_46;
			field_24 = var_184->field_24;
			var_25 = field_8;
			var_4 = field_24;
			var_184->field_46 = NUM;
			var_184->field_24 = var_184->field_54 - NUM;
		}

		var_91 = func_268();
		if (func_130())
			break;

		

		if (!var_91) {
			func_183(var_184->field_44, field_8);
			var_226 = field_24;
			var_200 = func_307(var_184, field_8,
						&var_226);
		}
	}

	var_349->field_51 &= ~(var_323 | var_322 | var_291);
	var_66->field_0 = NULL;
	func_251();

	return NUM;
}



void func_13(struct zone *field_39, int field_8, enum zone_type field_24)
{
	pg_data_t *var_184;

	if (!func_300(field_39))
		return;

	if (!func_170(field_39, var_1))
		return;
	var_184 = field_39->field_2;
	if (var_184->field_46 < field_8) {
		var_184->field_46 = field_8;
		var_184->field_24 = func_109(var_184->field_24, field_24);
	}
	if (!func_197(&var_184->field_17))
		return;
	if (func_257(field_39, field_8, func_137(field_39), NUM, NUM))
		return;

	func_297(var_184->field_44, func_117(field_39), field_8);
	func_230(&var_184->field_17);
}

#ifdef var_273


unsigned long func_239(unsigned long field_45)
{
	struct reclaim_state field_0;
	struct scan_control var_74 = {
		.field_1 = var_183,
		.field_36 = NUM,
		.field_32 = NUM,
		.field_53 = NUM,
		.field_45 = field_45,
		.field_55 = NUM,
		.field_8 = NUM,
		.field_20 = var_298,
	};
	struct shrink_control field_29 = {
		.field_1 = var_74.field_1,
	};
	struct zonelist *var_9 = func_163(func_6(), var_74.field_1);
	struct task_struct *var_218 = var_66;
	unsigned long field_26;

	var_218->field_51 |= var_323;
	func_208(var_74.field_1);
	field_0.field_27 = NUM;
	var_218->field_0 = &field_0;

	field_26 = func_286(var_9, &var_74, &field_29);

	var_218->field_0 = NULL;
	func_251();
	var_218->field_51 &= ~var_323;

	return field_26;
}
#endif 




static int func_38(struct notifier_block *var_32, unsigned long var_189,
			void *var_326)
{
	int var_283;

	if (var_189 == var_342 || var_189 == var_143) {
		func_207(var_283, var_137) {
			pg_data_t *var_184 = func_280(var_283);
			const struct cpumask *var_101;

			var_101 = func_131(var_184->field_44);

			if (func_11(var_134, var_101) < var_163)
				

				func_148(var_184->field_34, var_101);
		}
	}
	return var_316;
}

static int func_213(pg_data_t *var_184)
{
	int var_91 = NUM;
	cpumask_t var_292;

	if (!var_20)
		return NUM;

	func_54(var_292);
	var_91 = func_284(var_20, &var_292);
	if (var_91)
		return var_91;

	return func_148(var_184->field_34, &var_292);
}



int func_190(int var_283)
{
	pg_data_t *var_184 = func_280(var_283);
	int var_91 = NUM;

	if (var_184->field_34)
		return NUM;

	var_184->field_34 = func_47(field_34, var_184, STR, var_283);
	if (func_151(var_184->field_34)) {
		

		func_202(var_231 == var_178);
		func_195(STR, var_283);
		var_91 = func_311(var_184->field_34);
		var_184->field_34 = NULL;
	} else if (var_20) {
		if (func_213(var_184))
			func_17(STR);
	}
	return var_91;
}



void func_179(int var_283)
{
	struct task_struct *field_34 = func_280(var_283)->field_34;

	if (field_34) {
		func_96(field_34);
		func_280(var_283)->field_34 = NULL;
	}
}

static int var_346 func_165(void)
{
	int var_283;

	func_140();
	func_207(var_283, var_137)
 		func_190(var_283);
	if (var_20 == NULL)
		func_92(func_38, NUM);
	return NUM;
}

var_81(kswapd_init)

#ifdef var_174


int var_207 var_127;

#define var_50 NUM
#define var_26 (NUM<<NUM)	

#define var_288 (NUM<<NUM)	

#define var_246 (NUM<<NUM)	




#define var_47 NUM



int var_40 = NUM;



int var_34 = NUM;

static inline unsigned long func_222(struct zone *field_39)
{
	unsigned long var_324 = func_19(field_39, var_305);
	unsigned long var_270 = func_19(field_39, var_296) +
		func_19(field_39, var_214);

	

	return (var_270 > var_324) ? (var_270 - var_324) : NUM;
}



static long func_32(struct zone *field_39)
{
	long var_199;
	long var_232 = NUM;

	

	if (var_207 & var_246)
		var_199 = func_19(field_39, var_277);
	else
		var_199 = func_222(field_39);

	

	if (!(var_207 & var_288))
		var_232 += func_19(field_39, var_266);

	

	if (func_172(var_232 > var_199))
		var_232 = var_199;

	return var_199 - var_232;
}



static int func_228(struct zone *field_39, gfp_t field_1, unsigned int field_8)
{
	

	const unsigned long var_192 = NUM << field_8;
	struct task_struct *var_218 = var_66;
	struct reclaim_state field_0;
	struct scan_control var_74 = {
		.field_53 = !!(var_207 & var_288),
		.field_32 = !!(var_207 & var_246),
		.field_36 = NUM,
		.field_45 = func_210(var_192, var_149),
		.field_1 = (field_1 = func_255(field_1)),
		.field_8 = field_8,
		.field_20 = var_47,
	};
	struct shrink_control field_29 = {
		.field_1 = var_74.field_1,
	};
	unsigned long var_17, var_45;

	func_76();
	

	var_218->field_51 |= var_323 | var_322;
	func_208(field_1);
	field_0.field_27 = NUM;
	var_218->field_0 = &field_0;

	if (func_32(field_39) > field_39->field_35) {
		

		do {
			func_95(field_39, &var_74);
		} while (var_74.field_26 < var_192 && --var_74.field_20 >= NUM);
	}

	var_17 = func_19(field_39, var_92);
	if (var_17 > field_39->field_10) {
		

		for (;;) {
			unsigned long var_197 = func_118(field_39);

			

			if (!func_301(&field_29, var_74.field_57, var_197))
				break;

			

			var_45 = func_19(field_39,
							var_92);
			if (var_45 + var_192 <= var_17)
				break;
		}

		

		var_45 = func_19(field_39, var_92);
		if (var_45 < var_17)
			var_74.field_26 += var_17 - var_45;
	}

	var_218->field_0 = NULL;
	var_66->field_51 &= ~(var_323 | var_322);
	func_251();
	return var_74.field_26 >= var_192;
}

int func_12(struct zone *field_39, gfp_t field_1, unsigned int field_8)
{
	int field_44;
	int var_91;

	

	if (func_32(field_39) <= field_39->field_35 &&
	    func_19(field_39, var_92) <= field_39->field_10)
		return var_153;

	if (!func_8(field_39))
		return var_153;

	

	if (!(field_1 & var_297) || (var_66->field_51 & var_323))
		return var_103;

	

	field_44 = func_103(field_39);
	if (func_236(field_44, var_107) && field_44 != func_6())
		return var_103;

	if (func_145(field_39, var_180))
		return var_103;

	var_91 = func_228(field_39, field_1, field_8);
	func_263(field_39, var_180);

	if (!var_91)
		func_35(var_162);

	return var_91;
}
#endif



int func_146(struct page *var_203)
{
	return !func_189(func_2(var_203)) && !func_252(var_203);
}

#ifdef var_264


void func_154(struct page **var_345, int var_192)
{
	struct lruvec *var_331;
	struct zone *field_39 = NULL;
	int var_11 = NUM;
	int var_157 = NUM;
	int var_30;

	for (var_30 = NUM; var_30 < var_192; var_30++) {
		struct page *var_203 = var_345[var_30];
		struct zone *var_243;

		var_11++;
		var_243 = func_233(var_203);
		if (var_243 != field_39) {
			if (field_39)
				func_56(&field_39->field_12);
			field_39 = var_243;
			func_67(&field_39->field_12);
		}
		var_331 = func_175(var_203, field_39);

		if (!func_121(var_203) || !func_106(var_203))
			continue;

		if (func_146(var_203)) {
			enum lru_list field_23 = func_261(var_203);

			func_288(func_231(var_203));
			func_160(var_203);
			func_201(var_203, var_331, var_168);
			func_16(var_203, var_331, field_23);
			var_157++;
		}
	}

	if (field_39) {
		func_298(var_114, var_157);
		func_298(var_19, var_11);
		func_56(&field_39->field_12);
	}
}
#endif 


static void func_304(void)
{
	var_172(var_308
		    STR
		    STR
		    STR,
		    var_66->field_38);
}



unsigned long var_198;

int func_219(struct ctl_table *var_83, int var_285,
			   void var_12 *var_225,
			   size_t *var_293, loff_t *var_175)
{
	func_304();
	func_224(var_83, var_285, var_225, var_293, var_175);
	var_198 = NUM;
	return NUM;
}

#ifdef var_174



static ssize_t func_90(struct device *field_48,
					  struct device_attribute *var_128,
					  char *var_132)
{
	func_304();
	return func_112(var_132, STR);	

}

static ssize_t func_22(struct device *field_48,
					   struct device_attribute *var_128,
					const char *var_132, size_t var_169)
{
	func_304();
	return NUM;
}


static var_173(scan_unevictable_pages, var_99 | var_177,
			read_scan_unevictable_node,
			write_scan_unevictable_node)var_51;

int func_250(struct node *var_18)
{
	return func_273(&var_18->field_48, &var_16);
}

void func_70(struct node *var_18)
{
	func_119(&var_18->field_48, &var_16);
}
#endif

