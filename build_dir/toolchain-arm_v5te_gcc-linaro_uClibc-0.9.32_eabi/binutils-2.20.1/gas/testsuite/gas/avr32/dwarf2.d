#readelf: -wl
#name: dwarf2
#source: dwarf2.s

Dump of debug contents of section \.debug_line:

  Length:                      53
  DWARF Version:               2
  Prologue Length:             26
  Minimum Instruction Length:  1
  Initial value of 'is_stmt':  1
  Line Base:                   -5
  Line Range:                  14
  Opcode Base:                 10
  \(Pointer size:               4\)

 Opcodes:
  Opcode 1 has 0 args
  Opcode 2 has 1 args
  Opcode 3 has 1 args
  Opcode 4 has 1 args
  Opcode 5 has 1 args
  Opcode 6 has 0 args
  Opcode 7 has 0 args
  Opcode 8 has 0 args
  Opcode 9 has 1 args

 The Directory Table is empty\.

 The File Name Table:
  Entry	Dir	Time	Size	Name
  1	0	0	0	main\.c

 Line Number Statements:
  Extended opcode 2: set Address to 0x0
  Advance Line by 87 to 88
  Copy
  Advance Line by 23 to 111
  Special opcode .*: advance Address by 4 to 0x4 and Line by 0 to 111
  Special opcode .*: advance Address by 10 to 0xe and Line by 1 to 112
  Advance PC by 530 to 220
  Extended opcode 1: End of Sequence
