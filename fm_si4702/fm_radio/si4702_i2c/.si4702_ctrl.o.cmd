cmd_/home/zhangda/projects/teaching/fm_radio/si4702_i2c/si4702_ctrl.o := /usr/local/arm/4.3.2/bin/arm-linux-gcc -Wp,-MD,/home/zhangda/projects/teaching/fm_radio/si4702_i2c/.si4702_ctrl.o.d  -nostdinc -isystem /usr/local/arm/4.3.2/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/include -I/home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-s5pv210/include -Iarch/arm/plat-s5p/include -Iarch/arm/plat-samsung/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -fno-stack-protector -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(si4702_ctrl)"  -D"KBUILD_MODNAME=KBUILD_STR(s5pv210_si4702)"  -c -o /home/zhangda/projects/teaching/fm_radio/si4702_i2c/si4702_ctrl.o /home/zhangda/projects/teaching/fm_radio/si4702_i2c/si4702_ctrl.c

deps_/home/zhangda/projects/teaching/fm_radio/si4702_i2c/si4702_ctrl.o := \
  /home/zhangda/projects/teaching/fm_radio/si4702_i2c/si4702_ctrl.c \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/delay.h \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/delay.h \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /usr/local/arm/4.3.2/bin/../lib/gcc/arm-none-linux-gnueabi/4.3.2/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/linkage.h \
  include/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/posix_types.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/bitops.h \
    $(wildcard include/config/smp.h) \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/system.h \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/32v6k.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/irqflags.h \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/hwcap.h \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/dynamic_debug.h \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/div64.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/string.h \
  /home/zhangda/projects/teaching/fm_radio/si4702_i2c/si4702_ctrl.h \
  /home/zhangda/projects/teaching/fm_radio/si4702_i2c/si4702_i2c.h \
  arch/arm/mach-s5pv210/include/mach/regs-gpio.h \
  arch/arm/mach-s5pv210/include/mach/map.h \
    $(wildcard include/config/mach/smdkv210.h) \
  arch/arm/plat-samsung/include/plat/map-base.h \
  arch/arm/plat-s5p/include/plat/map-s5p.h \
  include/linux/gpio.h \
    $(wildcard include/config/generic/gpio.h) \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/gpio.h \
  arch/arm/mach-s5pv210/include/mach/gpio.h \
    $(wildcard include/config/s3c/gpio/space.h) \
    $(wildcard include/config/samsung/gpio/extra.h) \
  include/asm-generic/gpio.h \
    $(wildcard include/config/gpiolib.h) \
    $(wildcard include/config/gpio/sysfs.h) \
    $(wildcard include/config/have/gpio/lib.h) \
  include/linux/errno.h \
  /home/zhangda/kernel/unsp210_linux-2.6.35.7/arch/arm/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  arch/arm/plat-samsung/include/plat/gpio-cfg.h \

/home/zhangda/projects/teaching/fm_radio/si4702_i2c/si4702_ctrl.o: $(deps_/home/zhangda/projects/teaching/fm_radio/si4702_i2c/si4702_ctrl.o)

$(deps_/home/zhangda/projects/teaching/fm_radio/si4702_i2c/si4702_ctrl.o):
