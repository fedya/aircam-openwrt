# Source file used to test DWARF2 information for AVR32.

	.file	"main.c"

	.section .debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section .debug_info,"",@progbits
.Ldebug_info0:
	.section .debug_line,"",@progbits
.Ldebug_line0:

	.text
	.align	1
	.globl	main
	.type	main, @function
.Ltext0:
main:
	.file 1 "main.c"
	.loc 1 88 0
	pushm	r0-r7,lr
	sub	sp, 4
	.loc 1 111 0
	lddpc	r12, .LC1
	lddpc	r7, .LC1
	icall	r7
	.loc 1 112 0
	lddpc	r6, .LC4

	.align	2
.LC4:	.int	0

	.fill	256, 2, 0

	.align	2
.LC1:
	.int	0
.LC2:
	.int	0
.LC3:
	.int	0
	.size	main, . - main

.Letext0:

	.section .debug_info
	.int	.Ledebug_info0 - .Ldebug_info0	// size
	.short	2				// version
	.int	.Ldebug_abbrev0			// abbrev offset
	.byte	4				// bytes per addr

	.uleb128 1				// abbrev 1
	.int	.Ldebug_line0			// DW_AT_stmt_list
	.int	.Letext0			// DW_AT_high_pc
	.int	.Ltext0				// DW_AT_low_pc

.Ledebug_info0:

	.section .debug_abbrev
	.uleb128 0x01
	.uleb128 0x11		// DW_TAG_compile_unit
	.byte	0		// DW_CHILDREN_no
	.uleb128 0x10, 0x6	// DW_AT_stmt_list
	.uleb128 0x12, 0x1	// DW_AT_high_pc
	.uleb128 0x11, 0x1	// DW_AT_low_pc
	.uleb128 0, 0

	.byte	0
